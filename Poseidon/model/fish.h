#ifndef POSEIDON_FISH_H
#define POSEIDON_FISH_H

#include <stdlib.h>
#include "../utility/tools.h"
#include "../utility/vector.h"
#include "../utility/fish_type.h"
#include "../utility/hashmap.h"

struct fish {
    enum fish_type _type;
    struct position _position;
    struct position _cover;
    char *_id;
};

struct fish* fish__create(enum fish_type type, int x, int y, char* id, int width, int height);
int fish__free(any_t fish, any_t nothing);

#endif //POSEIDON_FISH_H
