#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "answer.h"
#include "../model/aquarium.h"

#include "../utility/tools.h"

extern struct aquarium *aquarium1;
LIST_HEAD(clientlist, client) clients;

char *available_id(char *wanted) {
        printf("wanted : #%s#\n",wanted);
        struct client *first_available_id = NULL;
        struct client *client;
        LIST_FOREACH(client, &clients, entries) {
            // The wanted id exists and is available
            if(wanted != NULL)
            {
                if (!strcmp(wanted, client->id) && client->is_free) {
                    client->is_free=0;
                    char res[100] = "greeting ";
                    return strcat(strcat(res,wanted),"\n");
                }
            }

            // Use the first available id
            if (client->is_free && first_available_id == NULL)
            {
                first_available_id = client;
            }


        }


        // The wanted id was not in the aquarium or not available but one id is available
        if(first_available_id != NULL)
        {
            first_available_id->is_free=0;
            char res[100] = "greeting ";
            return strcat(strcat(res,first_available_id->id),"\n");
        }
        else
        {
            return "no greeting\n";
        }
}

int asw__hello(char *arg, char *res, struct client *cli) {
    char * id;
    char * argv[4];
    char * cpy = malloc(100*4); // Copy of arg needed, else segfault with strtok
    strcpy(cpy,arg);

    argv[0] = strtok(cpy," "); // in
    argv[1] = strtok(NULL," "); // as
    argv[2] = strtok(NULL,"\n"); // wanted id

    free(cpy);

    // Seeking for an available id
    if((!strcmp(argv[0],"in") && !strcmp(argv[1],"as") && (argv[2]!=NULL) && strcmp(argv[2]," ")) || !strcmp(argv[0],"\n") || !strcmp(argv[0]," \n"))
    {
        id = available_id(strtok(argv[2]," "));
    }

    // Incorrect syntax for the command
    else
    {
        strcpy(res, "Invalid syntax for 'hello'. Corrects syntaxes are :\n'hello in as <wanted id>'\n'hello'\n");
        return HELLO_FAILURE;
    }

    strcpy(res, id);
    // Successful request
    if(strcmp(id,"no greeting\n")) {
        cli->id =malloc(sizeof(char)*(strlen(id)+1));
        strcpy(cli->id,id); cli->is_free = 0;
        return HELLO_SUCCESS;
    }
    // Failed request
    return HELLO_FAILURE;
}

/* Functions for the aquarium */
void asw__init_aquarium()
{
    printf("init aquarium\n");
    struct clientlist *clientp;
    LIST_INIT(&clients);
}

void asw__add_view(char *id)
{
    printf("add view: %s\n",id);
    struct client * cli = malloc(sizeof(struct client));
    cli->id = malloc(sizeof(char) * (strlen(id) + 1));
    strcpy(cli->id, id);
    cli->is_free=1;
    LIST_INSERT_HEAD(&clients, cli, entries);
}

void asw__remove_view(char *id)
{
    struct client *j;
    LIST_FOREACH(j, &clients, entries) {
        if(!strcmp(j->id,id))
        {
            LIST_REMOVE(j,entries);
            free(j->id);
            free(j);
            return;
        }
    }
}

void asw__remove_aquarium()
{
    struct client *client;
    LIST_FOREACH(client, &clients, entries) {
        LIST_REMOVE(client, entries);
        free(client->id);
        free(client);
    }
}

/*
#ifndef _PROCESS_
int main(int argc, char *argv[]) {
    // To test : a false aquarium1
    aq__initialize_aquarium(&aquarium1, (struct dimension) {1000, 1000});
    aq__add_view(&aquarium1, (struct position) {250, 250}, (struct dimension) {500, 500}, "Cookie");
    aq__add_view(&aquarium1, (struct position) {100, 100}, (struct dimension) {900, 900}, "Donald");
    // end of the test

    char res[256];
    struct client *henry = malloc(sizeof(struct client));
    henry->id = NULL;
    asw__hello("in as Cookie\n", res, henry);
    printf("######### res #########\n%s", res);
    return 0;
}
#endif
*/
