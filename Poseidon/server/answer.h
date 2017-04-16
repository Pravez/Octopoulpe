#ifndef POSEIDON_ANSWER_H
#define POSEIDON_ANSWER_H

#include <sys/queue.h>

#define HELLO_SUCCESS 0
#define HELLO_FAILURE 1

struct client {
    char * id;
    int is_free;
    LIST_ENTRY(client) entries;
};
/* Functions for the server thread */
int asw__hello(char * arg, char * res, struct client *cli);
void asw__get_fishes(char * arg, char * res);
void asw__get_fishes_continuously(char * arg, char * res);
void asw__ping(char * arg, char * res);
void asw__log(char * arg, char * res);
void asw__add_fish(char * arg, char * res);
void asw__del_fish(char * arg, char * res);
void asw__start_fish(char * arg, char * res);

/* Functions for the aquarium */
void asw__init_aquarium(void);
void asw__add_view(char *id);
void asw__remove_view(char *id);
void asw__remove_aquarium(void);

#endif //POSEIDON_ANSWER_H
