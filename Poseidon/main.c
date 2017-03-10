#include <stdio.h>
#include <stdlib.h>
#include "server/parser.h"
#include "model/aquarium.h"
#include "view/view.h"

extern struct _tvector* config_vector;

int main(int argc, char* argv[]){

    parse_config_file("controller.cfg");

    printf("Display timeout value : %d\n", _get_value(config_vector, "fish-update-interval"));
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
    display_aquarium(&aquarium1);

    display_prompt();

    return EXIT_SUCCESS;
}