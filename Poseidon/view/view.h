#ifndef POSEIDON_VIEW_H
#define POSEIDON_VIEW_H

#include "../utility/tools.h"

struct command{
    int _args;

};

enum menu_action{
    ADDFISH, ADDVIEW, STATUS
};

void display_menu();
void display_prompt();
void do_action(enum menu_action action);

#endif //POSEIDON_VIEW_H
