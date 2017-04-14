#include <stdio.h>
#include <string.h>
#include "fish.h"

#define _GNU_SOURCE

static int _fish_ids = 0;


struct fish fish__create(enum fish_type type, int x, int y, char* id) {
    struct fish f;
    if(id == NULL)
        asprintf(f._id, "%s%d", get_type_string(type), _fish_ids++);
    else
        f._id = id;
    f._position = __position(x, y);
    f._type = type;

    return f;
}

int fish__free(any_t fish, any_t nothing){
    struct fish* f = fish;
    free(f);
    return MAP_OK;
}