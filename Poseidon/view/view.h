#ifndef POSEIDON_VIEW_H
#define POSEIDON_VIEW_H

#include "../utility/tools.h"

struct command{
    int _args;

};

enum menu_action{
    LOAD, SHOW, ADDVIEW, DELVIEW, SAVE
};

void display_menu();
void display_prompt();
int handle_line();

#endif //POSEIDON_VIEW_H
