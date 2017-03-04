#ifndef POSEIDON_AQUARIUM_H
#define POSEIDON_AQUARIUM_H

#include "../utility/tools.h"
#include "../utility/fish_type.h"

#define DEFAULT_FISHVECTOR_SIZE 10
#define DEFAULT_VIEWS_QUANTITY 5

struct fish{
    int _id;
    enum fish_type _type;
    struct position _position;
};

struct fish_vector{
    size_t _size;
    int _current;
    struct fish* _vector;
};

struct aquarium_view{
    int _view_id;
    struct position _starting_position;
    struct dimension _dimensions;
    struct fish_vector _fishes;
};

struct aquarium{
    struct dimension _dimensions;
    size_t _views_qty;
    int _current;
    struct aquarium_view* _views;
};

struct fish fish__create(enum fish_type type, int x, int y);

void fv__initialize_vector(struct fish_vector* vector, size_t size);
void fv__add_fish(struct fish_vector* vector, struct fish fish);
void fv__remove_fish(struct fish_vector* vector, int id);
void fv__resize_vector(struct fish_vector* vector);
struct fish* fv__get_fish_by_id(struct fish_vector* vector, int id);
struct fish* fv__get_fishes_by_type(struct fish_vector* vector, enum fish_type type);

int aqv__initialize_aquarium_view(struct aquarium_view* aqv, struct position s_pos, struct dimension dimension);
void aqv__add_fish(struct aquarium_view* aqv, struct fish fish);
int aqv__get_fish_qty(struct aquarium_view* aqv);

void aq__initialize_aquarium(struct aquarium* aquarium, struct dimension dimension);
int aq__add_view(struct aquarium* aquarium, struct position s_pos, struct dimension dimensions);
struct aquarium_view* aq__get_view_by_id(struct aquarium* aquarium, int id);


//For debug purposes
void display_fish(struct fish* fish);
void display_view(struct aquarium_view* aqv);

#endif //POSEIDON_AQUARIUM_H
