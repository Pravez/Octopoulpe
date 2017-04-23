#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "server/parser.h"
#include "server/server.h"
#include "model/aquarium.h"
#include "utility/tools.h"
#include "view/view.h"

#include "server/answer.h"

extern struct _tvector* config_vector;
struct aquarium* aquarium;

//For testing
struct aquarium aquarium1;

int main(int argc, char* argv[]){
    /*
    pthread_t menu_t;
    pthread_t server_t;

    // Initialisation of the aquarium
    aq__initialize_aquarium(&aquarium1, (struct dimension) {1000, 1000});
    
    char* view1 = aq__add_view(&aquarium1, (struct position) {250, 250}, (struct dimension) {500, 500}, "N1");
    char* view2 = aq__add_view(&aquarium1, (struct position) {100, 100}, (struct dimension) {900, 900}, "N2");

    //Job to do with config file ... (before launching server)
    parse_config_file("/home/lparant/S8-projects/Octopoulpe/Poseidon/controller.cfg"); // How to add a .cfg file to the CMakefile ?
    int port = _get_value(config_vector, "controller-port");

    CHK_ERROR(pthread_create(&menu_t, NULL, main_menu, NULL), "main_menu thread")
    CHK_ERROR(pthread_create(&server_t, NULL, server_process, &port), "server thread");

    printf("Display timeout value : %d\n", _get_value(config_vector, "fish-update-interval"));
    _delete_tvector(config_vector);

    _set_verbosity(FALSE);
    _console_log(LOG_HIGH, "BIG PROBLEM");
    _console_log(LOG_LOW, "simple log");

    aq__add_fish_to_aqv(&aquarium1, view1, fish__create(BLOBFISH, 10, 20, "jeanmi"));
    aq__add_fish_to_aqv(&aquarium1, view1, fish__create(BLOBFISH, 10, 30, "jeanma"));
    aq__add_fish_to_aqv(&aquarium1, view2, fish__create(OCTOPUS, 300, 300, "jeanmo"));
    aq__add_fish_to_aqv(&aquarium1, view2, fish__create(OCTOPUS, 400, 400, "jeanbite"));

    aq__remove_fish(&aquarium1, "jeanbite");

    display_aquarium(&aquarium1);

    aq__remove_fish(&aquarium1, 1);
    aq__remove_fish(&aquarium1, 3);
    display_aquarium(&aquarium1);

    pthread_join(menu_t, NULL);
    pthread_join(server_t, NULL);

    return EXIT_SUCCESS;
    */

    // To test : a false aquarium1
    aq__initialize_aquarium(&aquarium1, (struct dimension) {1000, 1000});
    aq__add_view(&aquarium1, (struct position) {250, 250}, (struct dimension) {500, 500}, "Cookie");
    aq__add_view(&aquarium1, (struct position) {100, 100}, (struct dimension) {900, 900}, "Donald");
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
    aq__add_fish_to_aqv(&aquarium1,"Cookie",fish__create(BLOBFISH, 10, 20, "Bibi",4,5));
    aq__add_fish_to_aqv(&aquarium1,"Cookie",fish__create(BLOBFISH, 40, 50, "Bobo",4,5));

    asw__get_fishes("\n",res,henry);
    printf("main\t%s",res);
    return EXIT_SUCCESS;
}
