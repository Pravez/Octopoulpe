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

void fv__initialize_vector(struct fish_vector *vector, size_t size) {
    CHCK_NULL(vector, "fish vector");

    vector->_size = size;
    vector->_current = 0;
    vector->_vector = malloc(sizeof(struct fish *) * size);
}

void fv__add_fish(struct fish_vector *vector, struct fish fish) {
    CHCK_NULL(vector, "fish vector");

    if (vector->_current == vector->_size)
        fv__resize_vector(vector);

    vector->_vector[vector->_current++] = fish;
}

void fv__remove_fish(struct fish_vector *vector, char* id) {
    CHCK_NULL(vector, "fish vector")

    int position = -1;
    //Seeking for the fish
    for (int i = 0; i < vector->_current && position == -1; i++) {
        if (!strcmp(vector->_vector[i]._id, id))
            position = i;
    }

    if (position != -1) {
        //Then we replace the old one with the last we added
        vector->_vector[position] = vector->_vector[--vector->_current];
    } else {
        _console_log(LOG_HIGH, "Couldn't find fish asked");
    }
}

void fv__resize_vector(struct fish_vector *vector) {
    CHCK_NULL(vector, "fish vector")
    CHCK_NULL(vector->_vector, "fishes in vector of fish")

    vector->_size *= 2;
    vector->_vector = realloc(vector->_vector, vector->_size);
}

struct fish *fv__get_fish_by_id(struct fish_vector *vector, char* id) {
    for (int i = 0; i < vector->_current; i++) {
        if (!strcmp(vector->_vector[i]._id, id))
            return &vector->_vector[i];
    }

    return NULL;
}

struct fish *fv__get_fishes_by_type(struct fish_vector *vector, enum fish_type type) {
    for (int i = 0; i < vector->_current; i++) {
        if (vector->_vector[i]._type == type)
            return &vector->_vector[i];
    }

    return NULL;
}

void fv__remove_vector(struct fish_vector *vector) {
    free(vector->_vector);
}