#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "answer.h"
#include "../model/aquarium.h"

extern struct aquarium *aquarium;

/* Auxiliar functions*/
/**
 * @brief available_id  returns an available view identifier
 * @param wanted        the wanted view identifier, could be NULL
 * @return              wanted                      if available (and not NULL)
 *                      NULL                        if none of the view identifier is available
 *                      an other view identifier    else
 */
struct client* available_id(char *wanted) {
    struct client *first_available_id = NULL;
    struct client *client;
    LIST_FOREACH(client, &clients, entries) {
        // The wanted id exists and is available
        if (wanted != NULL) {
            if (!strcmp(wanted, client->id) && client->is_free) {
                return client;
            }
        }
        // Use the first available id
        if (client->is_free && first_available_id == NULL) {
            first_available_id = client;
        }
    }
    // The wanted id was not in the aquarium or not available but one id is available
    if (first_available_id != NULL) {
        return first_available_id;
    } else {
        return NULL;
    }
}

/* Functions for the server thread */
/**
 * @brief asw__hello    the handler for the "hello" command from the client
 * @param arg           the end of the command which represents the arguments, should finish with '\n'
 * @param res           the string used to communicate the answer
 *                      "greeting <VIEW IDENTIFIER>\n"  with an available view identifier
 *                      "no greeting\n"                 if none of the view identifier was available
 *                      The command syntax            if the arguments were incorrect
 * @param cli           the client who has made the command
 * @return              HELLO_SUCCESS       if a view identifier was attributed to the client
 *                      HELLO_FAILURE       else
 */
int asw__hello(char *arg, char **res, struct thread_p *thread) {
    struct client *id;

    if (arg != NULL) {
        char *argv[4];
        char *cpy;
        asprintf(&cpy, "%s", arg);

        argv[0] = strtok(cpy, " "); // in
        argv[1] = strtok(NULL, " "); // as
        argv[2] = strtok(NULL, " "); // wanted id

        // Seeking for an available id
        if ((!strcmp(argv[0], "in") && !strcmp(argv[1], "as") && (argv[2] != NULL) && strcmp(argv[2], " ")) ||
            !strcmp(argv[0], "\n") || !strcmp(argv[0], " \n")) {
            id = available_id(strtok(argv[2], " "));
        } else {
            asprintf(res, "Invalid syntax for 'hello'. Corrects syntaxes are :\n'hello in as <wanted id>'\n'hello'\n");
            return HELLO_INVALID;
        }

        free(cpy);

    } else {
        id = available_id(NULL);
    }

    // Successful request
    if (id != NULL) {
        thread->_client = id;
        thread->_client->is_free = FALSE;
        thread->_authenticated = TRUE;
        asprintf(res, "greeting %s\n", thread->_client->id);
        return HELLO_SUCCESS;
    }
    // Failed request
    asprintf(res, "> no greeting\n");
    return HELLO_FAILURE;
}

int asw__iterate_fishes(any_t *res, any_t fish) {
    struct fish *ffish = (struct fish *) fish;
    char *fishes_str = *(char **) res;
    char *temp_str = NULL;

    if (fishes_str != NULL) {
        temp_str = malloc(sizeof(char) * strlen(fishes_str));
        strcpy(temp_str, fishes_str);
    }

    struct relative_position fish_pos = aq__get_relative_pos(aquarium, ffish);
    //double lasting_time =

    char *new_str;
    asprintf(&new_str, "%s [%s at %dx%d,%dx%d,%d]", temp_str == NULL ? "" : temp_str, get_type_string(ffish->_type),
             fish_pos.x, fish_pos.y,
             ffish->_cover.width, ffish->_cover.height, (unsigned int) ffish->_speed_rate);

    free(fishes_str);
    free(temp_str);
    *res = new_str;
    return MAP_OK;
}

/**
 * @brief asw__get_fishes   the handler for the "hello" command from the client
 * @param arg               the end of the command which represents the arguments, should finish with '\n'
 * @param res               the string used to communicate the answer
 *                          "list [<FISH_NAME> at <POS%_X>x<POS%_Y>,<FISH_WIDTH>x<FISH_HEIGHT>,<SEC>] [...]\n"
 *                          FISH_NAME   the name of the fish (an identifier)
 *                          POS%_X      the new horizontal position in percentage of the screen width
 *                          POS%_Y      the new vertical position in percentage of the screen height
 *                          FISH_WIDTH  the width of the picture used to show the fish on the screen
 *                          FISH_HEIGHT the height of the picture used to show the fish on the screen
 *                          SEC         how many seconds has to last the move from the current position to the new one
 *                                      if SEC equals 0, the fish is shown immediately
 */
void asw__get_fishes(char **res, struct client *cli) {
    if (cli != NULL && cli->aqv != NULL) {
        if (hashmap_length(cli->aqv->_fishes) == 0) {
            asprintf(res, "list []\n");
        } else {
            char *fishes_str = NULL;
            hashmap_iterate(cli->aqv->_fishes, (PFany) asw__iterate_fishes, &fishes_str);
            asprintf(res, "list%s\n", fishes_str);
            free(fishes_str);
        }
    }
}


