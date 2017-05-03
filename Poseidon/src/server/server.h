#ifndef POSEIDON_SERVER_H
#define POSEIDON_SERVER_H

#include <sys/queue.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../utility/tools.h"
#include "../utility/data.h"
#include "communication/answer.h"
#include "communication/client.h"

struct server_p{
    struct sockaddr_in _server_socket;
    int _listen_socket_fd;

    pthread_t _link_keeper;
};

void* server_process(void* arg);
void server__init(struct server_p* server, int port);
void server__wait_connection(struct server_p* server);
void* server__check_connections(void* args);
void server__stop(int signo);


#endif //POSEIDON_SERVER_H
