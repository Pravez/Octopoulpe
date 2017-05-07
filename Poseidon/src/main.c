#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <readline.h>

#include "server/parser.h"
#include "server/server.h"
#include "model/aquarium.h"
#include "view/view.h"
#include "server/world/world.h"

extern struct _tvector* config_vector;
extern boolean readline_started;
struct aquarium* aquarium;

pthread_mutex_t mutex_observers;

pthread_t thread_menu;
pthread_t thread_server;
pthread_t thread_world;

int WORLD_READY;
int SERVER_READY;
int world_execution;

int main(int argc, char* argv[]){
    _set_verbosity(TRUE);
    _set_output_file("log.txt");
    readline_started = FALSE;
    CONSOLE_LOG_INFO("Starting Octopoulpe");
    CONSOLE_LOG_INFO("Initializing values ...");

    WORLD_READY = FALSE;
    SERVER_READY = FALSE;
    world_execution = TRUE;

    // Initialisation of the aquarium
    /*aquarium = malloc(sizeof(struct aquarium));
    aq__initialize_aquarium(aquarium, AQUARIUM_DIMENSIONS);

    aq__add_view(aquarium, (struct position) {250, 250}, (struct dimension){250, 250}, "N1");*/
    aquarium = NULL;

    //Job to do with config file ... (before launching server)
    if(access("controller.cfg", F_OK) != -1) {

        //We parse config file
        parse_config_file("controller.cfg");
        UPDATE_INTERVAL = _get_value(config_vector, "fish-update-interval");
        DISPLAY_TIMEOUT_VALUE = _get_value(config_vector, "display-timeout-value");
        CONTROLLER_PORT = _get_value(config_vector, "controller-port");
        _delete_tvector(config_vector);
        CONSOLE_LOG_INFO("Successfully parsed config file");
        CONSOLE_LOG_INFO("Fish update interval is %d seconds", UPDATE_INTERVAL);
        CONSOLE_LOG_INFO("Display timeout value is %d seconds", DISPLAY_TIMEOUT_VALUE);
        CONSOLE_LOG_INFO("Controller port is %d", CONTROLLER_PORT);

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
        _close_output_file();

        return EXIT_SUCCESS;
    }else{
        _close_output_file();
        CONSOLE_LOG_ERR("Impossible to find controller.cfg config file");

        return EXIT_FAILURE;
    }
}
