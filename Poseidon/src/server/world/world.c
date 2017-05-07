#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

#include "world.h"
#include "../../model/aquarium.h"
#include "../communication/client.h"

extern struct aquarium *aquarium;
extern int WORLD_READY;
extern struct vector* observers;
extern pthread_mutex_t mutex_observers;

static double update_rate;

void *world_process(void *pVoid) {
    world_init();
    world_loop();

    pthread_exit(NULL);
}

int world_init() {
    CONSOLE_LOG_INFO("Initializing Aquarium's world");
    srand((unsigned int) time(NULL));
    aquarium->_running = TRUE;
    update_rate = UPDATE_INTERVAL < 0 ? 1.0 / (-UPDATE_INTERVAL) : UPDATE_INTERVAL;

    WORLD_READY = TRUE;

    return 0;
}

struct position determine_new_position(struct position previous, struct fish *fish) {
    struct position newpos = (struct position) {-1, -1};
    struct movement movement;
    int iteration_nb = RAND_IN_RANGE(1, MAX_ITER);
    do {
        switch (fish->_strategy) {
            case HORIZONTAL:
                movement = (struct movement) {RAND_IN_RANGE(3, -1), 0};
                newpos = add_to_position(fish->_current, (movement.x * fish->_speed_rate) * update_rate * iteration_nb,
                                         (movement.y * fish->_speed_rate) * update_rate * iteration_nb);
                break;
            case VERTICAL:
                movement = (struct movement) {0, RAND_IN_RANGE(3, -1)};
                newpos = add_to_position(fish->_current, (movement.x * fish->_speed_rate * 2) * update_rate * iteration_nb,
                                         (movement.y * fish->_speed_rate) * update_rate * iteration_nb);
                break;
            case RANDOM:
                movement = (struct movement) {RAND_IN_RANGE(3, -1), RAND_IN_RANGE(3, -1)};
                newpos = add_to_position(fish->_current, (movement.x * fish->_speed_rate * 2) * update_rate * iteration_nb,
                                         (movement.y * fish->_speed_rate) * update_rate * iteration_nb);
                break;
            case UNREGISTERED:
                break;
        }
    }while(movement.x == 0 && movement.y == 0);

    //Then we adapt speed

    return newpos;
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
        if (!in_bounds(aqv->_outer._starting_position, aqv->_outer._dimensions, fish->_current)) {
            aqv__remove_fish(aqv, fish);
        }
    } else {
        if (in_bounds(aqv->_outer._starting_position, aqv->_outer._dimensions, fish->_current)) {
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


#ifdef DEBUG
        fprintf(stderr, "Fish %s is at %d, %d\n", fish->_id, (int) fish->_current.x, (int) fish->_current.y);
#endif

        if (position_equals(fish->_current, fish->_goal)) {
            fish->_goal = determine_new_position(fish->_goal, fish);
#ifdef DEBUG
            fprintf(stderr, "Fish %s has new goal ! : %d, %d\n", fish->_id, (int) fish->_goal.x, (int) fish->_goal.y);
#endif

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
    LOCK(&mutex_observers);
    struct thread_p* thread;
    for(int i = 0; i < v__size(observers);i++){
        thread = GET_THREAD_PTR(observers, i);
        if(thread->_client->_is_observer)
            pthread_kill(thread->_client->_continuous_sender, SIGNAL_NOTIFICATION);
    }
    UNLOCK(&mutex_observers);
}

int world_loop() {

    //Need to implement an alarm to stop the while true
    while (aquarium->_running) {
        update();
        notify_observers();

        msleep((unsigned long) (update_rate * 1000));
    }

    return 1;
}
