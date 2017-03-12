#include <stdlib.h>
#include <stdio.h>
#include "aquarium.h"
#include "../utility/tools.h"

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

void fv__remove_vector(struct fish_vector *vector) {
    free(vector->_vector);
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

void aqv__remove_aquarium_view(struct aquarium_view *aqv) {
    fv__remove_vector(&aqv->_fishes);
}

void aq__initialize_aquarium(struct aquarium *aquarium, struct dimension dimension) {
    CHCK_NULL(aquarium, "aquarium")

    aquarium->_dimensions = dimension;
    aquarium->_views_qty = DEFAULT_VIEWS_QUANTITY;
    aquarium->_current_views = 0;
    aquarium->_views = malloc(sizeof(struct aquarium_view) * DEFAULT_VIEWS_QUANTITY);
    for (int i = 0; i < DEFAULT_VIEWS_QUANTITY; i++) {
        aquarium->_views[i]._view_id = -1;
    }
    fv__initialize_vector(&aquarium->_fishes, DEFAULT_FISHVECTOR_SIZE);
}

int aq__add_view(struct aquarium *aquarium, struct position s_pos, struct dimension dimensions) {
    for (int i = 0; i < aquarium->_views_qty; i++) {
        if (aquarium->_views[i]._view_id == -1) {
            aqv__initialize_aquarium_view(&aquarium->_views[aquarium->_current_views++], s_pos, dimensions);
            break;
        }
    }

    return _views_ids - 1;
}

struct aquarium_view *aq__get_view_by_id(struct aquarium *aquarium, int id) {
    for (int i = 0; i < aquarium->_current_views; i++) {
        if (aquarium->_views[i]._view_id == id)
            return &aquarium->_views[i];
    }

    return NULL;
}

int aq__get_view_position_by_id(struct aquarium *aquarium, int id) {
    for (int i = 0; i < aquarium->_current_views; i++) {
        if (aquarium->_views[i]._view_id == id)
            return i;
    }

    return -1;
}

void aq__add_fish(struct aquarium *aquarium, struct fish fish) {
    CHCK_NULL(aquarium, "aquarium")

    int positions[aquarium->_current_views];
    int founds = 0;
    //First we try to add the fish to views, if coordinates corresponds
    for (int i = 0; i < aquarium->_current_views; i++) {
        if (check_in_screen(aquarium->_views[i]._starting_position, aquarium->_views[i]._dimensions, fish._position)) {
            positions[founds++] = i;
        }
    }

    //Then we try to add it
    if (founds == 0) {
        //We couldn't find any matches, so it's that it isn't in any screen ... !
        fv__add_fish(&aquarium->_fishes, fish);
    } else {
        //We found it, so we add it to the views ... !
        for (int i = 0; i < founds; i++)
            fv__add_fish(&aquarium->_views[positions[i]]._fishes, fish);
    }
}

/*void aq__add_fish_to_view(struct aquarium *aquarium, int view_id, struct fish fish) {
    CHCK_NULL(aquarium, "aquarium")

    if (view_id == -1) {
        aq__add_fish(aquarium, fish);
    } else {
        aqv__add_fish(aq__get_view_by_id(aquarium, view_id), fish);
    }
}*/

void aq__remove_fish(struct aquarium *aquarium, int fish_id) {
    //first we seek for fishes not displayed
    for (int i = 0; i < aquarium->_fishes._current; i++) {
        if (aquarium->_fishes._vector[i]._id == fish_id) {
            fv__remove_fish(&aquarium->_fishes, fish_id);
            _console_log(LOG_MEDIUM, "Removed fish");
            return;
        }
    }

    //Then if we didn't find, we seek in views ...
    for (int i = 0; i < aquarium->_current_views; i++) {
        for (int j = 0; j < aquarium->_views[i]._fishes._current; j++) {
            if (aquarium->_views[i]._fishes._vector[j]._id == fish_id) {
                fv__remove_fish(&aquarium->_views[i]._fishes, fish_id);
                _console_log(LOG_MEDIUM, "Removed fish");
            }
        }
    }
}

void aq__remove_aquarium_view(struct aquarium *aquarium, int view_id) {
    int position = aq__get_view_position_by_id(aquarium, view_id);
    struct aquarium_view *view = &aquarium->_views[position];

    for (int i = 0; i < view->_fishes._current; ++i) {
        fv__add_fish(&aquarium->_fishes, view->_fishes._vector[i]);
    }

    aqv__remove_aquarium_view(view);

    //Here it will be considered as removed
    for (int i = position + 1; i < aquarium->_current_views; i++) {
        aquarium->_views[i - 1] = aquarium->_views[i];
    }

    //update current_views
    aquarium->_current_views--;
}

struct array aq__get_views_ids(struct aquarium *aquarium) {
    struct array array;
    array._length = aquarium->_current_views;
    int *views = malloc(sizeof(int) * aquarium->_current_views);
    for (int i = 0; i < aquarium->_current_views; i++) {
        views[i] = aquarium->_views[i]._view_id;
    }

    array._values = views;

    return array;
}

void aq__remove_aquarium(struct aquarium *aquarium) {
    fv__remove_vector(&aquarium->_fishes);
    for (int i = 0; i < aquarium->_current_views; i++) {
        aqv__remove_aquarium_view(&aquarium->_views[i]);
    }

    free(aquarium->_views);
}

///////DEBUG

void display_fish(struct fish *fish) {
    printf("FISH : %d \n\t type : %s \n\t coordinates : %d, %d\n", fish->_id, get_type_string(fish->_type),
           fish->_position.x, fish->_position.y);
}

void display_view(struct aquarium_view *aqv) {
    printf("VIEW : %d (%dx%d+%d+%d) \n\t FISHES : \n", aqv->_view_id, aqv->_starting_position.x,
           aqv->_starting_position.y, aqv->_dimensions.width, aqv->_dimensions.height);
    for (int i = 0; i < aqv->_fishes._current; i++) {
        display_fish(&aqv->_fishes._vector[i]);
    }
    printf("END OF VIEW \n");
}

void display_aquarium(struct aquarium *aq) {
    printf("~~~~~~ AQUARIUM ~~~~~~\n");
    printf("Dimensions : %dx%d\n", aq->_dimensions.width, aq->_dimensions.height);
    printf("~~~~~~~~~~~~~~~~~~~~~~\n");
    for (int i = 0; i < aq->_current_views; i++) {
        display_view(&aq->_views[i]);
    }

    printf("NOT DISPLAYED FISHES : \n");
    for (int i = 0; i < aq->_fishes._current; i++) {
        display_fish(&aq->_fishes._vector[i]);
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~\n");
}