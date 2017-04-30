#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "client.h"
#include "send.h"

struct thread_entry;

static const char *delim = SERVER_CMD_DELIMITER;


void *client__start(void *arg) {

    struct thread_entry *entry = (struct thread_entry *) arg;
    struct thread_p* thread = &entry->_thread;
    int code_return;
    char *response;

    client__init(thread);

    //If something happened during actions that needs to stop everything
    //meaning if the client is still sending messages, and connected
    while (thread->_thread_working && thread->_client._connected) {
        //We clear entire array
        bzero(thread->_last_message, BUFFER_SIZE);

        code_return = (int) read(thread->_client._socket_fd, thread->_last_message, BUFFER_SIZE - 1);
        CHK_ERROR(code_return, "Error reading from socket")

        if(thread->_thread_working) {
            //We update last time client sent a message
            LOCK(&thread->_client._time_mutex);
            time(&thread->_client._last_ping);
            UNLOCK(&thread->_client._time_mutex);
        }

        if (code_return == 2) {
            //fix the segfault in case of empty message
            code_return = (int) write(thread->_client._socket_fd, "\n", 1);
            CHK_ERROR(code_return, "Error writing to socket")

        } else {
            //We parse command
            response = client__parse_command(thread->_last_message, &thread->_client);

            //We write answer
            if(response != NULL){
                code_return = (int) write(thread->_client._socket_fd, response, strlen(response));
                CHK_ERROR(code_return, "Error writing to socket");
            }
        }
    }

    client__destroy(entry);
    pthread_exit(NULL);
}

char *client__parse_command(char buffer[BUFFER_SIZE], struct client *client) {

    char *cmd;
    asprintf(&cmd, "%s", buffer);

    if(strlen(cmd) > 1)
        cmd[strlen(cmd) - 2] = '\0';

    char *token = strtok(cmd, delim);
    if(token == NULL){
        //The client closed brutally
        client->_connected = FALSE;
        return "\0";
    }

    if (!strcmp(token, "hello")) {
        if(client->id == NULL){
            return send__client_id(client);
        }else{
            return "Already authenticated\n";
        }
    }else if(client->id != NULL){
        if (!strcmp(token, "getFishes")) {
            return send__fishes(client);
        } else if (!strcmp(token, "getFishesContinuously")) {
            send__fishes_continuously(client);
            return "End of transaction";
        } else if (!strcmp(token, "log")) {
            return send__logout(client);
        } else if (!strcmp(token, "ping")) {
            return send__ping(client);
        } else if (!strcmp(token, "addFish")) {
            return send__add_fish(client);
        } else if (!strcmp(token, "delFish")) {
            return send__delete_fish();
        } else if (!strcmp(token, "startFish")) {
            return send__start_fish();
        }
    } else{
        return "Please authenticate yourself with a `hello` command first\n";
    }

    free(cmd);
    return "Unknown command\n";

}

void client__init(struct thread_p* client_thread){
    time(&client_thread->_client._last_ping);
    client_thread->_client._is_observer = FALSE;
    client_thread->_client._connected = TRUE;
    client_thread->_thread_working = TRUE;
    pthread_mutex_init(&client_thread->_client._time_mutex, NULL);
}

void client__destroy(struct thread_entry* client_thread){
    close(client_thread->_thread._client._socket_fd);
    pthread_mutex_destroy(&client_thread->_thread._client._time_mutex);
    free(client_thread->_thread._client.id);
    //free(client_thread);
}