/**
 * @brief asw__log frees the ressources allocated to manipulate the client informations
 * @param arg should be "out\n" or "out \n"
 * @param res the string "bye\n"
 * @param cli the client structure
 * @return LOGOUT_SUCCESS on success and LOGOUT_FAILURE on fail
 */
char *asw__log(char *arg, struct thread_p *thread) {
    if (arg == NULL || strcmp(arg, "out")) {
        return "Maybe you wanted to say `log out`\n";
    } else {
        if(thread->_authenticated == TRUE && thread->_client != NULL)
            thread->_client->is_free = TRUE;

        thread->_authenticated = FALSE;
        thread->_connected = FALSE;
        return "bye\n";
    }
}

/* Functions for the aquarium */
/**
 * @brief asw__init_aquarium initializes the list of the viewsof the aquarium
 * Has to be called by aq__initialize_aquarium();
 */
void asw__init_aquarium() {
    struct clientlist *clientp;
    LIST_INIT(&clients);
}

/**
 * @brief asw__add_view adds the identifier view to the view identifier list
 * @param id the view identifier of the view added
 * Has to be called by aq__add_view()
 */
void asw__add_view(struct aquarium_view *view) {
    struct client *cli = malloc(sizeof(struct client));
    cli->id = malloc(sizeof(char) * (strlen(view->_id) + 1));
    strcpy(cli->id, view->_id);
    cli->is_free = 1;
    cli->aqv = view;
    cli->_is_observer = FALSE;
    LIST_INSERT_HEAD(&clients, cli, entries);
}

/**
 * @brief asw__remove_view removes the identifier view from the view identifier list
 * @param id the view identifier of the view removed
 * Has to be called by aq__remove_aquarium_view();
 */
void asw__remove_view(char *id) {
    struct client *j;
    LIST_FOREACH(j, &clients, entries) {
        if (!strcmp(j->id, id)) {
            LIST_REMOVE(j, entries);
            free(j->id);
            free(j);
            return;
        }
    }
}

/**
 * @brief asw__remove_aquarium frees the list used to manipulate the aquarium
 * Has to be called by aq__remove_aquarium();
 */
void asw__remove_aquarium() {
    struct client *client;
    LIST_FOREACH(client, &clients, entries) {
        LIST_REMOVE(client, entries);
        free(client->id);
        free(client);
    }
}

void asw__ping(char *arg, char **res, struct client *client) {
    if (arg != NULL) {
        asprintf(res, "pong %s", arg);
    } else {
        asprintf(res, "> Please give a value with ping");
    }
}

void
asw__add_fish(char *id, struct relative_position pos, struct dimension dimension, char *fish_type, char *strategy,
              char **res,
              struct client *cli) {
    enum fish_type type = get_type_from_string(fish_type);
    enum MOVING_STRATEGY mv_strategy = get_strategy_from_string(strategy);

    if (type == NONE || mv_strategy == UNREGISTERED) {
        asprintf(res, "%s%s", type == NONE ? "> Fish type is not known\n" : "",
                 mv_strategy == UNREGISTERED ? "> Moving strategy is not registered\n" : "");
        return;
    }

    struct position real_position;
    real_position.x = cli->aqv->_inner._starting_position.x +
                      (cli->aqv->_inner._dimensions.width * pos.x) / 100;
    real_position.y = cli->aqv->_inner._starting_position.y +
                      (cli->aqv->_inner._dimensions.height * pos.y) / 100;

    int dimcond = dimension.width > FISH_MAX_WIDTH || dimension.height > FISH_MAX_HEIGHT;
    int poscond = real_position.x + dimension.width > AQUARIUM_WIDTH
                  || real_position.y + dimension.height > AQUARIUM_HEIGHT;

    if (dimcond || poscond) {
        char *dimstr = NULL;
        char *posstr = NULL;

        if (dimcond)
            asprintf(&dimstr, "> Please verify the fish's dimensions, cannot exceed %dx%d\n", FISH_MAX_WIDTH,
                     FISH_MAX_HEIGHT);
        if (poscond)
            asprintf(&posstr,
                     "> Please check the position of your fish, cannot exceed %dx%d (considering size of the fish)\n",
                     AQUARIUM_WIDTH, AQUARIUM_HEIGHT);

        asprintf(res, "%s%s", dimstr == NULL ? "" : dimstr, posstr == NULL ? "" : posstr);
        if(dimstr != NULL)
            free(dimstr);
        if(posstr != NULL)
            free(posstr);

        return;
    }

    struct fish *fish = fish__create(type, (int) real_position.x, (int) real_position.y, id, mv_strategy, dimension,
                                     UPDATE_INTERVAL);
    aq__add_fish_to_aqv(aquarium, cli->aqv->_id, fish);
    asprintf(res, "Fish successfully added\n");
}

void asw__start_fish(char *arg, char **res){
    if(aq__set_fish_running_state(aquarium, arg, 1) == -1){
        asprintf(res, "> Impossible to find fish %s\n", arg);
    }else{
        asprintf(res, "Fish %s started !\n", arg);
    }
}

void asw__del_fish(char *arg, char **res){
    if(aq__remove_fish(aquarium, arg) == 0){
        asprintf(res, "> Impossible to find fish %s\n", arg);
    }else{
        asprintf(res, "Fish %s removed !\n", arg);
    }
}