#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"


void parse_config_file(const char* file){
    FILE* conf_file;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    conf_file = fopen(file, "r");

    if(conf_file == NULL)
        return;

    int iterator = 0;
    char temp[30];

    struct _tuple temp_array[10];
    int tuple_nbr = 0;

    while ((read = getline(&line, &len, conf_file)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);

        if(strcmp(line, ";") == 0)
            break;

        for(int i=0;i < read;i++){
            if(line[i] != '=' && line[i] != '\n') {
                temp[i - iterator] = line[i];
            }else{
                temp[i - iterator] = '\0';
                if(line[i] == '=') {
                    temp_array[tuple_nbr]._key = malloc(sizeof(char) * (i - iterator));
                    strcpy(temp_array[tuple_nbr]._key, temp);
                    iterator = i+1;
                }else{
                    temp_array[tuple_nbr++]._value = atoi(temp);
                    iterator = 0;
                }
            }

        }
    }

    fclose(conf_file);

    config_vector = _init_tvector(tuple_nbr);
    for(int i = 0;i < tuple_nbr;i++){
        _add_tuple(config_vector, temp_array[i]);
        //if(i!=3)
        //free(temp_array[i]._key);
    }
}

struct _tvector* _init_tvector(int size){
    struct _tvector* tvector = malloc(sizeof(struct _tvector));
    tvector->_size = size;
    tvector->_ptr_current = 0;
    tvector->_vector = (struct _tuple*) malloc(sizeof(struct _tuple)*size);

    return tvector;
}

void _delete_tvector(struct _tvector* vector){
    for(int i = 0;i < vector->_ptr_current;i++){
        free(vector->_vector[i]._key);
    }
    free(vector->_vector);
    free(vector);
}

void _add_tuple(struct _tvector* vector, struct _tuple tuple){
    vector->_vector[vector->_ptr_current++] = tuple;
}

struct _tuple _create_tuple(char* key, int value){
    struct _tuple tuple;
    tuple._key = key;
    tuple._value = value;

    return tuple;
}

int _get_value(struct _tvector* vector, char* key){
    for(int i = 0;i < vector->_ptr_current;i++){
        if(strcmp(vector->_vector[i]._key, key) == 0){
            return vector->_vector[i]._value;
        }
    }

    return -1;
}
