#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "view.h"
#include "../utility/file.h"

#define RETURN_ERROR_MSG(message, errcode) printf("\t> %s\n", message); return errcode;

static char *line_read = (char *) NULL;
const char *prompt = YELLOW"Octopoulple "YELLOWBOLD"$ > "RESET;
const char *delim = " ";

struct aquarium *aquarium;

void display_menu() {
    printf("Welcome to "REDBOLD"Octopoulpe"RESET" main menu ! Enter a command to continue ...\n");
}

void display_help() {
    printf("Use : $ [command]\n");
    printf("\tload [aquariumfilename]\n");
    printf("\t\tLoad an aquarium from a file (.dot)\n");
    printf("\tshow [aquarium id]\n");
    printf("\t\tShow the description of an aquarium\n");
    printf("\tinit [width]x[height]\n");
    printf("\t\tInitializes the aquarium with a given width and height\n");
    printf("\tlist\n");
    printf("\t\tShow the list of aquariums, and their IDs\n");
    printf("\taddview [offset x]x[offset y]+[width]+[height]\n");
    printf("\t\tAdd a new aquarium (view), according to its width and height, and the starting position of its rectangle view\n");
    printf("\tdelview [view id]\n");
    printf("\t\tDeletes a view according to its ID\n");
    printf("\tsave [aquarium id] [filename]\n");
    printf("\t\tSaves an aquarium in a file (.dot)\n");
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
            cmd__call(LOAD);
        } else if (!strcmp(token, "init")) {
            cmd__call(INIT);
        } else if (!strcmp(token, "show")) {
            cmd__call(SHOW);
        } else if (!strcmp(token, "add")) {
            cmd__call(ADD);
        } else if (!strcmp(token, "list")) {
            cmd__call(LIST);
        } else if (!strcmp(token, "delete")) {
            cmd__call(DELETE);
        } else if (!strcmp(token, "save")) {
            cmd__call(SAVE);
        } else {
            printf("\t> Unrecognized command, use 'help' or '?' to list available commands\n");
        }
    }

    if (line[0] != 0) {
        add_history(line);
    }


    return 1;
}

int cmd__call(enum command cmd) {
    if (aquarium == NULL && (cmd == SAVE || cmd == ADD || cmd == LIST || cmd == DELETE || cmd == SHOW)) {
        printf("\t> Impossible to execute this action with non-existing aquarium ... create one first.\n");
        return 0;
    }

    if(aquarium != NULL && (cmd == LOAD || cmd == INIT)){
        printf("\t> An aquarium already exists ... Remove it ? (y/n)\n");
        //TODO
        return 0;
    }

    switch (cmd) {
        case LOAD:
            return cmd__load_file();
        case INIT:
            return cmd__init_aquarium();
        case SHOW:
            return cmd__show_aquarium();
        case ADD:
            return cmd__add();
        case DELETE:
            return cmd__delete();
        case SAVE:
            return cmd__save_aquarium();
        case LIST:
            return cmd__list();
        default:break;
    }

    return 0;
}

int cmd__init_aquarium() {
    char *value = strtok(NULL, "x");

    int width, height;
    width = atoi(value);
    value = strtok(NULL, "x");
    height = atoi(value);

    aquarium = malloc(sizeof(struct aquarium));
    aq__initialize_aquarium(aquarium, (struct dimension) {width, height});
    printf("\t> Initialized new aquarium with dimensions %dx%d\n", width, height);

    return 1;
}

int cmd__load_file() {
    char *string = strtok(NULL, delim);

    if(aquarium != NULL)
        aq__remove_aquarium(aquarium);

    aquarium = load_file(string);
    if(aquarium == NULL){
        printf("\t> Error while reading input file, are you sure it has the right format ?\n");
        return 0;
    }

    return 1;
}

int cmd__show_aquarium() {
    char *string = strtok(NULL, delim);
    if(string != NULL) {
        if (!strcmp(string, "aquarium")) {
            display_aquarium(aquarium);
            return 1;
        } else {
            display_view(aq__get_view_by_id(aquarium, atoi(string)));
            return 2;
        }
    }else{
        printf("\t> Please precise what thing you want to show\n");
        return 0;
    }
}

int cmd__add() {
    char *string = strtok(NULL, delim);
    if(string != NULL) {
        if (!strcmp(string, "view")) {
            char *value = strtok(NULL, "x");

            int width, height, off_x, off_y;
            off_x = atoi(value);
            value = strtok(NULL, "+");
            off_y = atoi(value);
            value = strtok(NULL, "+");
            width = atoi(value);
            value = strtok(NULL, "+");
            height = atoi(value);

            printf("\t> Added view with ID : %d\n",
                   aq__add_view(aquarium, (struct position) {off_x, off_y}, (struct dimension) {width, height}));

            return 1;
        } else {
            //Do other things if for instance add fish ...
            return 0;
        }
    }else{
        printf("\t> Please precise what thing you want to add\n");
        return 0;
    }
}

int cmd__delete() {
    char *string = strtok(NULL, delim);

    if (!strcmp(string, "view")) {
        aqv__remove_aquarium_view(aq__get_view_by_id(aquarium, atoi(string)));
        return 1;
    } else {
        //Do other stuff with delete ...
        return 0;
    }
}

int cmd__list() {
    //list views IDs
    struct array array = aq__get_views_ids(aquarium);
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

int cmd__save_aquarium() {
    char *string = strtok(NULL, delim);

    if(string != NULL){
        //Check if the file exists
        if(access(string, F_OK) != -1){
            printf("\t> File exists ... Erase ? (y/n)\n");
            //TODO
        }else{
            write_file(aquarium, string);
        }
    }else{
        RETURN_ERROR_MSG("Please give a file name ...", 0)
    }

    return 1;
}

void __init__() {
    //Do init stuff ... (private function)
    aquarium = malloc(sizeof(aquarium));
    aq__initialize_aquarium(aquarium, (struct dimension) {1000, 1000});
}

void __end__() {
    if(aquarium != NULL) {
        aq__remove_aquarium(aquarium);
        free(aquarium);
    }
}

void *main_menu(void *args) {
    //__init__();
    display_menu();

    while (1) {
        line_read = readline(prompt);
        if (!handle_line()) {
            break;
        }
        free(line_read);
    }

    __end__();
    return NULL;
}