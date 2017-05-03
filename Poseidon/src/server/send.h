#ifndef POSEIDON_SEND_H
#define POSEIDON_SEND_H

#include "answer.h"
#include "client.h"

char *send__client_id(struct thread_p *thread);
char* send__fishes(struct client* client);
void send__signal_callback_handler(int signum);
void* send__regular_sender(void* arg);
char* send__fishes_continuously(struct thread_p* thread);
char* send__add_fish(struct client* client);
char* send__delete_fish();
char* send__logout(struct thread_p* thread);
char* send__ping(struct client* client);
char* send__start_fish();
char* send__stop_send_continuously(struct thread_p* thread);

void __stop_send_continuously(struct thread_p* thread);

#endif //POSEIDON_SEND_H
