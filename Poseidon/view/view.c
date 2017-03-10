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
const char *prompt = "Octopoulple $ > ";
const char *delim = " ";
int running;

struct aquarium aquarium1;

void display_menu() {
    printf("Welcome to "REDBOLD"Octopoulpe"RESET" main menu ! Enter a command to continue ...\n");
}

void display_help() {
    printf("Use : $ [command]\n");
    printf("\tload [aquariumfilename]\n\n");
    printf("\tshow [aquarium id]\n\n");
    printf("\taddview [view id] [width]x[height]+[x]+[y]\n\n");
    printf("\tdelview [view id]\n\n");
    printf("\tsave [aquarium id] [filename]\n\n");
}

int handle_line(){
    char line[strlen(line_read)];
    strcpy(line, line_read);

    char* token;

    if(!strcmp(line, "quit") || !strcmp(line, "exit"))
        return 0;
    else if(!strcmp(line, "help") || !strcmp(line, "?"))
        display_help();
    else{
        token = strtok(line, delim);
        char* string;

        if(!strcmp(token, "load")){
            string = strtok(line, delim);
            load_file(string);
        }else if(!strcmp(token, "show")){
            string = strtok(line, delim);
            display_view(aq__get_view_by_id(&aquarium1, atoi(string)));
        }else if(!strcmp(token, "addview")){
            string = strtok(line, delim);
        }else if(!strcmp(token, "delview")){
            string = strtok(line, delim);
            aqv__remove_aquarium_view(aq__get_view_by_id(&aquarium1, atoi(string)));
        }else if(!strcmp(token, "save")){
            string = strtok(line, delim);
            int id = atoi(string);
            string = strtok(line, delim);
            write_file(string);
        }else{
            printf("\t> Unrecognized command, use 'help' or '?' to list available commands\n");
        }
    }

    return 1;
}

void display_prompt(){
    display_menu();

    while(1){
        line_read = readline(prompt);
        if(!handle_line()){
            break;
        }
    }
}