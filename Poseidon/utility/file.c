#include "file.h"

int write_file(struct aquarium *aquarium, char *filename) {
    FILE *file = fopen(filename, "w");

    fprintf(file, "%dx%d\n", aquarium->_dimensions.width, aquarium->_dimensions.height);

    for (int i = 0; i < v__size(&aquarium->_views); i++) {
        struct aquarium_view* aq_view = vi__convert_aq_view(v__get(&aquarium->_views, i));
        fprintf(file, "N%s %dx%d+%d+%d\n", aq_view->_id, (int)aq_view->_inner._starting_position.x,
                (int)aq_view->_inner._starting_position.y, aq_view->_inner._dimensions.width,
                aq_view->_inner._dimensions.height);
    }

    fclose(file);

    return 1;
}

struct aquarium *load_file(char *filename) {
    FILE *file = fopen(filename, "r");
    char *buffer;
    size_t length;
    ssize_t read_size;
    struct aquarium_view_properties prop;

    struct aquarium *aquarium = NULL;

    read_size = getdelim(&buffer, &length, '\n', file);

    struct dimension dims = parse__aq_dimensions(buffer);

    if (dims.width != -1 && dims.height != -1) {
        aquarium = malloc(sizeof(struct aquarium));
        aq__initialize_aquarium(aquarium, dims);

        while ((read_size = getdelim(&buffer, &length, '\n', file)) != -1) {
            prop = parse__aq_view(buffer);
            aq__add_view(aquarium, prop._position, prop._dimensions, prop._id);
        }
    }

    free(buffer);
    fclose(file);

    return aquarium;
}

struct dimension parse__aq_dimensions(char *line) {
    struct dimension dim;
    char *string = strtok(line, "x");
    if (string != NULL) {
        dim.width = atoi(string);
        string = strtok(NULL, "x");
        dim.height = atoi(string);
    } else {
        dim.height = -1;
        dim.width = -1;
    }


    return dim;
}

struct aquarium_view_properties parse__aq_view(char *line) {
    struct aquarium_view_properties prop;
    prop._id = strtok(line, " ");
    prop._position.x = atoi(strtok(NULL, "x"));
    prop._position.y = atoi(strtok(NULL, "+"));
    prop._dimensions.width = atoi(strtok(NULL, "+"));
    prop._dimensions.height = atoi(strtok(NULL, "+"));

    return prop;
}