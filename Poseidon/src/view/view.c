#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <pthread.h>

#include "view.h"
#include "../utility/file.h"
#include "../server/communication/interface.h"

#define RETURN_ERROR_MSG(message, errcode) printf("\t> %s\n", message); return errcode;

static char *line_read = (char *) NULL;
const char *prompt = YELLOW"Octopoulple "YELLOWBOLD"$ > "RESET;
static const char *delim = " ";

extern struct aquarium *aquarium;
extern pthread_t thread_server;
extern pthread_t thread_world;
extern int WORLD_READY;
extern int SERVER_READY;
extern int world_execution;

void display_menu() {
    while (!SERVER_READY);
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
    printf("\tadd view [offset x]x[offset y]+[width]+[height]\n");
    printf("\t\tAdd a new aquarium (view), according to its width and height, and the starting position of its rectangle view\n");
    printf("\tdel view [view id]\n");
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
        if (token != NULL) {
            //^C handling
            if (*token == -1)
                return 0;

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
            } else if (!strcmp(token, "del")) {
                cmd__call(DELETE);
            } else if (!strcmp(token, "save")) {
                cmd__call(SAVE);
            } else {
                printf("\t> Unrecognized command, use 'help' or '?' to list available commands\n");
            }

            //add_history(line);

        }
    }


    return 1;
}


int cmd__call(enum command cmd) {
    if (aquarium == NULL && (cmd == SAVE || cmd == ADD || cmd == LIST || cmd == DELETE || cmd == SHOW)) {
        RETURN_ERROR_MSG("Impossible to execute this action with non-existing aquarium ... create one first.", 0)
    }

    if (aquarium != NULL && (cmd == LOAD || cmd == INIT)) {
        printf("\t> An aquarium already exists ... Remove it ? (y/n)\n");
        char answer[4];
        scanf("%s", answer);
        if (strcmp(answer, "y")) {
            return 0;
        }
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
        default:
            break;
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

    if (aquarium != NULL)
        aq__remove_aquarium(aquarium);

    if (access(string, F_OK) != -1) {
        aquarium = load_file(string);
        if (aquarium == NULL) {
            RETURN_ERROR_MSG("Error while reading input file, are you sure it has the right format ?\n", 0)
        } else {
            printf("\t> Successfully loaded file \n");
            aquarium->_running = TRUE;
        }
    } else {
        RETURN_ERROR_MSG("Impossible to find file, please verify if it exists.\n", 0)
    }


    return 1;
}

int cmd__show_aquarium() {
    char *string = strtok(NULL, delim);
    if (string != NULL) {
        if (!strcmp(string, "aquarium")) {
            display_aquarium(aquarium);
            return 1;
        } else {
            struct aquarium_view *view = aq__get_view_by_id(aquarium, string);
            if (view != NULL) {
                display_view(view);
            } else {
                printf("\t> Could not find a view with name %s\n", string);
                return 0;
            }

            return 2;
        }
    } else {
        printf("\t> Please precise what thing you want to show\n");
        return 0;
    }
}

int cmd__add() {
    char *string = strtok(NULL, delim);
    if (string != NULL) {
        if (!strcmp(string, "view")) {
            char *id, *width, *height, *off_x, *off_y;

            id = strtok(NULL, " ");
            off_x = strtok(NULL, "x");
            off_y = strtok(NULL, "+");
            width = strtok(NULL, "+");
            height = strtok(NULL, "+");

            if (id != NULL && off_x != NULL && off_y != NULL && width != NULL && height != NULL) {
                if (atoi(off_x) != -1 && atoi(off_y) != -1 && atoi(width) != -1 && atoi(height) != -1) {
                    char *returned = aq__add_view(aquarium, (struct position) {atoi(off_x), atoi(off_y)},
                                                  (struct dimension) {atoi(width), atoi(height)}, id);
                    if (returned == NULL)
                        printf("\t> Unable to add view : ID %s already taken\n", id);
                    else
                        printf("\t> Added view with ID : %s\n", returned);

                    return 1;
                }
            }

            printf("\t> Please verify you're using the correct syntax\n");

        } else {
            printf("\t> Cannot add %s, unable to handle it.\n", string);
        }
    } else {
        printf("\t> Please precise what thing you want to add (view)\n");
    }

    return 0;
}

int cmd__delete() {
    char *string = strtok(NULL, delim);

    if (!strcmp(string, "view")) {
        char *view_name = strtok(NULL, delim);
        if(view_name != NULL){
            struct aquarium_view *view = aq__get_view_by_id(aquarium, view_name);
            if (view != NULL) {
                aq__remove_aquarium_view(aquarium, view_name);
                printf("\t> Removed view with name %s, clients will be notified.\n", view_name);

            } else {
                printf("\t> Could not find a view with name %s\n", view_name);
                return 0;
            }
            return 1;
        }else{
            printf("\t> Please give a name to remove view\n");
            return 0;
        }
    } else {
        printf("\t> Please precise what thing you want to delete (view)\n");
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
        char **ids = (char **) array._values;
        for (int i = 0; i < array._length; i++) {
            printf("\t> Aquarium view ID n° : %s\n", ids[i]);
        }
        free(ids);

        return 1;
    }
}

int cmd__save_aquarium() {
    char *string = strtok(NULL, delim);

    if (string != NULL) {
        //Check if the file exists
        if (access(string, F_OK) != -1) {
            printf("\t> File exists ... Erase ? (y/n) ");
            char answer[4];
            scanf("%s", answer);
            if (!strcmp(answer, "y")) {
                write_file(aquarium, string);
                printf("\t> Successfully saved aquarium in %s\n", string);
            }
        } else {
            write_file(aquarium, string);
            printf("\t> Successfully saved aquarium in %s\n", string);
        }
    } else {
        RETURN_ERROR_MSG("Please give a file name ...\n", 0)
    }

    return 1;
}

void __init__() {
    //Do init stuff ... (private function)
    aquarium = malloc(sizeof(aquarium));
    aq__initialize_aquarium(aquarium, (struct dimension) {1000, 1000});
}

void __end__() {
    printf("\t> Stopping server thread ...\n");
    pthread_kill(thread_server, SIGNAL_END_EVERYTHING);
    pthread_join(thread_server, NULL);

    printf("\t> Stopping aquarium emulation thread ...\n");
    world_execution = FALSE;
    if(aquarium != NULL)
        aquarium->_running = FALSE;
    pthread_join(thread_world, NULL);

    printf("\t> Clearing data ...\n");
    if (aquarium != NULL) {
        aq__remove_aquarium(aquarium);
        free(aquarium);
    }
}

void *main_menu(void *args) {
    //__init__();
#ifndef NOREADLINE
    readline_started = TRUE;
#endif
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
