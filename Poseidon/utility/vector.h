#ifndef POSEIDON_VECTOR_H
#define POSEIDON_VECTOR_H

#define INV_INT 32765

#define GET_VIEW_PTR(vector, position) vi__convert_aq_view(v__get(vector, position))

struct aquarium_view;
struct fish;

enum TTYPE {
    INT, AQ_VIEW, FISH
};

struct vector_item {
    enum TTYPE _type;
    void *_data;
};

struct vector {
    size_t _size;
    int _current_size;
    struct vector_item *_items;
};

void v__init(struct vector *vector, size_t size);

void v__destroy(struct vector *vector);

int v__add(struct vector *vector, void *data, enum TTYPE type);

int v__remove_by_data(struct vector *vector, void *data);

int v__remove_by_index(struct vector *vector, int index);

int v__size(struct vector *vector);


struct vector_item* v__get(struct vector *vector, int index);

struct vector_item vi__create(enum TTYPE type, void *data);

int vi__convert_int(struct vector_item *item);

struct aquarium_view *vi__convert_aq_view(struct vector_item *item);

struct fish *vi__convert_fish(struct vector_item *item);

#endif //POSEIDON_VECTOR_H
