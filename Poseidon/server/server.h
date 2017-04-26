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
char* parse(char buffer[BUFFER_SIZE]);

char* send__client_id();

#endif //POSEIDON_SERVER_H
