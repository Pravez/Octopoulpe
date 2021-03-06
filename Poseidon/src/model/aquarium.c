#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aquarium.h"
#include "aquarium.h"
#include "../server/communication/answer.h"

#define DEFAULT_VIEWS_QUANTITY 5

void aq__initialize_aquarium(struct aquarium *aquarium, struct dimension dimension) {
    CHCK_NULL(aquarium, "aquarium")

    aquarium->_dimensions = dimension;
    v__init(&aquarium->_views, DEFAULT_VIEWS_QUANTITY);
    aquarium->_fishes = hashmap_new();
    asw__init_aquarium();
}

char *aq__add_view(struct aquarium *aquarium, struct position s_pos, struct dimension dimensions, char *id) {
    if (aq__check_free_id(aquarium, id)) {
        struct aquarium_view *view = malloc(sizeof(struct aquarium_view));
        aqv__initialize_aquarium_view(view, s_pos, dimensions, id);
        v__add(&aquarium->_views, view, AQ_VIEW);

        asw__add_view(view); // Update the controller

        return view->_id;
    } else {
        return NULL;
    }
}

struct aquarium_view *aq__get_view_by_id(struct aquarium *aquarium, char *id) {
    for (int i = 0; i < v__size(&aquarium->_views); i++) {
        struct aquarium_view *view = GET_VIEW_PTR(&aquarium->_views, i);
        if (view != NULL && !strcmp(view->_id, id))
            return view;
    }

    return NULL;
}

void aq__add_fish_to_aqv(struct aquarium *aq, char *id, struct fish *fish) {
    struct aquarium_view *aqv = aq__get_view_by_id(aq, id);
    CHK_ERROR(aqv, "Unable to add fish to NULL view")

    //First we add it to the whole aquarium, and after, to the little view
    hashmap_put(aq->_fishes, fish->_id, fish);
    aqv__add_fish(aqv, fish);
}

int aq__remove_fish(struct aquarium *aquarium, char *fish_id) {
    //We first remove it from the entire aquarium
    struct fish *fish = NULL;
    if (hashmap_get(aquarium->_fishes, fish_id, (any_t *) &fish) == MAP_OK) {
        hashmap_remove(aquarium->_fishes, fish->_id);
        CONSOLE_LOG_INFO("Removed fish %s", fish_id);
    }

    //If we found it it has a presence in at least one view
    if (fish != NULL) {
        for (int j = 0; j < v__size(&aquarium->_views); j++) {
            struct aquarium_view *view = GET_VIEW_PTR(&aquarium->_views, j);
            if (view != NULL) {
                if (hashmap_get(view->_fishes, fish_id, NULL) == MAP_OK) {
                    hashmap_remove(view->_fishes, fish_id);
                }
            }
        }

        free(fish->_id);
        free(fish);

        return 1;
    }

    return 0;
}

int aq__set_fish_running_state(struct aquarium *aquarium, char *fish_id, int state) {
    void *fish_ptr;
    if (hashmap_get(aquarium->_fishes, fish_id, &fish_ptr) == MAP_OK) {
        fish__set_running_state((struct fish *) fish_ptr, state);
    } else {
        return -1;
    }

    return 0;
}

int get_aquarium_view_position(struct aquarium *aquarium, char *view_id) {
    for (int i = 0; i < v__size(&aquarium->_views); i++) {
        if (!strcmp(GET_VIEW_PTR(&aquarium->_views, i)->_id, view_id)) {
            return i;
        }
    }

    return -1;
}

int removal_iterator(any_t aquarium_view, any_t fish) {
    struct aquarium_view *aqua = aquarium_view;
    char *f = ((struct fish *) fish)->_id;

    return hashmap_remove(aqua->_fishes, f);
}

