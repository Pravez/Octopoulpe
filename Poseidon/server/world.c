#include <stdlib.h>
#include <unistd.h>
#include "world.h"
#include "../model/aquarium.h"
#include "../model/fish.h"

static int update_rate;
extern struct aquarium* aquarium;


void* world_process(){
    world_init();
    world_loop();

    return NULL;
}

int world_init(){
    update_rate = 1/60;

    return 0;
}

void update(){

}

int world_loop(){


    while(1){

        sleep(update_rate);
    }
}


