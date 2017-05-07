#ifndef POSEIDON_WORLD_H
#define POSEIDON_WORLD_H

#include "../../model/fish.h"
#include "../../utility/tools.h"

void * world_process(void *pVoid);
int world_init();
int world_loop();
/**
 * Main loop called at each world frame updating every single fish
 * @param nothing
 * @param item
 * @return
 */
int update_fishes(any_t nothing, any_t item);
/**
 * Called at each world frame and updating views content (fishes)
 * @param aqv
 * @param fish
 */
void update_view_content(struct aquarium_view* aqv, struct fish* fish);
/**
 * Intermediate function to call every action to update the world
 */
void update();
/**
 * Function to notify each X seconds every observer (thread receiving continuously fishes)
 */
void notify_observers();
/**
 * Function to find the next movement for a fish (randomly found)
 * @param fish
 * @return
 */
struct movement next_movement(struct fish* fish);
/**
 * Function to determine the next goal of a fish (randomly found)
 * @param fish
 * @return
 */
struct position determine_new_position(struct position, struct fish* fish);

#endif //POSEIDON_WORLD_H
