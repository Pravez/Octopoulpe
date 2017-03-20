#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "server/parser.h"
#include "server/server.h"
#include "model/aquarium.h"
#include "utility/tools.h"
#include "view/view.h"

#define _GNU_SOURCE

extern struct _tvector* config_vector;

int main(int argc, char* argv[]){

    pthread_t menu_t;
    pthread_t server_t;

    //Job to do with config file ... (before launching server)
    parse_config_file("controller.cfg");
    int port = _get_value(config_vector, "controller-port");

    CHK_ERROR(pthread_create(&menu_t, NULL, main_menu, NULL), "main_menu thread")
    CHK_ERROR(pthread_create(&server_t, NULL, server_process, &port), "server thread");

    _delete_tvector(config_vector);


    /*printf("Display timeout value : %d\n", _get_value(config_vector, "fish-update-interval"));
    _delete_tvector(config_vector);

    _set_verbosity(TRUE);
    _console_log(LOG_HIGH, "BIG PROBLEM");
    _console_log(LOG_LOW, "simple log");

    struct aquarium aquarium1;
    aq__initialize_aquarium(&aquarium1, (struct dimension) {1000, 1000});

    int view1 = aq__add_view(&aquarium1, (struct position) {250, 250}, (struct dimension) {500, 500});
    int view2 = aq__add_view(&aquarium1, (struct position) {100, 100}, (struct dimension) {900, 900});

    aq__add_fish(&aquarium1, fish__create(BLOBFISH, 10, 20));
    aq__add_fish(&aquarium1, fish__create(BLOBFISH, 10, 30));
    aq__add_fish(&aquarium1, fish__create(OCTOPUS, 300, 300));
    aq__add_fish(&aquarium1, fish__create(OCTOPUS, 400, 400));

    display_aquarium(&aquarium1);

    aq__remove_fish(&aquarium1, 1);
    aq__remove_fish(&aquarium1, 3);
    display_aquarium(&aquarium1);*/

    pthread_join(menu_t, NULL);
    pthread_join(server_t, NULL);

    return EXIT_SUCCESS;
}