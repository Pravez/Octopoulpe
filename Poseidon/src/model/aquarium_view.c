#include <stdio.h>
#include <stdlib.h>
#include "../utility/tools.h"
#include "aquarium_view.h"
#include "../utility/data.h"


static int _views_ids = 0;

int
aqv__initialize_aquarium_view(struct aquarium_view *aqv, struct position s_pos, struct dimension dimension, char *id) {
    CHCK_NULL_INT(aqv, "aquarium view")

    if (id == NULL)
        asprintf(&aqv->_id, "N%d", _views_ids++);
    else
        asprintf(&aqv->_id, "%s", id);

    aqv->_inner._starting_position = s_pos;
    aqv->_inner._dimensions = dimension;

    //Now we determine the outer bounds (including possible "toriq modulos")
    struct position outer_position = add_to_position_basic(aqv->_inner._starting_position,
                                                           -aqv->_inner._dimensions.width / 5,
                                                           -aqv->_inner._dimensions.height / 5);
    aqv->_outer._bounds_number = 0;

    if (outer_position.x < 0 && outer_position.y < 0) {
        aqv->_outer.bounds[aqv->_outer._bounds_number]._starting_position = add_to_position(
                aqv->_inner._starting_position, -aqv->_inner._dimensions.width / 5,
                -aqv->_inner._dimensions.height / 5);
        aqv->_outer.bounds[aqv->_outer._bounds_number]._dimensions = (struct dimension) {
                (int) (AQUARIUM_WIDTH - aqv->_outer.bounds[aqv->_outer._bounds_number]._starting_position.x),
                (int) (AQUARIUM_HEIGHT - aqv->_outer.bounds[aqv->_outer._bounds_number]._starting_position.y)};
        aqv->_outer._bounds_number++;
    }
    if(outer_position.x < 0){
        aqv->_outer.bounds[aqv->_outer._bounds_number]._starting_position = add_to_position(
                aqv->_inner._starting_position, -aqv->_inner._dimensions.width / 5, 0);
        aqv->_outer.bounds[aqv->_outer._bounds_number]._dimensions = (struct dimension) {
                (int) (AQUARIUM_WIDTH - aqv->_outer.bounds[aqv->_outer._bounds_number]._starting_position.x),
                aqv->_inner._dimensions.height + (aqv->_inner._dimensions.height / 5)};
        aqv->_outer._bounds_number++;
    }
    if(outer_position.y < 0){
        aqv->_outer.bounds[aqv->_outer._bounds_number]._starting_position = add_to_position(
                aqv->_inner._starting_position, 0, -aqv->_inner._dimensions.height / 5);
        aqv->_outer.bounds[aqv->_outer._bounds_number]._dimensions = (struct dimension) {
                aqv->_inner._dimensions.width + (aqv->_inner._dimensions.width / 5),
                (int) (AQUARIUM_HEIGHT - aqv->_outer.bounds[aqv->_outer._bounds_number]._starting_position.y)};
        aqv->_outer._bounds_number++;
    }

    aqv->_fishes = hashmap_new();

    return _views_ids - 1;
}

void aqv__add_fish(struct aquarium_view *aqv, struct fish *fish) {
    CHCK_NULL(aqv, "aquarium view")

    hashmap_put(aqv->_fishes, fish->_id, fish);
}

void aqv__remove_fish(struct aquarium_view *aqv, struct fish *fish) {
    CHCK_NULL(aqv, "aquarium view")

    hashmap_remove(aqv->_fishes, fish->_id);
}

int aqv__get_fish_qty(struct aquarium_view *aqv) {
    return hashmap_length(aqv->_fishes);
}

void aqv__remove_aquarium_view(struct aquarium_view *aqv) {
    free(aqv->_id);
    hashmap_free(aqv->_fishes);
}