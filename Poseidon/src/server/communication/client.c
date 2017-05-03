#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <bits/sigthread.h>
#include <bits/signum.h>

#include "client.h"
#include "send.h"
#include "../../utility/vector.h"

struct thread_entry;

static const char *delim = SERVER_CMD_DELIMITER;

void *client__start(void *arg) {

    struct thread_entry *entry = (struct thread_entry *) arg;
    struct thread_p *thread = &entry->_thread;
    int code_return;
    char *response;

    client__init(thread);

    //If something happened during actions that needs to stop everything
    //meaning if the client is still sending messages, and connected
    while (thread->_connected) {
        //We clear entire array
        bzero(thread->_last_message, BUFFER_SIZE);

        code_return = (int) read(thread->_socket_fd, thread->_last_message, BUFFER_SIZE);
        if(code_return == -1){
            //Error reading from socket
            CONSOLE_LOG_ERR("Client from IP %s reset connection", inet_ntoa(thread->_client_socket.sin_addr));
            thread->_connected = FALSE;
        }

        if (thread->_connected) {
            //We update last time client sent a message
            LOCK(&thread->_time_mutex);
            time(&thread->_last_ping);
            UNLOCK(&thread->_time_mutex);
        }

        if (code_return == 2) {
            //fix the segfault in case of empty message
            code_return = (int) write(thread->_socket_fd, "\n", 1);
            CHK_ERROR(code_return, "Error writing to socket")

        } else if (code_return != -1) {
            //We parse command
            response = client__parse_command(thread->_last_message, thread);

            //We write answer
            if (response != NULL) {
                code_return = (int) write(thread->_socket_fd, response, strlen(response));
                CHK_ERROR(code_return, "Error writing to socket");
            }
        }
    }

    client__destroy(entry);
    pthread_exit(NULL);
}

char *client__parse_command(char buffer[BUFFER_SIZE], struct thread_p *thread) {

    char *cmd;
    asprintf(&cmd, "%s", buffer);

    if (strlen(cmd) > 1)
        cmd[strlen(cmd) - 2] = '\0';

    char *token = strtok(cmd, delim);
    if (token == NULL || *token == -1) {
        //The client closed brutally
        thread->_connected = FALSE;
        return "\0";
    }

    if (!strcmp(token, "hello")) {
        if (!thread->_authenticated) {
            return send__client_id(thread);
        } else {
            return "Already authenticated\n";
        }
    } else if (!strcmp(token, "log")) {
        return send__logout(thread);
    } else if (!strcmp(token, "ping")) {
        return send__ping(thread->_client);
    } else if (thread->_client != NULL) {
        if (!strcmp(token, "getFishes")) {
            return send__fishes(thread->_client);
        } else if (!strcmp(token, "getFishesContinuously")) {
            return send__fishes_continuously(thread);
        } else if (!strcmp(token, "addFish")) {
            return send__add_fish(thread->_client);
        } else if (!strcmp(token, "delFish")) {
            return send__delete_fish();
        } else if (!strcmp(token, "startFish")) {
            return send__start_fish();
        } else if(!strcmp(token, "stopSendContinuously")){
            return send__stop_send_continuously(thread);
        }
    } else {
        return "Please authenticate yourself with a `hello` command first\n";
    }

    free(cmd);
    return "Unknown command\n";

}

void client__init(struct thread_p *client_thread) {
    client_thread->_connected = TRUE;
    client_thread->_authenticated = FALSE;
    client_thread->_client = NULL;
    pthread_mutex_init(&client_thread->_time_mutex, NULL);
}

void client__destroy(struct thread_entry *client_thread) {

    LOCK(&threads_list_mutex);
    LIST_REMOVE(client_thread, thread_entries);
    UNLOCK(&threads_list_mutex);

    if (client_thread->_thread._client != NULL) {
        if(client_thread->_thread._client->_is_observer){
            __stop_send_continuously(&client_thread->_thread);
        }
        client_thread->_thread._client->is_free = TRUE;
    }

    client_thread->_thread._client = NULL;
    close(client_thread->_thread._socket_fd);
    pthread_mutex_destroy(&client_thread->_thread._time_mutex);
    free(client_thread);

    CONSOLE_LOG_INFO("Disconnected client from IP %s", inet_ntoa(client_thread->_thread._client_socket.sin_addr))
}
