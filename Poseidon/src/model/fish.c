#include <stdio.h>
#include <string.h>

#include "fish.h"

static int _fish_ids = 0;


struct fish *
fish__create(enum fish_type type, int x, int y, char *id, enum MOVING_STRATEGY strategy, struct dimension dimension,
             double speed_rate) {
    struct fish *f = malloc(sizeof(struct fish));
    if (id == NULL)
        asprintf(&f->_id, "%s%d", get_type_string(type), _fish_ids++);
    else
        asprintf(&f->_id, "%s", id);
    f->_current = __position(x, y);
    f->_goal = __position(x, y);
    f->_cover = dimension;
    f->_type = type;
    f->_speed_rate = UPDATE_INTERVAL < 0 ? 1.0 / (-UPDATE_INTERVAL) : UPDATE_INTERVAL*10;
    f->_strategy = strategy;
    f->_running = 0;

    return f;
}

int fish__free(any_t fish, any_t nothing) {
    struct fish *f = fish;
    free(f);
    return MAP_OK;
}

void fish__set_position(struct fish *fish, struct position newpos) {
    fish->_current = newpos;
}

void fish__add_to_position(struct fish *fish, int x, int y) {
    fish->_current = add_to_position(fish->_current, x, y);
}

void fish__set_goal(struct fish *fish, struct position goal) {
    fish->_goal = goal;
}

void fish__set_running_state(struct fish *fish, int state) {
    fish->_running = state;
}