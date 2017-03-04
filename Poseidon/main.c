#include <stdio.h>
#include <stdlib.h>
#include "server/parser.h"
#include "model/aquarium.h"

extern struct _tvector* config_vector;

int main(int argc, char* argv[]){

    parse_config_file("controller.cfg");

    printf("Display timeout value : %d\n", _get_value(config_vector, "fish-update-interval"));
    _delete_tvector(config_vector);

    _set_verbosity(TRUE);
    _console_log(LOG_HIGH, "BIG PROBLEM");
    _console_log(LOG_LOW, "simple log");

    struct aquarium aquarium1;
    aq__initialize_aquarium(&aquarium1, (struct dimension) {100, 100});

    int view = aq__add_view(&aquarium1, (struct position) {10, 10}, (struct dimension) {10, 10});

    aq__add_fish_to_view(&aquarium1, view, fish__create(OCTOPUS, 50, 50));
    aq__add_fish(&aquarium1, fish__create(BLOBFISH, 10, 20));
    aq__add_fish(&aquarium1, fish__create(BLOBFISH, 10, 30));
    aq__add_fish(&aquarium1, fish__create(BLOBFISH, 10, 40));

    display_aquarium(&aquarium1);

    aq__remove_fish(&aquarium1, 1);
    display_aquarium(&aquarium1);

    return EXIT_SUCCESS;
}