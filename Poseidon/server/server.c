#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "server.h"
#include "send.h"
#include "../utility/data.h"
#include "../utility/vector.h"

#define PARAM 2
#define LISTEN_QUEUE 10
#define CHOSE_AUTOMATICALLY 0

#define DEFAULT_OBSERVERS_QTY 10

LIST_HEAD(listhead, thread_entry) thread_head = LIST_HEAD_INITIALIZER(thread_head);

extern struct aquarium *aquarium;
static const char *delim = SERVER_CMD_DELIMITER;
static int thread_ids = 0;

struct vector* observers;
extern pthread_mutex_t mutex_observers;

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
    pthread_mutex_unlock(&mutex_observers);

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
        if ((entry->_thread._client._socket_fd = accept(server->_listen_socket_fd, &entry->_thread._client_socket,
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

    struct thread_p *thread = (struct thread_p *) arg;
    thread->_client._connected = TRUE;
    int code_return;
    char *response;

    while (1) {
        //We clear entire array
        bzero(thread->_last_message, BUFFER_SIZE);

        code_return = (int) read(thread->_client._socket_fd, thread->_last_message, BUFFER_SIZE - 1);
        CHK_ERROR(code_return, "Error reading from socket")

        if (code_return == 2) {
            //fix the segfault in case of empty message
            code_return = (int) write(thread->_client._socket_fd, "\n", 1);
            CHK_ERROR(code_return, "Error writing to socket")

        } else {
            response = client__parse_command(thread->_last_message, &thread->_client);

            code_return = (int) write(thread->_client._socket_fd, response, strlen(response));
            CHK_ERROR(code_return, "Error writing to socket");

            if (!thread->_client._connected) {
                close(thread->_client._socket_fd);
                pthread_exit(NULL);
            }
        }
    }
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
            return "END";
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
