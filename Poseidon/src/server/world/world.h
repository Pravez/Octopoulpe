#ifndef POSEIDON_WORLD_H
#define POSEIDON_WORLD_H

#include "../../model/fish.h"
#include "../../utility/tools.h"

void * world_process(void *pVoid);
int world_init();
int world_loop();
void world_end(int signo);
int update_fishes(any_t nothing, any_t item);
void update_view_content(struct aquarium_view* aqv, struct fish* fish);
void update();
void notify_observers();
struct movement next_movement(struct fish* fish);
struct position determine_new_position(struct position, struct fish* fish);

#endif //POSEIDON_WORLD_H