void aq__remove_aquarium_view(struct aquarium *aquarium, char *view_id) {

    struct aquarium_view *view = aq__get_view_by_id(aquarium, view_id);

    hashmap_iterate(aquarium->_fishes, (PFany) removal_iterator, view);

    v__remove_by_data(&aquarium->_views, view);

    asw__remove_view(view_id);

    aqv__remove_aquarium_view(view);

    free(view);
}

struct array aq__get_views_ids(struct aquarium *aquarium) {
    struct array array;
    array._length = v__size(&aquarium->_views);
    char **views = malloc(sizeof(char *) * array._length);
    for (int i = 0; i < array._length; i++) {
        views[i] = GET_VIEW_PTR(&aquarium->_views, i)->_id;
    }

    array._values = views;

    return array;
}

void aq__remove_aquarium(struct aquarium *aquarium) {
    for (int i = 0; i < v__size(&aquarium->_views); i++) {
        aq__remove_aquarium_view(aquarium, GET_VIEW_PTR(&aquarium->_views, i)->_id);
    }

    hashmap_iterate(aquarium->_fishes, (PFany) fish__free, NULL);
    hashmap_free(aquarium->_fishes);
    v__destroy(&aquarium->_views);

    asw__remove_aquarium();
}

int aq__check_free_id(struct aquarium *aquarium, char *id) {
    if (id == NULL)
        return 0;

    for (int i = 0; i < v__size(&aquarium->_views); i++) {
        if (!strcmp(GET_VIEW_PTR(&aquarium->_views, i)->_id, id)) {
            return 0;
        }
    }

    return 1;
}

struct relative_position aq__get_relative_pos(struct aquarium_view *aqv, struct fish *fish) {
    /*struct aquarium_view* aqv = NULL;
    for (int i = 0; i < v__size(&aquarium->_views) && aqv == NULL; i++) {
        if (hashmap_get(GET_VIEW_PTR(&aquarium->_views, i)->_fishes, fish->_id, NULL) == MAP_OK) {
            aqv = GET_VIEW_PTR(&aquarium->_views, i);
        }
    }*/

    if (aqv == NULL || hashmap_get(aqv->_fishes, fish->_id, NULL) != MAP_OK)
        return (struct relative_position) {-1, -1};

    int relative_x =
            (100 * ((int) fish->_current.x - (int) aqv->_inner._starting_position.x)) / (aqv->_inner._dimensions.width);
    int relative_y = (100 * ((int) fish->_current.y - (int) aqv->_inner._starting_position.y)) /
                     (aqv->_inner._dimensions.height);


    return (struct relative_position) {relative_x, relative_y};
}

///////DEBUG

void display_fish(struct fish *fish) {
    printf("FISH : %s \n\t type : %s \n\t coordinates : %d, %d\n", fish->_id, get_type_string(fish->_type),
           (int) fish->_current.x, (int) fish->_current.y);
}

int iterate_fishes(any_t nothing, any_t fish) {
    display_fish(fish);
    return MAP_OK;
}

void display_view(struct aquarium_view *aqv) {
    printf("VIEW : %s (%dx%d+%d+%d) \n\t FISHES : \n", aqv->_id, (int) aqv->_inner._starting_position.x,
           (int) aqv->_inner._starting_position.y, aqv->_inner._dimensions.width, aqv->_inner._dimensions.height);
    hashmap_iterate(aqv->_fishes, (PFany) iterate_fishes, NULL);
    printf("END OF VIEW \n");
}

void display_aquarium(struct aquarium *aq) {
    printf("~~~~~~ AQUARIUM ~~~~~~\n");
    printf("Dimensions : %dx%d\n", aq->_dimensions.width, aq->_dimensions.height);
    printf("~~~~~~~~~~~~~~~~~~~~~~\n");
    for (int i = 0; i < v__size(&aq->_views); i++) {
        display_view(GET_VIEW_PTR(&aq->_views, i));
    }

    printf("TOTAL OF FISHES : \n");
    hashmap_iterate(aq->_fishes, (PFany) iterate_fishes, NULL);
    printf("~~~~~~~~~~~~~~~~~~~~~~\n");
}
