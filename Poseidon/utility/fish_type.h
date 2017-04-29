#ifndef POSEIDON_FISH_TYPE_H
#define POSEIDON_FISH_TYPE_H

enum fish_type{
    REDFISH, OCTOPUS, CATFISH, BLOBFISH, NONE
};

enum MOVING_STRATEGY{
    HORIZONTAL, VERTICAL, RANDOM
};

char* get_type_string(enum fish_type type);
enum fish_type get_type_from_string(char* type);
enum MOVING_STRATEGY get_strategy_from_string(char* type);

#endif //POSEIDON_FISH_TYPE_H
