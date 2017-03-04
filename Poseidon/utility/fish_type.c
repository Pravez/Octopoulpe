#include "fish_type.h"

char* get_type_string(enum fish_type type){
    switch(type){
        case REDFISH:
            return "REDFISH";
        case OCTOPUS:
            return "OCTOPUS";
        case CATFISH:
            return "CATFISH";
        case BLOBFISH:
            return "BLOBFISH";
        default:
            return "NONE";
    }
}