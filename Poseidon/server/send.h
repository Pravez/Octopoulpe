#ifndef POSEIDON_SEND_H
#define POSEIDON_SEND_H

#include "answer.h"

char *send__client_id(struct client *client);
char* send__fishes(struct client* client);
char *send__fishes_continuously(struct client *client, void *time);
char* send__add_fish(struct client* client);
char* send__logout(struct client* client);


#endif //POSEIDON_SEND_H
