#include <stdlib.h>
#include "parser.h"


void parse_config_file(char* file){

}

struct _tvector* _init_tvector(int size){
    struct _tvector* tvector = malloc(sizeof(struct _tvector));
    tvector->_size = size;
    tvector->_vector = malloc(sizeof(struct _tuple) * size);

    return tvector;
}

void _delete_tvector(struct _tvector* vector){

}

void _add_tuple(struct _tvector* vector){

}

struct _tvector _get_vector(){

}

int _get_value(struct _tvector* vector, char* key){

}
