#ifndef POSEIDON_AQUARIUM_VIEW_H
#define POSEIDON_AQUARIUM_VIEW_H

#include "../utility/tools.h"
#include "fish.h"

#define DEF_FISHES_SIZE 50

struct aquarium_view {
    struct bounds _outer;
    struct bounds _inner;
    char *_id;
    struct fish_vector _fishes;
};

int aqv__initialize_aquarium_view(struct aquarium_view* aqv, struct position s_pos, struct dimension dimension, char* id);
void aqv__add_fish(struct aquarium_view* aqv, struct fish fish);
int aqv__get_fish_qty(struct aquarium_view* aqv);
void aqv__remove_aquarium_view(struct aquarium_view* aqv);

#endif //POSEIDON_AQUARIUM_VIEW_H
