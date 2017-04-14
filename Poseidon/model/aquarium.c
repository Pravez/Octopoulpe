#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aquarium.h"
#include "aquarium.h"

#define _GNU_SOURCE

#define DEFAULT_VIEWS_QUANTITY 5

#define GET_VIEW_PTR(vector, position) vi__convert_aq_view(v__get(vector, position))

void aq__initialize_aquarium(struct aquarium *aquarium, struct dimension dimension) {
    CHCK_NULL(aquarium, "aquarium")

    aquarium->_dimensions = dimension;
    v__init(&aquarium->_views, DEFAULT_VIEWS_QUANTITY);
    aquarium->_fishes = hashmap_new();
}

char* aq__add_view(struct aquarium *aquarium, struct position s_pos, struct dimension dimensions, char* id) {
    if(aq__check_free_id(aquarium, id)) {
        struct aquarium_view *view = malloc(sizeof(struct aquarium_view));
        aqv__initialize_aquarium_view(view, s_pos, dimensions, id);
        v__add(&aquarium->_views, view, AQ_VIEW);

        return view->_id;
    }else{
        return NULL;
    }
}

struct aquarium_view *aq__get_view_by_id(struct aquarium *aquarium, char* id) {
    for(int i = 0;i < v__size(&aquarium->_views);i++){
        struct aquarium_view* view = GET_VIEW_PTR(&aquarium->_views, i);
        if(view != NULL && !strcmp(view->_id, id))
            return view;
    }

    return NULL;
}

void aq__add_fish_to_aqv(struct aquarium* aq, char* id, struct fish* fish){
    struct aquarium_view* aqv = aq__get_view_by_id(aq, id);
    CHK_ERROR(aqv, "Unable to add fish to NULL view")

    aqv__add_fish(aqv, fish);
}

void aq__remove_fish(struct aquarium *aquarium, char* fish_id) {
    //first we seek for fishes not displayed
    void* found_fish = malloc(sizeof(struct fish**));
    if(hashmap_get(aquarium->_fishes, fish_id, found_fish) == MAP_OK){
        hashmap_remove(aquarium->_fishes, (*(struct fish**) found_fish)->_id);
        _console_log(LOG_MEDIUM, "Removed fish");
    }

    //Then if we didn't find, we seek in views ...
    for (int j = 0; j < v__size(&aquarium->_views); j++) {
        struct aquarium_view* view = GET_VIEW_PTR(&aquarium->_views, j);
        if(view != NULL){
            if(hashmap_get(view->_fishes, fish_id, found_fish) == MAP_OK){
                hashmap_remove(view->_fishes, (*(struct fish**) found_fish)->_id);
                _console_log(LOG_MEDIUM, "Removed fish");
            }
        }
    }

    if(found_fish != NULL){
        free(*(struct fish**) found_fish);
        free(found_fish);
    }
}

int get_aquarium_view_position(struct aquarium* aquarium, char* view_id){
    for(int i = 0;i < v__size(&aquarium->_views);i++){
        if(!strcmp(GET_VIEW_PTR(&aquarium->_views, i)->_id, view_id)){
            return i;
        }
    }

    return -1;
}

int removal_iterator(any_t aquarium,any_t fish){
    struct aquarium* aqua = aquarium;
    struct fish* f = fish;
    if(aquarium == NULL)
        return MAP_MISSING;

    return hashmap_put(aqua->_fishes, f->_id, f);
}

void aq__remove_aquarium_view(struct aquarium *aquarium, char* view_id) {

    struct aquarium_view *view = aq__get_view_by_id(aquarium, view_id);

    hashmap_iterate(aquarium->_fishes, (PFany) removal_iterator, aquarium);

    v__remove_by_data(&aquarium->_views, view);
    aqv__remove_aquarium_view(view);

    free(view);

}

struct array aq__get_views_ids(struct aquarium *aquarium) {
    struct array array;
    array._length = v__size(&aquarium->_views);
    char **views = malloc(sizeof(char*) * array._length);
    for (int i = 0; i < array._length; i++) {
        views[i] = GET_VIEW_PTR(&aquarium->_views, i)->_id;
    }

    array._values = views;

    return array;
}

void aq__remove_aquarium(struct aquarium *aquarium) {
    hashmap_iterate(aquarium->_fishes, (PFany) fish__free, NULL);
    hashmap_free(aquarium->_fishes);

    for (int i = 0; i < v__size(&aquarium->_views); i++) {
        aqv__remove_aquarium_view(GET_VIEW_PTR(&aquarium->_views, i));
    }

    v__destroy(&aquarium->_views);
}

char* aq__check_free_id(struct aquarium* aquarium, char* id){
    if(id == NULL)
        return "OK";

    for(int i = 0;i < v__size(&aquarium->_views);i++){
        if(!strcmp(GET_VIEW_PTR(&aquarium->_views, i)->_id, id)){
            return NULL;
        }
    }

    return id;
}

///////DEBUG

void display_fish(struct fish *fish) {
    printf("FISH : %s \n\t type : %s \n\t coordinates : %d, %d\n", fish->_id, get_type_string(fish->_type),
           fish->_position.x, fish->_position.y);
}

int iterate_fishes(any_t nothing, any_t fish){
    display_fish(fish);
    return MAP_OK;
}

void display_view(struct aquarium_view *aqv) {
    printf("VIEW : %s (%dx%d+%d+%d) \n\t FISHES : \n", aqv->_id, aqv->_inner._starting_position.x,
           aqv->_inner._starting_position.y, aqv->_inner._dimensions.width, aqv->_inner._dimensions.height);
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

    printf("NOT DISPLAYED FISHES : \n");
    hashmap_iterate(aq->_fishes, (PFany) iterate_fishes, NULL);
    printf("~~~~~~~~~~~~~~~~~~~~~~\n");
}