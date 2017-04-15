#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "answer.h"
#include "../model/aquarium.h"

extern struct aquarium *aquarium1;
LIST_HEAD(clientlist, client) clients;

char *available_id(char *wanted) {
    // Cheking if the wanted id exists in the aquarium
    if (aq__check_free_id(aquarium1, wanted)) {

        // Checking whether the id is already taken or not
        struct client *client;
        LIST_FOREACH(client, &clients, entries) {
            if (!strcmp(wanted, client->id)) {
                break;
            }
        }
        if (client == NULL)
            return wanted;
    }
    // Give a free id from the aquarium
    struct array ids = aq__get_views_ids(aquarium1);
    // TO DO
    return "Youpi";
}

void init() {
    struct clientlist *clientp;
    LIST_INIT(&clients);
}

int asw__hello(char *arg, char *res, struct client *cli) {
    init();
    char cmd[256];
    char *id;
    strcpy(cmd, "greeting ");
    if (!strcmp(arg, "\n")) {
        id = available_id(NULL);
        strcat(strcat(cmd, id), "\n");
    } else {
        char str[strlen(arg)];
        strcpy(str, arg);
        // lecture du "in"
        if (strcmp(strtok(str, " "), "in") == 0) {
            if (strcmp(strtok(NULL, " "), "as") == 0) {
                id = strtok(NULL, " ");
                if (id[strlen(id) - 1] == '\n')
                    id[strlen(id) - 1] = '\0';
                id = available_id(strtok(NULL, " "));
                strcat(strcat(cmd, id), "\n");
            } else {
                strcpy(res, "no greeting\n");
                return HELLO_FAILURE;
            }
        } else {
            strcpy(res, "no greeting\n");
            return HELLO_FAILURE;
        }
    }
    // Successful request
    strcpy(res, cmd);
    cli->id = malloc(sizeof(char) * (strlen(res) + 1));
    strcpy(cli->id, res);
    LIST_INSERT_HEAD(&clients, cli, entries);
    return HELLO_SUCCESS;
}

#ifndef _PROCESS_
int main(int argc, char *argv[]) {
    char res[256];
    struct client *henry = malloc(sizeof(struct client));
    henry->id = NULL;
    asw__hello("\n", res, henry);
    printf("%s", res);
    return 0;
}
#endif
