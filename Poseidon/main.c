#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "server/parser.h"

extern struct _tvector* config_vector;

int main(int argc, char* argv[]){

    parse_config_file("controller.cfg");

    printf("Display timeout value : %d\n", _get_value(config_vector, "fish-update-interval"));


    _delete_tvector(config_vector);

    return EXIT_SUCCESS;
}