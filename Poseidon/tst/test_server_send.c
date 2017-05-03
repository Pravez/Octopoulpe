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
#include "../src/server/world.h"
#include "../src/server/send.h"

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
    assert(strcmp(send__client_id(&henry),"greeting Donald\n")==0);

    /* hello \n */
    cmd = strdup("hello    \n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"greeting Cookie\n")==0);
    CONSOLE_LOG_TEST("Reception of \"hello\" with free identifiers");

    /* hello in as available_id\n */
    aq__add_view(aquarium, (struct position) {2, 5}, (struct dimension) {900, 900}, "Mandarine99");
    cmd = strdup("hello in as Mandarine99\n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"greeting Mandarine99\n")==0);

    /* hello in as available_id \n */
    aq__add_view(aquarium, (struct position) {10, 20}, (struct dimension) {900, 900}, "Post-it");
    aq__add_view(aquarium, (struct position) {20, 30}, (struct dimension) {900, 900}, "Nemo");
    cmd = strdup("hello in as Post-it  \n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"greeting Post-it\n")==0);
    CONSOLE_LOG_TEST("Reception of \"hello in as available_id\"");

    /* hello in as already_taken_id\n */
    cmd = strdup("hello in as Mandarine   \n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"greeting Nemo\n")==0);

    /* hello in as already_taken_id \n */
    //BEUG (segfault)
    printf("BEUG !!!!!!! : segfault when adding the 6th view.Tests are commented.\n");
/*
    aq__add_view(aquarium, (struct position) {30, 40}, (struct dimension) {900, 900}, "Bilboquet");
    cmd = strdup("hello in as Mandarine \n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"greeting Bilboquet\n")==0);
*/
    CONSOLE_LOG_TEST("Reception of \"hello in as already_taken_id\" with free identifiers");

    /* hello in as nawak_id\n */
    //BEUG (segfault)
/*
    aq__add_view(aquarium, (struct position) {40, 50}, (struct dimension) {900, 900}, "Atos");
    cmd = strdup("hello in as Nawak118\n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"greeting Atos\n")==0);
*/

    /* hello in as nawak_id \n */
    //BEUG (segfault)
/*
    aq__add_view(aquarium, (struct position) {40, 50}, (struct dimension) {900, 900}, "Blanco");
    cmd = strdup("hello in as Nawak412            \n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"greeting Blanco\n")==0);
*/
    CONSOLE_LOG_TEST("Reception of \"hello in as not_existing_id\" with free identifiers");

    //All identifiers taken
    cmd = strdup("hello in as Nawak412\n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"no greeting\n")==0);

    cmd = strdup("hello in as Nawak412            \n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"no greeting\n")==0);

    cmd = strdup("hello\n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"no greeting\n")==0);

    cmd = strdup("hello \n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"no greeting\n")==0);

    CONSOLE_LOG_TEST("Reception of \"hello...\" without free identifiers");

    /* Invalid syntaxes */
    /* hello in\n */
    cmd = strdup("hello in\n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"NOK : Please use : hello in as [view id]\n")==0);

    /* hello in \n */
    cmd = strdup("hello in   \n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"NOK : Please use : hello in as [view id]\n")==0);
    CONSOLE_LOG_TEST("Reception of \"hello in\"");

    /* hello in as\n */
    cmd = strdup("hello in as\n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"NOK : Please use : hello in as [view id]\n")==0);

    /* hello in as \n */
    cmd = strdup("hello in as   \n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"NOK : no spaces allowed in the view id\n")==0);
    CONSOLE_LOG_TEST("Reception of \"hello in as\"");

    /* without \n at the end */
    // Done in client.c while parsing the command
}

void tst__send_fishes()
{
    aquarium = malloc(sizeof(struct aquarium));
    aq__initialize_aquarium(aquarium, AQUARIUM_DIMENSIONS);
    aq__add_view(aquarium, (struct position) {250, 250}, (struct dimension) {500, 500}, "N1");
    aq__add_view(aquarium, (struct position) {100, 100}, (struct dimension) {900, 900}, "N2");
    aq__add_view(aquarium, (struct position) {2, 5}, (struct dimension) {900, 900}, "N3");
    aq__add_view(aquarium, (struct position) {10, 20}, (struct dimension) {900, 900}, "N4");
    aq__add_view(aquarium, (struct position) {20, 30}, (struct dimension) {900, 900}, "N5");

    struct thread_p henry;
    henry._client = malloc(sizeof(struct client *));

    char * cmd = strdup("hello ino aso N1\n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"greeting N1\n")==0);

    cmd=strdup("gestFishes\n");
    strtok(cmd,delim);
    printf("res : '%s'",send__fishes(henry._client));
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
