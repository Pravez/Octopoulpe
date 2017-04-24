#ifndef POSEIDON_FISH_H
#define POSEIDON_FISH_H

#include <stdlib.h>
#include "../utility/tools.h"
#include "../utility/vector.h"
#include "../utility/fish_type.h"
#include "../utility/hashmap.h"
#include "../utility/data.h"

enum MOVING_STRATEGY{
    HORIZONTAL, VERTICAL, RANDOM
};

struct fish {
    enum fish_type _type;
    struct dimension _cover;
    struct position _current;
    struct position _goal;
    struct position _next_position;
    enum MOVING_STRATEGY _strategy;
    double _speed_rate;
    char *_id;
    int _running;
};

struct fish* fish__create(enum fish_type type, int x, int y, char* id, enum MOVING_STRATEGY strategy, struct dimension dimension);
int fish__free(any_t fish, any_t nothing);
void fish__set_position(struct fish* fish, struct position newpos);
void fish__add_to_position(struct fish* fish, int x, int y);
void fish__set_goal(struct fish* fish, struct position goal);

#endif //POSEIDON_FISH_H
