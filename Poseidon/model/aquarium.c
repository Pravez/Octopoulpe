#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aquarium.h"
#include "aquarium.h"

#define _GNU_SOURCE

#define DEFAULT_FISHVECTOR_SIZE 10
#define DEFAULT_VIEWS_QUANTITY 5

#define GET_VIEW_PTR(vector, position) vi__convert_aq_view(v__get(vector, position))

void aq__initialize_aquarium(struct aquarium *aquarium, struct dimension dimension) {
    CHCK_NULL(aquarium, "aquarium")

    aquarium->_dimensions = dimension;
    v__init(&aquarium->_views, DEFAULT_VIEWS_QUANTITY);
    fv__initialize_vector(&aquarium->_fishes, DEFAULT_FISHVECTOR_SIZE);
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

void aq__add_fish_to_aqv(struct aquarium* aq, char* id, struct fish fish){
    struct aquarium_view* aqv = aq__get_view_by_id(aq, id);
    CHK_ERROR(aqv, "Unable to add fish to NULL view")

    aqv__add_fish(aqv, fish);
}

/*void aq__add_fish(struct aquarium *aquarium, struct fish fish) {
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
}*/

void aq__remove_fish(struct aquarium *aquarium, char* fish_id) {
    //first we seek for fishes not displayed
    for (int i = 0; i < aquarium->_fishes._current; i++) {
        if (!strcmp(aquarium->_fishes._vector[i]._id, fish_id)) {
            fv__remove_fish(&aquarium->_fishes, fish_id);
            _console_log(LOG_MEDIUM, "Removed fish");
            return;
        }
    }

    //Then if we didn't find, we seek in views ...
    for (int j = 0; j < v__size(&aquarium->_views); j++) {
        struct aquarium_view* view = GET_VIEW_PTR(&aquarium->_views, j);
        if(view != NULL){
            for(int i = 0; i < view->_fishes._current;i++){
                if(!strcmp(view->_fishes._vector[i]._id, fish_id)){
                    fv__remove_fish(&view->_fishes, fish_id);
                    _console_log(LOG_MEDIUM, "Removed fish");
                }
            }
        }
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

void aq__remove_aquarium_view(struct aquarium *aquarium, char* view_id) {

    struct aquarium_view *view = aq__get_view_by_id(aquarium, view_id);

    for (int i = 0; i < view->_fishes._current; ++i) {
        fv__add_fish(&aquarium->_fishes, view->_fishes._vector[i]);
    }

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
    fv__remove_vector(&aquarium->_fishes);

    for (int i = 0; i < v__size(&aquarium->_views); i++) {
        aqv__remove_aquarium_view(GET_VIEW_PTR(&aquarium->_views, i));
    }

    v__destroy(&aquarium->_views);
}

int aq__check_free_id(struct aquarium* aquarium, char* id){
    if(id == NULL)
        return 0;

    for(int i = 0;i < v__size(&aquarium->_views);i++){
        if(!strcmp(GET_VIEW_PTR(&aquarium->_views, i)->_id, id)){
            return 0;
        }
    }

    return 1;
}

///////DEBUG

void display_fish(struct fish *fish) {
    printf("FISH : %s \n\t type : %s \n\t coordinates : %d, %d\n", fish->_id, get_type_string(fish->_type),
           fish->_position.x, fish->_position.y);
}

void display_view(struct aquarium_view *aqv) {
    printf("VIEW : %s (%dx%d+%d+%d) \n\t FISHES : \n", aqv->_id, aqv->_inner._starting_position.x,
           aqv->_inner._starting_position.y, aqv->_inner._dimensions.width, aqv->_inner._dimensions.height);
    for (int i = 0; i < aqv->_fishes._current; i++) {
        display_fish(&aqv->_fishes._vector[i]);
    }
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
    for (int i = 0; i < aq->_fishes._current; i++) {
        display_fish(&aq->_fishes._vector[i]);
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~\n");
}
