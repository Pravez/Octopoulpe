#include <stdio.h>
#include <stdlib.h>
#include "server/parser.h"
#include "utility/verbosity.h"

extern struct _tvector* config_vector;

int main(int argc, char* argv[]){

    parse_config_file("controller.cfg");

    printf("Display timeout value : %d\n", _get_value(config_vector, "fish-update-interval"));
    _delete_tvector(config_vector);

    _set_verbosity(TRUE);
    _console_log(LOG_HIGH, "BIG PROBLEM");
    _console_log(LOG_LOW, "simple log");

    return EXIT_SUCCESS;
}