#include <stdio.h>
#include <readline/readline.h>
#include "view.h"

static char *line_read = (char *)NULL;

void display_menu(){
    printf("Welcome to "REDBOLD"Octopoulpe"RESET" main menu ! Enter a command to continue ...\n");
}

void display_help(){
    printf("Use : $> [command]\n");
    printf("\taddfish [type] at [x]x[y], [width]x[height], [moving tendancy]\n");
    printf("\t\tAdds a fish on position x and y, with width and height and with a certain moving tendancy\n\n");
    printf("\taddview ");
}

void display_prompt();

void do_action(enum menu_action action);