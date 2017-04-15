#ifndef POSEIDON_SERVER_H
#define POSEIDON_SERVER_H

#include <sys/queue.h>
#include <pthread.h>

LIST_HEAD(listhead, thread_entry) thread_head;
struct listhead *headp;
struct thread_entry {
    LIST_ENTRY(entry) entries;
    pthread_t* _thread;
};

void * start(void* arg);
void wait_connection(int portno);
void* server_process(void* arg);

#endif //POSEIDON_SERVER_H
