#include <stdio.h>
#include <string.h>

#include "fish.h"

static int _fish_ids = 0;


struct fish* fish__create(enum fish_type type, int x, int y, char* id, enum MOVING_STRATEGY strategy, struct dimension dimension) {
    struct fish* f = malloc(sizeof(struct fish));
    if(id == NULL)
        asprintf(&f->_id, "%s%d", get_type_string(type), _fish_ids++);
    else
        f->_id = id;
    f->_current = __position(x, y);
    f->_goal = __position(x, y);
    f->_cover = dimension;
    f->_type = type;
    f->_speed_rate = FISH_SPEEDRATE;
    f->_strategy = strategy;

    return f;
}

int fish__free(any_t fish, any_t nothing){
    struct fish* f = fish;
    free(f);
    return MAP_OK;
}

void fish__set_position(struct fish* fish, struct position newpos){
    fish->_current = newpos;
}

void fish__add_to_position(struct fish* fish, int x, int y){
    fish->_current = add_to_position(fish->_current, x, y);
}

void fish__set_goal(struct fish* fish, struct position goal){
    fish->_goal = goal;
}