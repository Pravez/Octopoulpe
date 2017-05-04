#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include "send.h"
#include "../../utility/data.h"
#include "../../utility/vector.h"

static const char *delim = SERVER_CMD_DELIMITER;
static const char *end_delim = SERVER_END_CMD_DELIMITER;
static const char *val_delim = SERVER_FISHVAL_DELIMITER;
extern struct vector *observers;



char *send__client_id(struct thread_p *thread) {
    char *result = NULL;

    char *in = strtok(NULL, delim);
    char *as = strtok(NULL, delim);
    char *id = strtok(NULL, end_delim);
    char *str = NULL;
    if (in != NULL && strcmp(in, "in") == 0 && as != NULL && strcmp(as, "as") == 0 && id != NULL) {
        asprintf(&str, "%s", id);
    } else if (in !=NULL && strcmp(in,"\n")!= 0) {
        asprintf(&result, "no greeting\n");
        return result;
    }

    if (asw__hello(str, &result, thread) == HELLO_FAILURE) {
        thread->_authenticated = FALSE;
    } else {
        if (thread->_client->id != NULL) {
            CONSOLE_LOG_INFO("Authenticated Client %s with View %s", inet_ntoa(thread->_client_socket.sin_addr),
                             thread->_client->id);
            asprintf(&thread->_client_name, "%s", thread->_client->id);
        }
    }

    free(str);
    return result;
}

char *send__fishes(struct client *client) {
    char *end_line = strtok(NULL, end_delim);
    char *arg = strtok(end_line, delim);
    char *result = NULL;

    /* Skiping the spaces */
    if (arg != NULL) {
        asprintf(&result, "NOK\n");
    } else {
        asw__get_fishes(&result, client);
    }
    return result;
}

void send__signal_callback_handler(int signum) {
    if (signum == SIGNAL_STOP_SENDING) {
        pthread_exit(NULL);
    }
}

void *send__regular_sender(void *arg) {
    struct thread_p *thread = (struct thread_p *) arg;
    signal(SIGNAL_STOP_SENDING, send__signal_callback_handler);
    signal(SIGNAL_NOTIFICATION, send__signal_callback_handler);

    while (1) {
        pause();
        LOCK(&thread->_time_mutex);
        time(&thread->_last_ping);
        UNLOCK(&thread->_time_mutex);

        char *fishes = send__fishes(thread->_client);
        CHK_ERROR(write(thread->_socket_fd, fishes, strlen(fishes)), "Error writing to socket");
        free(fishes);
    }
}

void send__fishes_continuously(struct thread_p *thread) {
    if (!thread->_client->_is_observer) {
        CONSOLE_LOG_INFO("Starting to send continuously to %s", thread->_client->id);
        thread->_client->_is_observer = TRUE;
        v__add(observers, thread, THREAD);
        pthread_create(&thread->_client->_continuous_sender, NULL, send__regular_sender, thread);
    }
}

char *send__add_fish(struct client *client) {

    const char *coma = ",";

    char *result = NULL;

    char *id = strtok(NULL, delim); printf("id : \t'%s'\n",id);
    char *at = strtok(NULL, delim); printf("at : \t'%s'\n",at);
    char *tmp_rel_pos = strtok(NULL, coma);
    char *tmp_size = strtok(NULL, coma);
    char *tmp_strategy = strtok(NULL, end_delim);

    char * rel_pos = strtok(tmp_rel_pos, delim); printf("rel_pos : \t'%s'\n",rel_pos);
    char *size = strtok(tmp_size, delim); printf("size : \t'%s'\n",size);
    char *strategy = strtok(tmp_strategy, delim); printf("strategy : '%s'\n",strategy);
    char *suparg = strtok(NULL, delim); printf("suparg : \t'%s'\n",suparg);

    if(client == NULL || suparg != NULL || id == NULL || at == NULL || rel_pos == NULL || size == NULL || strategy == NULL){
        asprintf(&result,"NOK\n");
        return result;
    }

    if (rel_pos[strlen(rel_pos) - 1] == ',')
        rel_pos[strlen(rel_pos) - 1] = '\0';
    if (size[strlen(size) - 1] == ',')
        size[strlen(size) - 1] = '\0';

    struct relative_position pos;
    struct dimension dimensions;

    pos.x = atoi(strtok(rel_pos, val_delim));
    pos.y = atoi(strtok(NULL, val_delim));

    dimensions.width = atoi(strtok(size, val_delim));
    dimensions.height = atoi(strtok(NULL, val_delim));

    //Ignoring fish type
    asw__add_fish(id, pos, dimensions, NULL, strategy, &result, client);
    return result;
}

char *send__logout(struct thread_p *thread) {
    char *end_line = strtok(NULL, end_delim);
    char *out = strtok(end_line, delim);
    char *arg = strtok(NULL, delim);
    if (arg == NULL)
        return asw__log(out, thread);
    else {
        return "NOK\n";
    }
}

char *send__delete_fish() {
    char *result;
    char *tmp_id= strtok(NULL, end_delim);
    char * id = strtok(tmp_id,delim);
    char *suparg = strtok(NULL, delim);
    if (id != NULL && strtok(NULL, delim) == NULL && suparg == NULL) {
        asw__del_fish(id, &result);
    } else {
        asprintf(&result, "NOK\n");
    }
    return result;
}

char *send__ping(struct client *client) {
    char *value = strtok(NULL, end_delim);
    char *result = NULL;
    asw__ping(value, &result, client);
    return result;
}

char *send__start_fish() {
    char *result;

    char *id = strtok(NULL, delim);
    if (id != NULL && strtok(NULL, delim) == NULL) {
        asw__start_fish(id, &result);
    } else {
        asprintf(&result, "NOK\n");
    }

    return result;
}

void send__stop_send_continuously(struct thread_p *thread) {
    if (thread->_client->_is_observer) {
        __stop_send_continuously(thread);
    }
}

void __stop_send_continuously(struct thread_p *thread) {
    pthread_kill(thread->_client->_continuous_sender, SIGNAL_STOP_SENDING);
    pthread_join(thread->_client->_continuous_sender, NULL);

    CONSOLE_LOG_INFO("Stopping to send continuously to %s", thread->_client->id);

    thread->_client->_is_observer = FALSE;
    v__remove_by_data(observers, thread);
}
