#include "file.h"

int write_file(struct aquarium* aquarium, char* filename){

}

struct aquarium* load_file(char* filename){
    FILE* file = fopen(filename, "r");
    char* buffer;
    size_t length;
    ssize_t read_size;
    struct aquarium_view_properties prop;

    struct aquarium* aquarium;
    aquarium = malloc(sizeof(struct aquarium));

    read_size = getdelim(&buffer, &length, '\n', file);
    aq__initialize_aquarium(aquarium, parse__aq_dimensions(buffer));

    while((read_size = getdelim(&buffer, &length, '\n', file)) != -1){
        prop = parse__aq_view(buffer);
        aq__add_view(aquarium, prop._position, prop._dimensions);
    }

    return aquarium;
}

struct dimension parse__aq_dimensions(char* line){
    struct dimension dim;
    char* string = strtok(line, "x");
    dim.width = atoi(string);
    string = strtok(NULL, "x");
    dim.height = atoi(string);

    return dim;
}

struct aquarium_view_properties parse__aq_view(char* line){
    struct aquarium_view_properties prop;
    prop._id = atoi(strtok(line, " "));
    prop._position.x = atoi(strtok(NULL, "x"));
    prop._position.y = atoi(strtok(NULL, "+"));
    prop._dimensions.width = atoi(strtok(NULL, "+"));
    prop._dimensions.height = atoi(strtok(NULL, "+"));

    return prop;
}