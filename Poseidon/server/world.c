#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

#include "world.h"
#include "../model/aquarium.h"
#include "../model/fish.h"
#include "../utility/vector.h"
#include "../utility/data.h"
#include "answer.h"

extern struct aquarium *aquarium;
extern struct vector* observers;
extern pthread_mutex_t mutex_observers;

static double update_rate;
static int run;

time_t world_last_sleep;
int world_initialized;


void *world_process(void *pVoid) {
    world_init();
    world_loop();

    return NULL;
}

int world_init() {
    srand((unsigned int) time(NULL));
    update_rate = SPEED_RATE < 0 ? 1.0 / (-SPEED_RATE) : SPEED_RATE;
    world_initialized = 1;

    return 0;
}

struct position determine_new_position(struct position previous, struct fish *fish) {
    switch (fish->_strategy) {
        case HORIZONTAL:
            return (struct position) {(unsigned int) RAND_IN_RANGE(1000, 0), previous.y};
            //return (struct position) {fish->_current}
        case VERTICAL:
            return (struct position) {previous.x, (unsigned int) RAND_IN_RANGE(1000, 0)};
        case RANDOM:
            return (struct position) {(unsigned int) RAND_IN_RANGE(1000, 0), (unsigned int) RAND_IN_RANGE(1000, 0)};
    }

    //Then we adapt speed

    return (struct position) {-1, -1};
}

struct movement next_movement(struct fish *fish) {
    struct movement movement = (struct movement) {0, 0};
    if (fish->_current.x - fish->_goal.x > 0)
        movement.x = -1;
    else if (fish->_current.x - fish->_goal.x < 0)
        movement.x = 1;
    else
        movement.x = 0;

    if (fish->_current.y - fish->_goal.y > 0)
        movement.y = -1;
    else if (fish->_current.y - fish->_goal.y < 0)
        movement.y = 1;
    else
        movement.y = 0;
    return movement;
}

void update_view_content(struct aquarium_view *aqv, struct fish *fish) {
    if (hashmap_get(aqv->_fishes, fish->_id, NULL) == MAP_OK) {
        if (!in_bounds(aqv->_inner._starting_position, aqv->_inner._dimensions, fish->_current)) {
            aqv__remove_fish(aqv, fish);
        }
    } else {
        if (in_bounds(aqv->_inner._starting_position, aqv->_inner._dimensions, fish->_current)) {
            aqv__add_fish(aqv, fish);
        }
    }
}

int update_fishes(any_t nothing, any_t item) {

    struct fish *fish = (struct fish *) item;
    struct aquarium_view *aqv;

    if (fish->_running) {
        struct movement movement = next_movement(fish);

        struct position newpos = add_to_position(fish->_current, (movement.x * fish->_speed_rate) * update_rate,
                                                 (movement.y * fish->_speed_rate) * update_rate);
        fish->_current = newpos;
        fish->_next_position =
                fish->_speed_rate < (1 / 30) ? add_to_position(newpos, (movement.x * fish->_speed_rate) * update_rate,
                                                               (movement.y * fish->_speed_rate) * update_rate)
                                             : newpos;

        //TODO test if new position*2 is after the position in itself



        fprintf(stderr, "Fish %s is at %d, %d\n", fish->_id, (int) fish->_current.x, (int) fish->_current.y);

        if (position_equals(fish->_current, fish->_goal)) {
            fish->_goal = determine_new_position(fish->_goal, fish);
            fprintf(stderr, "Fish %s has new goal ! : %d, %d\n", fish->_id, (int) fish->_goal.x, (int) fish->_goal.y);
        }
    }

    //Here we test fishes are always on good views
    for (int i = 0; i < aquarium->_views._current_size; i++) {
        aqv = GET_VIEW_PTR(&aquarium->_views, i);
        update_view_content(aqv, fish);
    }

    return MAP_OK;
}

void update() {
    hashmap_iterate(aquarium->_fishes, (PFany) update_fishes, NULL);
}

void notify_observers(){
    pthread_mutex_lock(&mutex_observers);
    struct client* client;
    for(int i = 0; i < v__size(observers);i++){
        client = GET_CLIENT_PTR(observers, i);
        if(client->_is_observer)
            pthread_kill(client->_continuous_sender, SIGNAL_NOTIFICATION);
    }
    pthread_mutex_unlock(&mutex_observers);
}

int world_loop() {

    run = 1;

    //Need to implement an alarm to stop the while true
    while (run) {
        update();
        notify_observers();

        msleep((unsigned long) (update_rate * 1000));
    }

    return 1;
}


