#include <stdio.h>
#include <stdlib.h>
#include "../utility/tools.h"
#include "aquarium_view.h"


static int _views_ids = 0;

int aqv__initialize_aquarium_view(struct aquarium_view *aqv, struct position s_pos, struct dimension dimension, char* id) {
    CHCK_NULL_INT(aqv, "aquarium view")

    if(id == NULL)
        asprintf(&aqv->_id, "N%d", _views_ids++);
    else
        asprintf(&aqv->_id, "%s", id);

    aqv->_inner._starting_position = s_pos;
    aqv->_inner._dimensions = dimension;
    aqv->_fishes = hashmap_new();

    return _views_ids-1;
}

void aqv__add_fish(struct aquarium_view *aqv, struct fish* fish) {
    CHCK_NULL(aqv, "aquarium view")

    hashmap_put(aqv->_fishes, fish->_id, fish);
}

int aqv__get_fish_qty(struct aquarium_view *aqv) {
    return hashmap_length(aqv->_fishes);
}

void aqv__remove_aquarium_view(struct aquarium_view *aqv) {
    hashmap_free(aqv->_fishes);
}