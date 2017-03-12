#ifndef POSEIDON_VIEW_H
#define POSEIDON_VIEW_H

#include "../utility/tools.h"

struct command{
    int _args;

};

enum menu_action{
    LOAD, SHOW, ADDVIEW, DELVIEW, SAVE
};

void* main_menu(void *args);
void display_prompt();
int handle_line();

int cmd__load_file();
int cmd__show_aquarium();
int cmd__add();
int cmd__list();
int cmd__delete();
int cmd__save_aquarium();

#endif //POSEIDON_VIEW_H
