#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "answer.h"
#include "../model/aquarium.h"
#include "../utility/tools.h"

extern struct aquarium *aquarium;
LIST_HEAD(clientlist, client) clients;

/* Auxiliar functions*/
/**
 * @brief available_id  returns an available view identifier
 * @param wanted        the wanted view identifier, could be NULL
 * @return              wanted                      if available (and not NULL)
 *                      NULL                        if none of the view identifier is available
 *                      an other view identifier    else
 */
struct aquarium_view *available_id(char *wanted) {
        struct client *first_available_id = NULL;
        struct client *client;
        LIST_FOREACH(client, &clients, entries) {
            // The wanted id exists and is available
            if(wanted != NULL)
            {
                if (!strcmp(wanted, client->id) && client->is_free) {
                    client->is_free=0;
                    return client->aqv;
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
            return first_available_id->aqv;
        }
        else
        {
            return NULL;
        }
}


int sprintf_fish(any_t res, any_t fish)
{
    struct fish * cpy = (struct fish *) fish;
    double sec = cpy->_speed_rate;
    char * info_fish = malloc(sizeof(char)*(27 + strlen(cpy->_id)));
    sprintf(info_fish," [%s at %dx%d,%dx%d,%f]",cpy->_id,
            (int)cpy->_current.x, (int)cpy->_current.y,
            cpy->_cover.width, cpy->_cover.height, sec);
    strcat((char *)res,info_fish);
    return MAP_OK;
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
int asw__hello(char *arg, char *res, struct client *cli) {
    struct aquarium_view * id;
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
        sprintf(res, "Invalid syntax for 'hello'. Corrects syntaxes are :\n'hello in as <wanted id>'\n'hello'\n");
        return HELLO_FAILURE;
    }

    // Successful request
    if(id != NULL) {
        cli->id =malloc(sizeof(char)*(strlen(id->_id)+1));
        strcpy(cli->id,id->_id); cli->is_free = 0;
        cli->aqv=id;
        sprintf(res,"greeting %s\n",id->_id);
        return HELLO_SUCCESS;
    }
    // Failed request
    strcpy(res,"no greeting\n");
    return HELLO_FAILURE;
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
void asw__get_fishes(char * arg, char * res, struct client *cli)
{
    if(strcmp(arg,"\n")==0 || strcmp(arg," \n")==0)
    {
        sprintf(res,"list");
        hashmap_iterate(cli->aqv->_fishes, (PFany) sprintf_fish, res);
        strcat(res,"\n");
    }
    else
        strcpy(res,"Invalid syntax for 'getFishesContinuously'. No argument allowed : 'getFishesContinuously\n'\n");
}

/**
 * @brief asw__log
 * @param arg
 * @param res
 * @param cli
 */
void asw__log(char * arg, char * res, struct client *cli)
{
    // Get the view identifier available
    struct client *client;
    LIST_FOREACH(client, &clients, entries) {
       if (!strcmp(cli->id, client->id)) {
            client->is_free=1;
        }
    }
    free(cli->id);
    strcpy(res,"bye\n");
}

/* Functions for the aquarium */
/**
 * @brief asw__init_aquarium initializes the list of the viewsof the aquarium
 * Has to be called by aq__initialize_aquarium();
 */
void asw__init_aquarium()
{
    struct clientlist *clientp;
    LIST_INIT(&clients);
}

/**
 * @brief asw__add_view adds the identifier view to the view identifier list
 * @param id the view identifier of the view added
 * Has to be called by aq__add_view()
 */
void asw__add_view(struct aquarium_view *view)
{
    struct client * cli = malloc(sizeof(struct client));
    cli->id = malloc(sizeof(char) * (strlen(view->_id) + 1));
    strcpy(cli->id, view->_id);
    cli->is_free=1;
    cli->aqv=view;
    LIST_INSERT_HEAD(&clients, cli, entries);
}

/**
 * @brief asw__remove_view removes the identifier view from the view identifier list
 * @param id the view identifier of the view removed
 * Has to be called by aq__remove_aquarium_view();
 */
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

/**
 * @brief asw__remove_aquarium frees the list used to manipulate the aquarium
 * Has to be called by aq__remove_aquarium();
 */
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
int main(int argc, char *argv[]) {
// To test : a false aquarium
    aq__initialize_aquarium(&aquarium, (struct dimension) {1000, 1000});
    aq__add_view(&aquarium, (struct position) {250, 250}, (struct dimension) {500, 500}, "Cookie");
    aq__add_view(&aquarium, (struct position) {100, 100}, (struct dimension) {900, 900}, "Donald");
    // end of the test

    char * res = malloc(sizeof(400));
    struct client *henry = malloc(sizeof(struct client));
    henry->id = NULL;
    asw__hello("\n", res, henry);
    printf("main\t%s", res);
    asw__hello("\n", res, henry);
    printf("main\t%s", res);
    asw__hello("\n", res, henry);
    printf("main\t%s", res);

    // Henry a la vue "Cookie"
    aq__add_fish_to_aqv(&aquarium,"Cookie",fish__create(BLOBFISH, 10, 20, "Bibi",4,5));
    aq__add_fish_to_aqv(&aquarium,"Cookie",fish__create(BLOBFISH, 40, 50, "Bobo",4,5));

    asw__get_fishes("\n",res,henry);
    printf("main\t%s",res);
    return EXIT_SUCCESS;
}
*/
