#ifndef POSEIDON_ANSWER_H
#define POSEIDON_ANSWER_H

#define HELLO_SUCCESS 0
#define HELLO_FAILURE 1

int asw__hello(char * arg, char * res);
void asw__get_fishes(char * arg, char * res);
void asw__get_fishes_continuously(char * arg, char * res);
void asw__ping(char * arg, char * res);
void asw__log(char * arg, char * res);
void asw__add_fish(char * arg, char * res);
void asw__del_fish(char * arg, char * res);
void asw__start_fish(char * arg, char * res);

#endif //POSEIDON_ANSWER_H
