#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#include "../src/server/parser.h"
#include "../src/server/server.h"
#include "../src/model/aquarium.h"
#include "../src/view/view.h"
#include "../src/server/world/world.h"
#include "../src/server/communication/send.h"

static const char *delim = " ";

// TESTS TO DO
/*
char* send__fishes(struct client* client);
void send__signal_callback_handler(int signum);
void* send__regular_sender(void* arg);
void send__fishes_continuously(struct thread_p* thread);
char* send__add_fish(struct client* client);
char* send__delete_fish();
char* send__logout(struct thread_p* thread);
char* send__ping(struct client* client);
char* send__start_fish();
*/

extern struct _tvector* config_vector;

struct aquarium* aquarium;

pthread_mutex_t mutex_observers;

pthread_t thread_menu;
pthread_t thread_server;
pthread_t thread_world;

int WORLD_READY;
int SERVER_READY;

void tst__send_client_id()
{
    char * res;

    aquarium = malloc(sizeof(struct aquarium));
    aq__initialize_aquarium(aquarium, AQUARIUM_DIMENSIONS);
    aq__add_view(aquarium, (struct position) {250, 250}, (struct dimension) {500, 500}, "Cookie");

    aq__add_view(aquarium, (struct position) {100, 100}, (struct dimension) {900, 900}, "Donald");

    struct thread_p henry;
    henry._client = malloc(sizeof(struct client *));

    /* Valid syntaxes with free identifiers */
    /* hello\n */
    char * cmd = strdup("hello\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Donald\n")==0);
    free(res);

    /* hello \n */
    cmd = strdup("hello    \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Cookie\n")==0);
    CONSOLE_LOG_TEST("\"hello\" with free identifiers");
    free(res);

    /* hello in as available_id\n */
    aq__add_view(aquarium, (struct position) {2, 5}, (struct dimension) {900, 900}, "Mandarine99");
    cmd = strdup("hello in as Mandarine99\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Mandarine99\n")==0);
    free(res);

    /* hello in as available_id \n */
    aq__add_view(aquarium, (struct position) {10, 20}, (struct dimension) {900, 900}, "Post-it");
    aq__add_view(aquarium, (struct position) {20, 30}, (struct dimension) {900, 900}, "Nemo");
    cmd = strdup("hello in as Post-it  \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Post-it\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"hello in as available_id\"");

    /* hello in as already_taken_id\n */
    cmd = strdup("hello in as Mandarine   \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Nemo\n")==0);
    free(res);

    /* hello in as already_taken_id \n */

    aq__add_view(aquarium, (struct position) {30, 40}, (struct dimension) {900, 900}, "Bilboquet");
    cmd = strdup("hello in as Mandarine \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Bilboquet\n")==0);
    free(res);

    CONSOLE_LOG_TEST("\"hello in as already_taken_id\" with free identifiers");

    /* hello in as nawak_id\n */

    aq__add_view(aquarium, (struct position) {40, 50}, (struct dimension) {900, 900}, "Atos");
    cmd = strdup("hello in as Nawak118\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Atos\n")==0);
    free(res);


    /* hello in as nawak_id \n */

    aq__add_view(aquarium, (struct position) {40, 50}, (struct dimension) {900, 900}, "Blanco");
    cmd = strdup("hello in as Nawak412            \n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"greeting Blanco\n")==0);

    CONSOLE_LOG_TEST("\"hello in as not_existing_id\" with free identifiers");

    //All identifiers taken
    cmd = strdup("hello in as Nawak412\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res);

    cmd = strdup("hello in as Nawak412            \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res);

    cmd = strdup("hello\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res);

    cmd = strdup("hello \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res);

    CONSOLE_LOG_TEST("\"hello...\" without free identifiers");

    /* Invalid syntaxes */
    /* hello in\n */
    cmd = strdup("hello in\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"NOK : Please use : hello in as [view id]\n")==0);
    free(res);

    /* hello in \n */
    cmd = strdup("hello in   \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"NOK : Please use : hello in as [view id]\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"hello in\"");

    /* hello in as\n */
    cmd = strdup("hello in as\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"NOK : Please use : hello in as [view id]\n")==0);
    free(res);

    /* hello in as \n */
    cmd = strdup("hello in as   \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"NOK : no spaces allowed in the view id\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"hello in as\"");

    /* hello not_in not_as login \n */
    cmd = strdup("hello in nawak id\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"NOK : Please use : hello in as [view id]\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"hello word word id\" (not valid keywords)");

    /* without \n at the end */
    // Done in client.c while parsing the command
}

void tst__send_fishes()
{
    char * res;

    aquarium = malloc(sizeof(struct aquarium));
    aq__initialize_aquarium(aquarium, AQUARIUM_DIMENSIONS);
    aq__add_view(aquarium, (struct position) {0, 0}, (struct dimension) {500, 500}, "N1");
    aq__add_view(aquarium, (struct position) {0, 500}, (struct dimension) {500, 500}, "N2");
    aq__add_view(aquarium, (struct position) {500, 0}, (struct dimension) {500, 500}, "N3");
    aq__add_view(aquarium, (struct position) {500, 500}, (struct dimension) {500, 500}, "N4");

    struct thread_p henry;
    henry._client = malloc(sizeof(struct client *));

    char * cmd = strdup("hello in as N1\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting N1\n")==0);
    free(res);

    cmd=strdup("getFishes\n");
    strtok(cmd,delim);
    res= send__fishes(henry._client);
    assert(strcmp(res,"list\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"getFishes\" without fishes");

    /* Adding some fishes */
    struct fish *poulpi = fish__create(OCTOPUS,10,10,"Poulpi",HORIZONTAL,(struct dimension) {10,10}, 5.0);
    struct fish *superpoulpi = fish__create(OCTOPUS,100,80,"Super-poulpi",VERTICAL,(struct dimension) {10,10}, 10.0);
    struct fish *reddy = fish__create(REDFISH,800,900,"Reddy",RANDOM,(struct dimension) {10,10}, 1.0);
    aq__add_fish_to_aqv(aquarium,"N1",poulpi);
    aq__add_fish_to_aqv(aquarium,"N1",superpoulpi);
    aq__add_fish_to_aqv(aquarium,"N4",reddy);

    cmd=strdup("getFishes\n");
    strtok(cmd,delim);
    res= send__fishes(henry._client);
    assert(strcmp(res,"list [Poulpi at 2x2,10x10,0] [Super-poulpi at 20x16,10x10,0]\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"getFishes\" with fishes in the view and others elsewhere");

    /* Deleting one fish */
    aq__remove_fish(aquarium,"Poulpi");
    cmd=strdup("getFishes          \n");
    strtok(cmd,delim);
    res= send__fishes(henry._client);
    assert(strcmp(res,"list [Super-poulpi at 20x16,10x10,0]\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"getFishes\" after deleting some fishes");

    /* Invalid syntax : putting arguments */
    cmd=strdup("getFishes   something   arg1    arg2         arg3    \n");
    strtok(cmd,delim);
    res= send__fishes(henry._client);
    assert(strcmp(res,"NOK : no arguments allowed in getFishes\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"getFishes\" after deleting some fishes");
}

int main()
{
  _set_verbosity(FALSE);
  _set_test_verbosity(TRUE);

  // Intern communication
  tst__send_client_id();
  tst__send_fishes();

  return 0;
}
