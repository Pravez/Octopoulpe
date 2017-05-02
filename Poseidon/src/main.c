#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "server/parser.h"
#include "server/server.h"
#include "model/aquarium.h"
#include "view/view.h"
#include "server/world.h"


extern struct _tvector* config_vector;
struct aquarium* aquarium;

pthread_mutex_t mutex_observers;

pthread_t thread_menu;
pthread_t thread_server;
pthread_t thread_world;

int WORLD_READY;
int SERVER_READY;

int main(int argc, char* argv[]){

    _set_verbosity(TRUE);
    CONSOLE_LOG_INFO("Starting Octopoulpe");
    CONSOLE_LOG_INFO("Initializing values ...");

    WORLD_READY = FALSE;
    SERVER_READY = FALSE;

    // Initialisation of the aquarium
    aquarium = malloc(sizeof(struct aquarium));
    aq__initialize_aquarium(aquarium, AQUARIUM_DIMENSIONS);
    
    char* view1 = aq__add_view(aquarium, (struct position) {250, 250}, (struct dimension){250, 250}, "N1");

    //Job to do with config file ... (before launching server)
    if(access("controller.cfg", F_OK) != -1) {

        //We parse config file
        parse_config_file("controller.cfg");
        UPDATE_INTERVAL = _get_value(config_vector, "fish-update-interval");
        DISPLAY_TIMEOUT_VALUE = _get_value(config_vector, "display-timeout-value");
        CONTROLLER_PORT = _get_value(config_vector, "controller-port")+4;
        _delete_tvector(config_vector);
        CONSOLE_LOG_INFO("Successfully parsed config file");

        //First we allocate the mutex and lock it
        pthread_mutex_init(&mutex_observers, NULL);
        pthread_mutex_lock(&mutex_observers);

        CONSOLE_LOG_INFO("Launching threads ...");
        CHK_ERROR(pthread_create(&thread_world, NULL, world_process, NULL), "world thread");
        CHK_ERROR(pthread_create(&thread_menu, NULL, main_menu, NULL), "main_menu thread")
        CHK_ERROR(pthread_create(&thread_server, NULL, server_process, NULL), "server thread");
        CONSOLE_LOG_INFO("Done !");


        pthread_join(thread_server, NULL);
        pthread_join(thread_menu, NULL);

        pthread_mutex_destroy(&mutex_observers);

        return EXIT_SUCCESS;
    }else{
        CONSOLE_LOG_ERR("Impossible to find controller.cfg config file");
    }


    // To test : a false aquarium
    /*    aquarium = malloc(sizeof(struct aquarium));
        aq__initialize_aquarium(aquarium, AQUARIUM_DIMENSIONS);
        aq__add_view(aquarium, (struct position) {250, 250}, (struct dimension) {500, 500}, "Cookie");
        aq__add_view(aquarium, (struct position) {100, 100}, (struct dimension) {900, 900}, "Donald");
        // end of the test

        char * res = malloc(sizeof(400));
        struct client *henry = malloc(sizeof(struct client));
        henry->id = NULL;
        asw__hello("\n", res, henry);
        printf("main\t%s", res);
        asw__hello("\n", res, henry);
        printf("main\t%s", res);
        asw__hello("\n", res, henry);
        printf("main\t%s", res);

        // Henry a la vue "Cookie"
        struct dimension def = (struct dimension){1, 1};
        aq__add_fish_to_aqv(aquarium,"Cookie",fish__create(BLOBFISH, 10, 20, "Bibi", HANDV, def));
        aq__add_fish_to_aqv(aquarium,"Cookie",fish__create(BLOBFISH, 40, 50, "Bobo",HANDV,def));

        asw__get_fishes("\n",res,henry);
        printf("main\t%s",res);

        asw__log("out\n",res,henry);
        printf("main\t%s",res);
        return EXIT_SUCCESS;*/
}
