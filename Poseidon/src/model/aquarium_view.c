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
    aqv->_inner._dimensions = (struct dimension) {
            (int) (s_pos.x + dimension.width > AQUARIUM_WIDTH ? AQUARIUM_WIDTH - s_pos.x : dimension.width),
            (int) (s_pos.y + dimension.height > AQUARIUM_HEIGHT ? AQUARIUM_HEIGHT - s_pos.y : dimension.height)};


    aqv->_outer = aqv__determine_outer_bounds(aqv);


    aqv->_fishes = hashmap_new();

    return _views_ids - 1;
}

struct bounds aqv__determine_outer_bounds(struct aquarium_view *aqv) {
    //Now we determine the outer bounds (including possible "toriq modulos")
    struct bounds outer;
    int relative_adding_width = aqv->_inner._dimensions.width / 5;
    int relative_adding_height = aqv->_inner._dimensions.height / 5;
    outer._starting_position = add_to_position_basic(aqv->_inner._starting_position,
                                                     -relative_adding_width,
                                                     -relative_adding_height);

    if (outer._starting_position.x < 0) {
        outer._starting_position.x = aqv->_inner._starting_position.x;
    }
    if (outer._starting_position.y < 0) {
        outer._starting_position.y = aqv->_inner._starting_position.y;
    }
    if (outer._starting_position.x + relative_adding_width + aqv->_inner._dimensions.width > AQUARIUM_WIDTH) {
        outer._dimensions.width = AQUARIUM_WIDTH - aqv->_inner._dimensions.width;
    } else {
        outer._dimensions.width = aqv->_inner._dimensions.width + relative_adding_width * 2;
    }
    if (outer._starting_position.y + relative_adding_height + aqv->_inner._dimensions.height > AQUARIUM_HEIGHT) {
        outer._dimensions.height = AQUARIUM_HEIGHT - aqv->_inner._dimensions.height;
    } else {
        outer._dimensions.height = aqv->_inner._dimensions.height + relative_adding_height * 2;
    }

    return outer;
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