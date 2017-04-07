#ifndef POSEIDON_FISH_H
#define POSEIDON_FISH_H

#include <stdlib.h>
#include "../utility/tools.h"
#include "../utility/vector.h"
#include "../utility/fish_type.h"

struct fish_vector {
    size_t _size;
    int _current;
    struct fish *_vector;
};

struct fish {
    enum fish_type _type;
    struct position _position;
    char *_id;
};

struct fish fish__create(enum fish_type type, int x, int y, char* id);

void fv__initialize_vector(struct fish_vector *vector, size_t size);
void fv__add_fish(struct fish_vector *vector, struct fish fish);
void fv__remove_fish(struct fish_vector *vector, char *id);
void fv__resize_vector(struct fish_vector *vector);
struct fish *fv__get_fish_by_id(struct fish_vector *vector, char *id);
struct fish *fv__get_fishes_by_type(struct fish_vector *vector, enum fish_type type);
void fv__remove_vector(struct fish_vector *vector);


#endif //POSEIDON_FISH_H
