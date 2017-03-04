#ifndef POSEIDON_FISH_TYPE_H
#define POSEIDON_FISH_TYPE_H

enum fish_type{
    REDFISH, OCTOPUS, CATFISH, BLOBFISH
};

char* get_type_string(enum fish_type type);

#endif //POSEIDON_FISH_TYPE_H
