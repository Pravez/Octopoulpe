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

    aqv__add_fish(aq__get_view_by_id(&aquarium1, view), fish__create(OCTOPUS, 5, 6));

    display_view(aq__get_view_by_id(&aquarium1, view));

    return EXIT_SUCCESS;
}