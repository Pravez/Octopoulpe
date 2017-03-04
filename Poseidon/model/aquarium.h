#ifndef POSEIDON_AQUARIUM_H
#define POSEIDON_AQUARIUM_H

#include "../utility/tools.h"
#include "../utility/fish_type.h"

#define DEFAULT_FISHVECTOR_SIZE 10
#define DEFAULT_VIEWS_QUANTITY 5

/**
 * Simple fish structure. A fish is identified by an ID, has a type and a position.
 */
struct fish{
    int _id;
    enum fish_type _type;
    struct position _position;
};

/**
 * A fish vector aims to regulate how are managed fishes in a view (easily to add/delete operations).
 */
struct fish_vector{
    size_t _size;
    int _current;
    struct fish* _vector;
};

/**
 * An aquarium view is the partial view of an aquarium, representing from the starting point with the
 * dimensions a rectangle qualifying the visible fishes into it.
 */
struct aquarium_view{
    int _view_id;
    struct position _starting_position;
    struct dimension _dimensions;
    struct fish_vector _fishes;
};

/**
 * The main aquarium, containing view, and fishes...
 * /!\ CARE, the _fishes are only fishes wich are not contained in views (maybe rework this ?)
 */
struct aquarium{
    struct dimension _dimensions;
    size_t _views_qty;
    int _current_views;
    struct aquarium_view* _views;
    struct fish_vector _fishes;
};

/**
 * Basic function to create a fish, returns an entire structure of the created fish
 * @param type
 * @param x
 * @param y
 * @return
 */
struct fish fish__create(enum fish_type type, int x, int y);

/////////////////////////////////////////////////
/////           FISH VECTOR                 /////
/////////////////////////////////////////////////

/**
 * Basic function to initialize a vector, with a defined size
 * @param vector
 * @param size
 */
void fv__initialize_vector(struct fish_vector* vector, size_t size);

/**
 * To add a fish, if size is too small, does a realloc
 * @param vector
 * @param fish
 */
void fv__add_fish(struct fish_vector* vector, struct fish fish);

/**
 * To remove a fish, replaces the fish with the last added fish
 * @param vector
 * @param id
 */
void fv__remove_fish(struct fish_vector* vector, int id);

/**
 * Function to resize a vector
 * @param vector
 */
void fv__resize_vector(struct fish_vector* vector);

/**
 * Basic function to get the reference to a fish by its ID
 * @param vector
 * @param id
 * @return
 */
struct fish* fv__get_fish_by_id(struct fish_vector* vector, int id);

/**
 * List all fishes from a vector, according to their type (this functions does not this
 * - just returns the first fish found with the type - stupid)
 * @param vector
 * @param type
 * @return
 */
struct fish* fv__get_fishes_by_type(struct fish_vector* vector, enum fish_type type);

/////////////////////////////////////////////////
/////          AQUARIUM VIEW                /////
/////////////////////////////////////////////////

/**
 * To initialize a view. The "screen" start on a pos (s_pos), and has a certain dimension
 * @param aqv
 * @param s_pos
 * @param dimension
 * @return
 */
 //TODO verify if dimensions do not overlapse aquarium
int aqv__initialize_aquarium_view(struct aquarium_view* aqv, struct position s_pos, struct dimension dimension);

/**
 * To add a fish to a view
 * @param aqv
 * @param fish
 */
void aqv__add_fish(struct aquarium_view* aqv, struct fish fish);

/**
 * To get the quantity of fishes contained in a certain view
 * @param aqv
 * @return
 */
int aqv__get_fish_qty(struct aquarium_view* aqv);

/////////////////////////////////////////////////
/////             AQUARIUM                  /////
/////////////////////////////////////////////////

/**
 * To initialize an aquarium, based on a dimension.
 * @param aquarium
 * @param dimension
 */
void aq__initialize_aquarium(struct aquarium* aquarium, struct dimension dimension);

/**
 * To add a view to an aquarium. Return the ID of the added view. (view is constructed in the adding)
 * @param aquarium
 * @param s_pos
 * @param dimensions
 * @return
 */
int aq__add_view(struct aquarium* aquarium, struct position s_pos, struct dimension dimensions);

/**
 *  To get a view according to its ID
 * @param aquarium
 * @param id
 * @return
 */
struct aquarium_view* aq__get_view_by_id(struct aquarium* aquarium, int id);

/**
 * To add a fish to the aquarium, but in the not-displayed fish context
 * @param aquarium
 * @param fish
 */
void aq__add_fish(struct aquarium* aquarium, struct fish fish);

/**
 * To add a fish to the aquarium, but in a view (so, ideally it is supposed to be displayed)
 * @param aquarium
 * @param view_id
 * @param fish
 */
void aq__add_fish_to_view(struct aquarium* aquarium, int view_id, struct fish fish);

/**
 * To remove a fish from the aquarium, if it can't find it in the "not displayed" ones,
 * it visits all the views and tries to remove it.
 * @param aquarium
 * @param fish_id
 */
void aq__remove_fish(struct aquarium* aquarium, int fish_id);

//For debug purposes
void display_fish(struct fish* fish);
void display_view(struct aquarium_view* aqv);
void display_aquarium(struct aquarium* aq);

#endif //POSEIDON_AQUARIUM_H
