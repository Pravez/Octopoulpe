#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "server.h"
#include "send.h"
#include "../utility/data.h"
#include "../utility/vector.h"

LIST_HEAD(listhead, thread_entry) thread_head = LIST_HEAD_INITIALIZER(thread_head);

extern struct aquarium *aquarium;
extern pthread_mutex_t mutex_observers;
struct vector* observers;

static const char *delim = SERVER_CMD_DELIMITER;
static int thread_ids = 0;
static int check_threads_connection = TRUE;
static pthread_mutex_t threads_list_mutex;

int server_working = TRUE;

#ifdef _PROCESS_

void *server_process(void *arg) {
    LIST_INIT(&thread_head);
    struct server_p server;

    //Then we launch listening
    server__init(&server, *(int *) arg);
    server__wait_connection(&server);
    return 0;
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
        if ((entry->_thread._client._socket_fd = accept(server->_listen_socket_fd, &entry->_thread._client_socket,
                                                (socklen_t *) &entry->_thread._addr_len))){

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
                LOCK(&en->_thread._client._time_mutex);
                if(difftime(now, en->_thread._client._last_ping) > (double) MAXIMUM_IDLE_TIME){
                    _console_log(LOG_MEDIUM, "Thread has not sent anything for 10 seconds, terminating it ...");
                    en->_thread._thread_working = FALSE;
                    shutdown(en->_thread._client._socket_fd, SHUT_RD);
                    LIST_REMOVE(en, thread_entries);
                }
                UNLOCK(&en->_thread._client._time_mutex);
            }
            UNLOCK(&threads_list_mutex);
        }

        sleep(TIME_BETWEEN_CHECKS);
    }

    pthread_exit(NULL);
}

void *client__start(void *arg) {

    struct thread_entry *entry = (struct thread_entry *) arg;
    struct thread_p* thread = &entry->_thread;
    int code_return;
    char *response;

    client__init(thread);

    //If something happened during actions that needs to stop everything
    //meaning if the client is still sending messages, and connected
    while (thread->_thread_working && thread->_client._connected) {
        //We clear entire array
        bzero(thread->_last_message, BUFFER_SIZE);

        code_return = (int) read(thread->_client._socket_fd, thread->_last_message, BUFFER_SIZE - 1);
        CHK_ERROR(code_return, "Error reading from socket")

        if(thread->_thread_working) {
            //We update last time client sent a message
            LOCK(&thread->_client._time_mutex);
            time(&thread->_client._last_ping);
            UNLOCK(&thread->_client._time_mutex);
        }

        if (code_return == 2) {
            //fix the segfault in case of empty message
            code_return = (int) write(thread->_client._socket_fd, "\n", 1);
            CHK_ERROR(code_return, "Error writing to socket")

        } else {
            //We parse command
            response = client__parse_command(thread->_last_message, &thread->_client);

            //We write answer
            if(response != NULL){
                code_return = (int) write(thread->_client._socket_fd, response, strlen(response));
                CHK_ERROR(code_return, "Error writing to socket");
            }
        }
    }

    client__destroy(entry);
    pthread_exit(NULL);
}

char *client__parse_command(char buffer[BUFFER_SIZE], struct client *client) {

    char *cmd;
    asprintf(&cmd, "%s", buffer);

    cmd[strlen(cmd) - 2] = '\0';

    char *token = strtok(cmd, delim);
    if(token == NULL){
        //The client closed brutally
        client->_connected = FALSE;
        return "\0";
    }

    if (!strcmp(token, "hello")) {
        if(client->id != NULL){
            return "Already authenticated\n";
        }
        return send__client_id(client);
    }else if(client->id != NULL){
        if (!strcmp(token, "getFishes")) {
            return send__fishes(client);
        } else if (!strcmp(token, "getFishesContinuously")) {
            send__fishes_continuously(client);
            return "End of transaction";
        } else if (!strcmp(token, "log")) {
            return send__logout(client);
        } else if (!strcmp(token, "ping")) {
            return send__ping(client);
        } else if (!strcmp(token, "addFish")) {
            return send__add_fish(client);
        } else if (!strcmp(token, "delFish")) {
            return send__delete_fish(client);
        } else if (!strcmp(token, "startFish")) {
            return send__start_fish(client);
        }
    } else{
        return "Please authenticate yourself with a `hello` command first\n";
    }

    free(cmd);
    return "Unknown command\n";

}

void client__init(struct thread_p* client_thread){
    time(&client_thread->_client._last_ping);
    client_thread->_client._is_observer = FALSE;
    client_thread->_client._connected = TRUE;
    client_thread->_thread_working = TRUE;
    pthread_mutex_init(&client_thread->_client._time_mutex, NULL);
}

void client__destroy(struct thread_entry* client_thread){
    close(client_thread->_thread._client._socket_fd);
    free(client_thread->_thread._client.id);
    free(client_thread);
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
