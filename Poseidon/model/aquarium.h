#ifndef POSEIDON_AQUARIUM_REWORK_H
#define POSEIDON_AQUARIUM_REWORK_H

#include <stdlib.h>
#include "../utility/tools.h"
#include "../utility/vector.h"
#include "../utility/fish_type.h"
#include "aquarium_view.h"

struct aquarium{
    struct dimension _dimensions;
    struct vector _views;
    map_t _fishes;
};

void aq__initialize_aquarium(struct aquarium* aquarium, struct dimension dimension);
void aq__remove_aquarium(struct aquarium* aquarium);

char* aq__add_view(struct aquarium* aquarium, struct position s_pos, struct dimension dimensions, char* id);
struct aquarium_view* aq__get_view_by_id(struct aquarium* aquarium, char* id);
void aq__remove_aquarium_view(struct aquarium* aquarium, char* view_id);

//void aq__add_fish(struct aquarium* aquarium, struct fish fish);
void aq__add_fish_to_aqv(struct aquarium* aq, char* id, struct fish* fish);
void aq__remove_fish(struct aquarium *aquarium, char* fish_id);
struct array aq__get_views_ids(struct aquarium* aquarium);

int aq__check_free_id(struct aquarium* aquarium, char* id);

int aq__set_fish_running_state(struct aquarium* aquarium, char* fish_id, int state);
struct relative_position aq__get_relative_pos(struct aquarium* aquarium, struct fish* fish);

//For debug purposes
void display_fish(struct fish* fish);
void display_view(struct aquarium_view* aqv);
void display_aquarium(struct aquarium* aq);


#endif //POSEIDON_AQUARIUM_REWORK_H
