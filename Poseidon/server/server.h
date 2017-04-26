#ifndef POSEIDON_SERVER_H
#define POSEIDON_SERVER_H

#include <sys/queue.h>
#include <pthread.h>
#include "answer.h"

#define BUFFER_SIZE 256

struct thread_p{
    pthread_t _thread;
    struct client _client;
    int _thread_id;
    int _sockfd;
};

struct listhead *headp;
struct thread_entry {
    LIST_ENTRY(entry) entries;
    struct thread_p _thread;
};
/**
 * @fn      start
 * @param   arg file descriptor of the current socket
 */
void* start(void* arg);
/**
 * @fn      get_fishes_continuously
 * @param   time
 * @return  char result list of fishes
 */
void *get_fishes_continuously(void *time);
void wait_connection(int portno);
void* server_process(void* arg);
char* parse(char buffer[BUFFER_SIZE], struct client* client);

char* send__client_id(struct client* client);

#endif //POSEIDON_SERVER_H
