#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "send.h"
#include "../utility/data.h"
#include "../utility/vector.h"

static const char *delim = SERVER_CMD_DELIMITER;
extern struct vector* observers;

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

void send__signal_callback_handler(int signum){
    if(signum == SIGNAL_STOP_SENDING){
        pthread_exit(NULL);
    }
}

void* send__regular_sender(void* arg){
    struct client* client = (struct client*)arg;
    signal(SIGNAL_STOP_SENDING, send__signal_callback_handler);
    signal(SIGNAL_NOTIFICATION, send__signal_callback_handler);

    while(1){
        pause();
        char* fishes = send__fishes(client);
        CHK_ERROR(write(client->_socket_fd, fishes, strlen(fishes)), "Error writing to socket");
        free(fishes);
    }
}

void send__fishes_continuously(struct client* client) {
    client->_is_observer = TRUE;
    v__add(observers, client, CLIENT);
    pthread_create(&client->_continuous_sender, NULL, send__regular_sender, client);

    read(client->_socket_fd, NULL, 256-1);
    pthread_kill(client->_continuous_sender, SIGNAL_STOP_SENDING);
    pthread_join(client->_continuous_sender, NULL);

    printf("END OF SENDING\n");

    client->_is_observer = FALSE;
    v__remove_by_data(observers, client);
}

char* send__add_fish(struct client* client){
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

