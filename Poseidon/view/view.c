#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "view.h"
#include "../utility/file.h"
#include "../model/aquarium.h"

static char *line_read = (char *) NULL;
const char *prompt = YELLOW"Octopoulple "YELLOWBOLD"$ > "RESET;
const char *delim = " ";

struct aquarium aquarium;

void display_menu() {
    printf("Welcome to "REDBOLD"Octopoulpe"RESET" main menu ! Enter a command to continue ...\n");
}

void display_help() {
    printf("Use : $ [command]\n");
    printf("\tload [aquariumfilename]\n");
    printf("\t\tLoad an aquarium from a file (.dot)\n");
    printf("\tshow [aquarium id]\n");
    printf("\t\tShow the description of an aquarium\n");
    printf("\tlist\n");
    printf("\t\tShow the list of aquariums, and their IDs\n");
    printf("\taddview [width]x[height]+[x]+[y]\n");
    printf("\t\tAdd a new aquarium (view), according to its width and height, and the starting position of its rectangle view\n");
    printf("\tdelview [view id]\n");
    printf("\t\tDeletes a view according to its ID\n");
    printf("\tsave [aquarium id] [filename]\n");
    printf("\t\tSaves an aquarium in a file (.dot)\n");
    printf("\tresize [width]x[height]\n");
    printf("\t\tResizes the aquarium with the new width and height (beta)\n");
}

int handle_line() {
    char line[strlen(line_read)];
    strcpy(line, line_read);

    char *token;

    if (!strcmp(line, "quit") || !strcmp(line, "exit"))
        return 0;
    else if (!strcmp(line, "help") || !strcmp(line, "?"))
        display_help();
    else {
        token = strtok(line, delim);

        if (!strcmp(token, "load")) {
            cmd__load_file();
        } else if (!strcmp(token, "show")) {
            cmd__show_aquarium();
        } else if (!strcmp(token, "add")) {
            cmd__add();
        } else if (!strcmp(token, "list")) {
            cmd__list();
        } else if (!strcmp(token, "delete")) {
            cmd__delete();
        } else if (!strcmp(token, "save")) {
            cmd__save_aquarium();
        } else {
            printf("\t> Unrecognized command, use 'help' or '?' to list available commands\n");
        }
    }

    if(line[0] != 0){
        add_history(line);
    }

    return 1;
}

int cmd__load_file(){
    char* string = strtok(NULL, delim);
    load_file(string);

    return 1;
}

int cmd__show_aquarium(){
    char* string = strtok(NULL, delim);
    if(!strcmp(string, "aquarium")){
        display_aquarium(&aquarium);
        return 1;
    }else {
        display_view(aq__get_view_by_id(&aquarium, atoi(string)));
        return 2;
    }
}

int cmd__add(){
    char* string = strtok(NULL, delim);

    if(!strcmp(string, "view")){
        char *value = strtok(NULL, "x");

        int width, height, off_x, off_y;
        width = atoi(value);
        value = strtok(NULL, "+");
        height = atoi(value);
        value = strtok(NULL, "+");
        off_x = atoi(value);
        value = strtok(NULL, "+");
        off_y = atoi(value);

        printf("\t> Added view with ID : %d\n",
               aq__add_view(&aquarium, (struct position) {off_x, off_y}, (struct dimension) {width, height}));

        return 1;
    }else{
        //Do other things if for instance add fish ...
        return 0;
    }
}

int cmd__delete(){
    char* string = strtok(NULL, delim);

    if(!strcmp(string, "view")){
        aqv__remove_aquarium_view(aq__get_view_by_id(&aquarium, atoi(string)));
        return 1;
    }else{
        //Do other stuff with delete ...
        return 0;
    }
}

int cmd__list(){
    //list views IDs
    struct array array = aq__get_views_ids(&aquarium);
    if (array._length == 0) {
        printf("\t> No aquarium created for now !\n");
        return 0;
    } else {
        int *ids = (int *) array._values;
        for (int i = 0; i < array._length; i++) {
            printf("\t> Aquarium view ID n° : %d\n", ids[i]);
        }
        free(ids);

        return 1;
    }
}

int cmd__save_aquarium(){
    char* string = strtok(NULL, delim);
    int id = atoi(string);
    string = strtok(NULL, delim);
    write_file(string);

    return 1;
}


void __init__(){
    aq__initialize_aquarium(&aquarium, (struct dimension){1000, 1000});
}

void *main_menu(void *args) {

    __init__();

    display_menu();

    while (1) {
        line_read = readline(prompt);
        if (!handle_line()) {
            break;
        }
    }

    return NULL;
}