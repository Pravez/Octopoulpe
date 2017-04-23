#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#include "world.h"
#include "../model/aquarium.h"
#include "../model/fish.h"

static double update_rate;
extern struct aquarium* aquarium;

int world_initialized;
static int run;


void * world_process(void *pVoid){
    world_init();
    world_loop();

    return NULL;
}

int world_init(){
    srand((unsigned int) time(NULL));
    update_rate = 1.0/60;
    world_initialized = 1;

    return 0;
}

struct position determine_new_position(struct position previous){
    return (struct position){(unsigned int)RAND_IN_RANGE(1000, 0), (unsigned int)RAND_IN_RANGE(1000, 0)};
}

struct movement next_movement(struct fish* fish){
    struct movement movement = (struct movement){0, 0};
    switch(fish->_strategy){
        case HANDV:
            if(fish->_current.x - fish->_goal.x > 0)
                movement.x = -1;
            else if(fish->_current.x - fish->_goal.x < 0)
                movement.x = 1;
            else
                movement.x = 0;

            if(fish->_current.y - fish->_goal.y > 0)
                movement.y = -1;
            else if(fish->_current.y - fish->_goal.y < 0)
                movement.y = 1;
            else
                movement.y = 0;
            break;
        case RANDOM:break;
    }

    return movement;
}

int update_fishes(any_t nothing, any_t item){

    struct fish* fish = (struct fish*)item;
    struct movement movement = next_movement(fish);

    struct position newpos = add_to_position(fish->_current, (movement.x*fish->_speed_rate)*update_rate, (movement.y*fish->_speed_rate)*update_rate);
    fish->_current = newpos;

    if(position_equals(fish->_current, fish->_goal)){
        fish->_goal = determine_new_position(fish->_goal);
        fprintf(stderr, "Fish %s has new goal ! : %d, %d\n", fish->_id, (int)fish->_goal.x, (int)fish->_goal.y);
    }

    return MAP_OK;
}

void update(){
    hashmap_iterate(aquarium->_fishes, (PFany) update_fishes, NULL);
}

void update_views(){
    //Here we update fishes in views (calculate if fishes are always in views etc ...)
}

int world_loop(){

    run = 1;

    //Need to implement an alarm to stop the while true
    while(run){
        update();

        update_views();
        usleep((unsigned int) (update_rate*1000));
    }

    return 1;
}


