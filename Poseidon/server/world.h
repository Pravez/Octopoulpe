#ifndef POSEIDON_WORLD_H
#define POSEIDON_WORLD_H

enum MOVING_STRATEGY{
    STUPID, HORIZ, VERT, RANDOM
};

void* world_process();
int world_init();
int world_loop();

#endif //POSEIDON_WORLD_H
