#ifndef POSEIDON_VIEW_H
#define POSEIDON_VIEW_H

#include <unistd.h>

#include "../utility/tools.h"

enum command{
    LOAD, INIT, SHOW, ADD, DELETE, SAVE, LIST
};

void* main_menu(void *args);
int handle_line();

int cmd__call(enum command cmd);
int cmd__init_aquarium();
int cmd__load_file();
int cmd__show_aquarium();
int cmd__add();
int cmd__list();
int cmd__delete();
int cmd__save_aquarium();

#endif //POSEIDON_VIEW_H
