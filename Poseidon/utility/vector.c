#include <stdlib.h>
#include "vector.h"

void v__init(struct vector *vector, size_t size) {
    vector = malloc(sizeof(struct vector));
    vector->_current_size = 0;
    vector->_size = size;
    vector->_items = malloc(sizeof(struct vector_item) * size);
}

void v__destroy(struct vector *vector) {
    free(vector->_items);
    free(vector);
}

void _v__resize(struct vector *vector){
    vector->_size *= 2;
    vector->_items = realloc(vector->_items, vector->_size);
}

int v__add(struct vector *vector, void *data, enum TTYPE type) {
    struct vector_item vi = vi__create(type, data);
    if((vector->_current_size+1) == vector->_size)
        _v__resize(vector);

    vector->_items[vector->_current_size++] = vi;

    return vector->_current_size-1;
}

int v__remove_by_data(struct vector *vector, void *data, enum TTYPE type) {
    return 0;
}

int v__remove_by_index(struct vector *vector, int index) {
    for (int i = index; i < vector->_current_size - 1; i++) {
        vector->_items[i] = vector->_items[i+1];
    }

    return vector->_current_size--;
}

struct vector_item v__get(struct vector *vector, int index) {
    return vector->_items[index];
}

struct vector_item vi__create(enum TTYPE type, void *data) {
    return (struct vector_item) {type, data};
}

int vi__convert_int(struct vector_item* item){
    if(item->_type == INT)
        return (int)item->_data;
    return INV_INT;
}

struct aquarium_view* vi__convert_aq_view(struct vector_item* item){
    if(item->_type == AQ_VIEW)
        return (struct aquarium_view*)item->_data;

    return NULL;
}

struct fish* vi__convert_fish(struct vector_item* item){
    if(item->_type == FISH)
        return (struct fish*)item->_data;

    return NULL;
}