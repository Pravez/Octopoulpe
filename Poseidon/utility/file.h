#ifndef POSEIDON_FILE_H
#define POSEIDON_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../model/aquarium.h"
#include "../utility/tools.h"

struct aquarium_view_properties{
    struct position _position;
    struct dimension _dimensions;
    int _id;
};

int write_file(struct aquarium* aquarium, char* filename);
struct aquarium* load_file(char* filename);
struct dimension parse__aq_dimensions(char* line);
struct aquarium_view_properties parse__aq_view(char* line);

#endif //POSEIDON_FILE_H
