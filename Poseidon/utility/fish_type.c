#include <string.h>
#include "fish_type.h"

char* get_type_string(enum fish_type type){
    switch(type){
        case REDFISH:
            return "RedFish";
        case OCTOPUS:
            return "Octopus";
        case CATFISH:
            return "CatFish";
        case BLOBFISH:
            return "BlobFish";
        default:
            return "NONE";
    }
}

enum fish_type get_type_from_string(char* type){
    if(!strcmp(type, "RedFish")){
        return REDFISH;
    }
    if(!strcmp(type, "Octopus")){
        return OCTOPUS;
    }
    if(!strcmp(type, "CatFish")){
        return CATFISH;
    }
    if(!strcmp(type, "BlobFish")){
        return BLOBFISH;
    }

    return NONE;
}

enum MOVING_STRATEGY get_strategy_from_string(char* type){
    if(!strcmp(type, "RandomWayPoint")){
        return RANDOM;
    }
    if(!strcmp(type, "HorizontalWayPoint")){
        return HORIZONTAL;
    }
    if(!strcmp(type, "VerticalWayPoint")){
        return VERTICAL;
    }

    return UNREGISTERED;
}