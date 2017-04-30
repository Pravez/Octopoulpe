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
    aqv->_outer._starting_position = add_to_position(aqv->_inner._starting_position, -aqv->_inner._dimensions.width / 5,
                                                     -aqv->_inner._dimensions.height / 5);
    aqv->_outer._dimensions = (struct dimension) {dimension.width + (aqv->_inner._dimensions.width / 5) * 2,
                                                  dimension.height + (aqv->_inner._dimensions.height / 5) * 2};
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