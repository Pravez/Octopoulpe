#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "server.h"
#include "send.h"
#include "../utility/data.h"
#include "../utility/vector.h"

LIST_HEAD(listhead, thread_entry) thread_head = LIST_HEAD_INITIALIZER(thread_head);

extern struct aquarium *aquarium;
extern pthread_mutex_t mutex_observers;
struct vector* observers;

static int check_threads_connection = TRUE;
static struct server_p server;

int server_working = TRUE;

#ifdef _PROCESS_

void *server_process(void *arg) {
    LIST_INIT(&thread_head);

    //Then we launch listening
    server__init(&server, CONTROLLER_PORT);
    server__wait_connection(&server);

    pthread_exit(NULL);
}

#endif //_PROCESS_

void server__init(struct server_p *server, int port) {
    //We initialize observers vector
    observers = malloc(sizeof(struct vector));
    v__init(observers, DEFAULT_OBSERVERS_QTY);
    //and say it's okay
    UNLOCK(&mutex_observers);

    //Creating the socket to listen to
    server->_listen_socket_fd = socket(AF_INET, SOCK_STREAM, CHOSE_AUTOMATICALLY);
    CHK_ERROR(server->_listen_socket_fd, "Error opening socket")

    //Is it really useful ?
    bzero((char *) &server->_server_socket, sizeof(server->_server_socket));
    //Creating the server structure
    server->_server_socket.sin_family = AF_INET;
    server->_server_socket.sin_addr.s_addr = INADDR_ANY;
    server->_server_socket.sin_port = htons((uint16_t) port);

    //Binding the socket to the server structure
    CHK_ERROR(bind(server->_listen_socket_fd, (struct sockaddr *) &server->_server_socket,
                   sizeof(server->_server_socket)), "Error on binding")

    //We create the thread in charge of verifying clients are always connected
    CHK_ERROR(pthread_create(&server->_link_keeper, NULL, server__check_connections, NULL), "Error creating thread");

    //We set the mutex for the threads list
    pthread_mutex_init(&threads_list_mutex, NULL);

    //If we get a sigstop, we stop everything
    signal(SIGNAL_END_EVERYTHING, server__stop);
}

void server__stop(int signo){
    if(signo == SIGNAL_END_EVERYTHING){
        server_working = FALSE;
        check_threads_connection = FALSE;

        struct thread_entry* en;
        LIST_FOREACH(en, &thread_head, thread_entries){
            en->_thread._connected = FALSE;
            pthread_join(en->_thread._thread, NULL);
            LIST_REMOVE(en, thread_entries);
            free(en);
        }

        pthread_join(server._link_keeper, NULL);

        shutdown(server._listen_socket_fd, SHUT_RDWR);
    }
}

void server__wait_connection(struct server_p *server) {
    struct thread_entry* entry = NULL;

    //We listen to connections
    listen(server->_listen_socket_fd, LISTEN_QUEUE);

    while (server_working) {
        //Setting up the client structure
        entry = malloc(sizeof(struct thread_entry));
        entry->_thread._addr_len = sizeof(entry->_thread._client_socket);

        //Waiting for a connection
        if ((entry->_thread._socket_fd = accept(server->_listen_socket_fd, &entry->_thread._client_socket,
                                                (socklen_t *) &entry->_thread._addr_len)) != -1){

            LOCK(&threads_list_mutex);
            LIST_INSERT_HEAD(&thread_head, entry, thread_entries);
            UNLOCK(&threads_list_mutex);

            CHK_ERROR(pthread_create(&entry->_thread._thread, NULL, client__start, entry),
                      "Error creating thread") //when do we destroy the thread ?
        }else{
            break;
        }


    }

    if(entry != NULL)
        free(entry);

    close(server->_listen_socket_fd);
}

void* server__check_connections(void* args){
    time_t now;
    struct thread_entry* en;

    while(check_threads_connection){
        if(!LIST_EMPTY(&thread_head)){
            time(&now);

            LOCK(&threads_list_mutex);
            LIST_FOREACH(en, &thread_head, thread_entries){
                if(en->_thread._authenticated){
                    LOCK(&en->_thread._time_mutex);
                    if(difftime(now, en->_thread._last_ping) > (double) MAXIMUM_IDLE_TIME){
                        _console_log(LOG_MEDIUM, "Thread has not sent anything for 10 seconds, terminating it ...");
                        en->_thread._connected = FALSE;
                        shutdown(en->_thread._socket_fd, SHUT_RD);
                    }
                    UNLOCK(&en->_thread._time_mutex);
                }
            }
            UNLOCK(&threads_list_mutex);
        }

        sleep(TIME_BETWEEN_CHECKS);
    }

    pthread_exit(NULL);
}

#ifndef _PROCESS_
int main(int argc, char *argv[]) {
    if (argc < PARAM) {
    fprintf(stderr, "ERROR no port provided\n");
    exit(1);
  }
  int portno = atoi(argv[1]);
  wait_connection(portno);
  return 0;
}
#endif
