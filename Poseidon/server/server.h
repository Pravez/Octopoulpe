#ifndef POSEIDON_SERVER_H
#define POSEIDON_SERVER_H

#include <sys/queue.h>
#include <pthread.h>

#define BUFFER_SIZE 256


struct listhead *headp;
struct thread_entry {
    LIST_ENTRY(entry) entries;
    pthread_t _thread;
};

void* start(void* arg);
void wait_connection(int portno);
void* server_process(void* arg);
char* parse(char buffer[BUFFER_SIZE]);

char* check_client_id();

#endif //POSEIDON_SERVER_H
