#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "server.h"
#include "../utility/tools.h"

#define PARAM 2
#define LISTEN_QUEUE 10
#define CHOSE_AUTOMATICALLY 0

LIST_HEAD(listhead, thread_entry) thread_head = LIST_HEAD_INITIALIZER(thread_head);

static const char *delim = " ";
static int com[2];
static int thread_ids = 0;

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
}

void server__wait_connection(struct server_p *server) {
    struct thread_entry* entry;

    //We listen to connections
    listen(server->_listen_socket_fd, LISTEN_QUEUE);

    while (1) {
        //Setting up the client structure
        entry = malloc(sizeof(struct thread_entry));
        entry->_thread._addr_len = sizeof(entry->_thread._client_socket);

        //Waiting for a connection
        if ((entry->_thread._socket_fd = accept(server->_listen_socket_fd, &entry->_thread._client_socket,
                                                (socklen_t *) &entry->_thread._addr_len))){

            entry->_thread._thread_id = thread_ids++;
            LIST_INSERT_HEAD(&thread_head, entry, entries);
            CHK_ERROR(pthread_create(&entry->_thread._thread, NULL, client__start, &entry->_thread),
                      "Error creating thread") //when do we destroy the thread ?
            //TODO CLOSE SOCKET
        }else{
            break;
        }
    }

    free(entry);
    close(server->_listen_socket_fd);
}

void *client__start(void *arg) {

    char buffer[BUFFER_SIZE];
    struct thread_p *thread = (struct thread_p *) arg;
    int n;
    char *response;
    //create a pipe to communicate between threads
    int com[2];
    while (1) {
        bzero(buffer, BUFFER_SIZE);
        n = read(thread->_socket_fd, buffer, BUFFER_SIZE - 1);
        //printf("n: %d\n", n);
        CHK_ERROR(n, "Error reading from socket")
        else if (n == 2) { //fix the segfault in case of empty message
            n = write(thread->_socket_fd, "\n", 1);
            CHK_ERROR(n, "Error writing to socket")
        } else {
            response = client__parse_command(buffer, &thread->_client);
            //traitement
            //read the result from the pipe and write it
            /*if() {//check if the pipe is not empty
              n = write(newsockfd, ...)
            }*/
            n = write(thread->_socket_fd, response, strlen(response));
            CHK_ERROR(n, "Error writing to socket");
            if (!strcmp(response, "no greeting")) {
                pthread_exit(NULL);
            }

        }
    }
}

char *client__parse_command(char buffer[BUFFER_SIZE], struct client *client) {
    //char *res;// = malloc(sizeof(char)*MAX);

    char *cmd;
    asprintf(&cmd, "%s", buffer);

    cmd[strlen(cmd) - 2] = '\0';

    char *token = strtok(cmd, delim);
    if (!strcmp(token, "hello")) {
        return send__client_id(client);
    } else if (!strcmp(token, "getFishes")) {
        //return send__
    } else if (!strcmp(token, "getFishesContinuously")) {
        //create a pipe
        pipe(com);
        //launch thread
        //where should I write ? A new pipe ? -> quite a good idea !
        pthread_t moult_fishes;
        pthread_create(&moult_fishes, NULL, get_fishes_continuously, 0);
    } else if (!strcmp(token, "log")) {

    } else if (!strcmp(token, "ping")) {

    } else if (!strcmp(token, "addFish")) {

    } else if (!strcmp(token, "delFish")) {

    } else if (!strcmp(token, "startFish")) {

    }

    free(cmd);
    return "Unknown command\n";

}

char *send__client_id(struct client *client) {
    char *result = NULL;

    char *in = strtok(NULL, delim);
    char *as = strtok(NULL, delim);
    char *id = strtok(NULL, delim);
    char *str = NULL;
    if (in != NULL)
        asprintf(&str, "%s %s %s", in, as, id);

    asw__hello(str, &result, client);
    free(str);
    return result;
}

//for testing time = 0
void *get_fishes_continuously(void *time) {
    int delay = (int) time;
    char *res;
    while (1) {
        sleep(delay);
        asw__get_fishes(NULL, res, NULL);
        //lock ?
        write(com[1], res, strlen(res));
        //unlock
    }
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
