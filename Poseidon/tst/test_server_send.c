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

/*
char *send__client_id(struct thread_p *thread);
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
    aq__add_view(aquarium, (struct position) {2, 5}, (struct dimension) {900, 900}, "Mandarine");
    cmd = strdup("hello in as Mandarine\n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"greeting Mandarine\n")==0);

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
    /*
    aq__add_view(aquarium, (struct position) {30, 40}, (struct dimension) {900, 900}, "Bilboquet");
    cmd = strdup("hello in as Mandarine \n");
    strtok(cmd,delim);
    assert(strcmp(send__client_id(&henry),"greeting Bilboquet\n")==0);
    */
    CONSOLE_LOG_TEST("Reception of \"hello in as already_taken_id\" with free identifiers");

    /* hello in as nawak_id\n */
    /* hello in as nawak_id \n */

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
    assert(strcmp(send__client_id(&henry),"NOK : Please use : hello in as [view id]\n")==0);
    CONSOLE_LOG_TEST("Reception of \"hello in as\"");

    /* without \n at the end */
}

int main()
{
  _set_verbosity(FALSE);
  _set_test_verbosity(TRUE);
  tst__send_client_id();
  return 0;
}
