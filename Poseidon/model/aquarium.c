#include <stdlib.h>
#include <stdio.h>
#include "aquarium.h"

static int _fish_ids = 0;
static int _views_ids = 0;

#define CHCK_NULL(x, name) if(x == NULL){ fprintf(stderr, "FUNC %s : A VALUE IS NULL : %s\n", __func__, name); return; }
#define CHCK_NULL_INT(x, name) if(x == NULL){ fprintf(stderr, "FUNC %s : A VALUE IS NULL : %s\n", __func__, name); return -1; }

struct fish fish__create(enum fish_type type, int x, int y) {
    struct fish f;
    f._id = _fish_ids++;
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

void fv__remove_fish(struct fish_vector *vector, int id) {
    CHCK_NULL(vector, "fish vector")

    int position = -1;
    //Seeking for the fish
    for (int i = 0; i < vector->_current && position == -1; i++) {
        if (vector->_vector[i]._id == id)
            position = i;
    }

    if (position != -1) {
        //Then we replace the old one with the last we added
        vector->_vector[position] = vector->_vector[vector->_current--];
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

struct fish *fv__get_fish_by_id(struct fish_vector *vector, int id) {
    for (int i = 0; i < vector->_current; i++) {
        if (vector->_vector[i]._id == id)
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

int aqv__initialize_aquarium_view(struct aquarium_view *aqv, struct position s_pos, struct dimension dimension) {
    CHCK_NULL_INT(aqv, "aquarium view")

    aqv->_view_id = _views_ids++;
    aqv->_starting_position = s_pos;
    aqv->_dimensions = dimension;
    fv__initialize_vector(&aqv->_fishes, DEFAULT_FISHVECTOR_SIZE);

    return aqv->_view_id;
}

void aqv__add_fish(struct aquarium_view *aqv, struct fish fish) {
    CHCK_NULL(aqv, "aquarium view")

    fv__add_fish(&aqv->_fishes, fish);
}

int aqv__get_fish_qty(struct aquarium_view *aqv) {
    return aqv->_fishes._current;
}

void aq__initialize_aquarium(struct aquarium *aquarium, struct dimension dimension) {
    CHCK_NULL(aquarium, "aquarium")

    aquarium->_dimensions = dimension;
    aquarium->_views_qty = DEFAULT_VIEWS_QUANTITY;
    aquarium->_current = 0;
    aquarium->_views = malloc(sizeof(struct aquarium_view) * DEFAULT_VIEWS_QUANTITY);
}

int aq__add_view(struct aquarium *aquarium, struct position s_pos, struct dimension dimensions) {
    aqv__initialize_aquarium_view(&aquarium->_views[aquarium->_current++], s_pos, dimensions);
    return _views_ids - 1;
}

struct aquarium_view *aq__get_view_by_id(struct aquarium *aquarium, int id) {
    for (int i = 0; i < aquarium->_current; i++) {
        if (aquarium->_views[i]._view_id == id)
            return &aquarium->_views[i];
    }

    return NULL;
}





///////DEBUG

void display_fish(struct fish *fish) {
    printf("FISH : %d \n\t type : %s \n\t coordinates : %d, %d\n", fish->_id, get_type_string(fish->_type),
           fish->_position.x, fish->_position.y);
}

void display_view(struct aquarium_view *aqv){
    printf("VIEW : %d \n\t FISHES : \n", aqv->_view_id);
    for(int i = 0;i < aqv->_fishes._current;i++){
        display_fish(&aqv->_fishes._vector[i]);
    }
    printf("END OF VIEW \n");
}