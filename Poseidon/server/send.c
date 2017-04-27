#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "send.h"
#include "../utility/data.h"


static const char *delim = SERVER_CMD_DELIMITER;

char *send__client_id(struct client *client) {
    char *result = NULL;

    char *in = strtok(NULL, delim);
    char *as = strtok(NULL, delim);
    char *id = strtok(NULL, delim);
    char *str = NULL;
    if (in != NULL)
        asprintf(&str, "%s %s %s", in, as, id);

    if(asw__hello(str, &result, client) == HELLO_FAILURE){
        client->_connected = FALSE;
    }

    free(str);
    return result;
}

char *send__fishes(struct client *client) {
    char *result = NULL;
    asw__get_fishes(&result, client);
    return result;
}

//for testing time = 0
char *send__fishes_continuously(struct client *client, void *time) {
    /*int delay = (int) time;
    char *res;
    while (1) {
        sleep(delay);
        asw__get_fishes(NULL, res, NULL);
        //lock ?
        write(com[1], res, strlen(res));
        //unlock
    }*/
}

char *send__add_fish(struct client *client) {
    return NULL;
    //asw__add_fish()
}

char *send__logout(struct client *client) {
    char *out = strtok(NULL, delim);
    return asw__log(out, client);
}

char *send__delete_fish(struct client *client) {
    return NULL;
}

char *send__ping(struct client *client) {
    return NULL;
}

char *send__start_fish(struct client *client) {
    return NULL;
}

