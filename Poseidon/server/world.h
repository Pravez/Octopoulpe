#ifndef POSEIDON_WORLD_H
#define POSEIDON_WORLD_H

#include "../model/fish.h"
#include "../utility/tools.h"

void * world_process(void *pVoid);
int world_init();
int world_loop();
void update();
void update_views();
struct movement next_movement(struct fish* fish);
struct position determine_new_position(struct position);

#endif //POSEIDON_WORLD_H
