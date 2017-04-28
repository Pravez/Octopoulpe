#ifndef POSEIDON_SEND_H
#define POSEIDON_SEND_H

#include "answer.h"

char *send__client_id(struct client *client);
char* send__fishes(struct client* client);
void send__fishes_continuously(struct client* client);
char* send__add_fish(struct client* client);
char* send__delete_fish(struct client* client);
char* send__logout(struct client* client);
char* send__ping(struct client* client);
char* send__start_fish(struct client* client);
char* send__logout(struct client* client);



#endif //POSEIDON_SEND_H