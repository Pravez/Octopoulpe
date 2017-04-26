#ifndef POSEIDON_SERVER_H
#define POSEIDON_SERVER_H

#include <sys/queue.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "answer.h"

#define BUFFER_SIZE 256

struct thread_p{
    struct sockaddr _client_socket;
    pthread_t _thread;
    struct client _client;
    int _thread_id;
    int _socket_fd;
    int _addr_len;
};

struct server_p{
    struct sockaddr_in _server_socket;
    int _listen_socket_fd;
};

struct listhead *headp;
struct thread_entry {
    LIST_ENTRY(entry) entries;
    struct thread_p _thread;
};

void* server_process(void* arg);
void server__init(struct server_p* server, int port);
void server__wait_connection(struct server_p* server);

/**
 * @fn      start
 * @param   arg file descriptor of the current socket
 */
void* client__start(void* arg);
char* client__parse_command(char buffer[BUFFER_SIZE], struct client* client);

/**
 * @fn      get_fishes_continuously
 * @param   time
 * @return  char result list of fishes
 */
void *get_fishes_continuously(void *time);

char* send__client_id(struct client* client);

#endif //POSEIDON_SERVER_H
