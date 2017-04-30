#ifndef POSEIDON_CLIENT_H
#define POSEIDON_CLIENT_H

#include <sys/queue.h>
#include <pthread.h>
#include <time.h>
#include <netinet/in.h>

#include "../utility/data.h"

struct client {
    char *id;
    int is_free;
    int _connected;
    struct aquarium_view *aqv;
    LIST_ENTRY(client) entries;

    pthread_t _continuous_sender;
    int _is_observer;

    int _socket_fd;

    time_t _last_ping;
    pthread_mutex_t _time_mutex;
};

struct thread_p{
    struct sockaddr _client_socket;
    int _addr_len;

    pthread_t _thread;
    int _thread_working;

    struct client _client;
    char _last_message[BUFFER_SIZE];
};

struct listhead *headp;
struct thread_entry {
    LIST_ENTRY(thread_entry) thread_entries;
    struct thread_p _thread;
};

void* client__start(void* arg);
char* client__parse_command(char buffer[BUFFER_SIZE], struct client* client);
void client__init(struct thread_p* client_thread);
void client__destroy(struct thread_entry* client_thread);

#endif //POSEIDON_CLIENT_H
