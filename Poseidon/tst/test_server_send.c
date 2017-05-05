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
void send__signal_callback_handler(int signum);
void* send__regular_sender(void* arg);
void send__fishes_continuously(struct thread_p* thread);
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
    free(res); free(cmd);

    /* hello \n */
    cmd = strdup("hello    \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Cookie\n")==0);
    CONSOLE_LOG_TEST("\"hello\" with free identifiers");
    free(res); free(cmd);

    /* hello in as available_id\n */
    aq__add_view(aquarium, (struct position) {2, 5}, (struct dimension) {900, 900}, "Mandarine99");
    cmd = strdup("hello in as Mandarine99\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Mandarine99\n")==0);
    free(res); free(cmd);

    /* hello in as available_id \n */
    aq__add_view(aquarium, (struct position) {10, 20}, (struct dimension) {900, 900}, "Post-it");
    aq__add_view(aquarium, (struct position) {20, 30}, (struct dimension) {900, 900}, "Nemo");
    cmd = strdup("hello in as Post-it  \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Post-it\n")==0);
    free(res); free(cmd);
    CONSOLE_LOG_TEST("\"hello in as available_id\"");

    /* hello in as already_taken_id\n */
    cmd = strdup("hello in as Mandarine   \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Nemo\n")==0);
    free(res); free(cmd);

    /* hello in as already_taken_id \n */

    aq__add_view(aquarium, (struct position) {30, 40}, (struct dimension) {900, 900}, "Bilboquet");
    cmd = strdup("hello in as Mandarine \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Bilboquet\n")==0);
    free(res); free(cmd);

    CONSOLE_LOG_TEST("\"hello in as already_taken_id\" with free identifiers");

    /* hello in as nawak_id\n */

    aq__add_view(aquarium, (struct position) {40, 50}, (struct dimension) {900, 900}, "Atos");
    cmd = strdup("hello in as Nawak118\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"greeting Atos\n")==0);
    free(res); free(cmd);


    /* hello in as nawak_id \n */

    aq__add_view(aquarium, (struct position) {40, 50}, (struct dimension) {900, 900}, "Blanco");
    cmd = strdup("hello in as Nawak412            \n");
    strtok(cmd,delim);
    res = send__client_id(&henry);
    assert(strcmp(res,"greeting Blanco\n")==0);
    free(res); free(cmd);

    CONSOLE_LOG_TEST("\"hello in as not_existing_id\" with free identifiers");

    //All identifiers taken
    cmd = strdup("hello in as Nawak412\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res); free(cmd);

    cmd = strdup("hello in as Nawak412            \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res); free(cmd);

    cmd = strdup("hello\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res); free(cmd);

    cmd = strdup("hello \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res); free(cmd);

    CONSOLE_LOG_TEST("\"hello...\" without free identifiers");

    /* Invalid syntaxes */
    /* hello in\n */
    cmd = strdup("hello in\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res); free(cmd);

    /* hello in \n */
    cmd = strdup("hello in   \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res); free(cmd);
    CONSOLE_LOG_TEST("\"hello in\"");

    /* hello in as\n */
    cmd = strdup("hello in as\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res); free(cmd);

    /* hello in as \n */
    cmd = strdup("hello in as   \n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res); free(cmd);
    CONSOLE_LOG_TEST("\"hello in as\"");

    /* hello not_in not_as login \n */
    cmd = strdup("hello in nawak id\n");
    strtok(cmd,delim);
    res= send__client_id(&henry);
    assert(strcmp(res,"no greeting\n")==0);
    free(res); free(cmd);
    CONSOLE_LOG_TEST("\"hello word word id\" (not valid keywords)");

    /* without \n at the end */
    // Done in client.c while parsing the command

    /* Free */
    aq__remove_aquarium(aquarium);
    free(aquarium);
}

void tst__send_fishes()
{
    char * res = NULL;

    aquarium = malloc(sizeof(struct aquarium));
    aq__initialize_aquarium(aquarium, AQUARIUM_DIMENSIONS);
    aq__add_view(aquarium, (struct position) {0, 0}, (struct dimension) {500, 500}, "N1");
    aq__add_view(aquarium, (struct position) {0, 500}, (struct dimension) {500, 500}, "N2");
    aq__add_view(aquarium, (struct position) {500, 0}, (struct dimension) {500, 500}, "N3");
    aq__add_view(aquarium, (struct position) {500, 500}, (struct dimension) {500, 500}, "N4");

    struct thread_p henry;
    henry._client = malloc(sizeof(struct client *));printf("#### res : %s",res);

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
    assert(strcmp(res,"NOK\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"getFishes some arguments\" (invalid syntax)");
}

void tst__send_logout(){

    char * res;
    char * cmd = strdup("log out\n");
    strtok(cmd,delim);
    res = send__logout(NULL);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"log out\" (with NULL thread argument)");

    struct thread_p henry;
    henry._client = malloc(sizeof(struct client *));

    cmd = strdup("log out      \n");
    strtok(cmd,delim);
    res = send__logout(&henry);
    assert(strcmp(res,"bye\n")==0);
    CONSOLE_LOG_TEST("\"log out\"");

    cmd = strdup("log out arg arg \n");
    strtok(cmd,delim);
    res = send__logout(&henry);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("log out arg\n");
    strtok(cmd,delim);
    res = send__logout(&henry);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"log out some arguments\"");
}

void tst__send_ping(){
    char * res;
    char * cmd = strdup("ping 666    \n");
    strtok(cmd,delim);
    res = send__ping(NULL);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"ping number\" (with NULL thread argument)");

    struct thread_p henry;
    henry._client = malloc(sizeof(struct client *));

    cmd = strdup("ping    666      \n");
    strtok(cmd,delim);
    res = send__ping(henry._client);
    assert(strcmp(res,"pong 666\n")==0);

    cmd = strdup("ping    abc   \n");
    strtok(cmd,delim);
    res = send__ping(henry._client);
    assert(strcmp(res,"pong abc\n")==0);
    CONSOLE_LOG_TEST("\"ping something(number or string)\"");

    cmd = strdup("ping   666   suparg   \n");
    strtok(cmd,delim);
    res = send__ping(henry._client);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"ping number more arguments\"");
}

void tst__send_add_fish(){

    aquarium = malloc(sizeof(struct aquarium));
    aq__initialize_aquarium(aquarium, AQUARIUM_DIMENSIONS);
    aq__add_view(aquarium, (struct position) {250, 250}, (struct dimension) {500, 500}, "Cookie");

    struct thread_p henry;
    henry._client = malloc(sizeof(struct client *));

    char * res;
    char * cmd = strdup("hello\n");
    res = send__client_id(&henry);
    free(res);

    cmd = strdup("addFish PoissonRouge at 90x40,10x4, RandomWayPoint\n");
    strtok(cmd,delim);
    res = send__add_fish(NULL);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"addFish\" (with NULL thread argument)");

    cmd = strdup("addFish PoissonRouge        at         90x40      ,      10x4     ,      RandomWayPoint   qqch    \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"addFish\" with extra arguments");

    cmd = strdup("addFish PoissonRouge        at         90x40      ,      10x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"OK\n")==0);
    CONSOLE_LOG_TEST("\"addFish\" correct syntax");

    cmd = strdup("addFish PoissonRouge        at         90x40      ,      10x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"addFish\" adding an existing fish");

    /* Invalid syntaxes */
    cmd = strdup("addFish PoissonRouge1     90x40      ,      10x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("addFish PoissonRouge2   at      ,      10x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("addFish PoissonRouge3   at    90x50  ,        ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("addFish PoissonRouge4   at    90x50  ,    10x4    ,        \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"addFish\" missing arguments");

    cmd = strdup("addFish PoissonRouge5     at       90x40      ,      10x4           RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("addFish PoissonRouge6     at       90x40            10x4           RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("addFish PoissonRouge7     at       90x40            10x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"addFish\" without comas separators");

    cmd = strdup("addFish PoissonRouge8     at       9040      ,      10x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("addFish PoissonRouge9     at       90x40      ,      104     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("addFish PoissonRouge10     at       abc      ,      10x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("addFish PoissonRouge11     at       90x50      ,      def     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("addFish PoissonRouge12     at       axb      ,      10x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("addFish PoissonRouge13     at       10x10      ,      cxd     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"addFish\" with incorrect format for <x>x<y> arguments (string, without the separator 'x')");

    cmd = strdup("addFish PoissonRouge14     at       90x40      ,      10x4     ,      Patate   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"addFish\" with unavailable mobility name");

    cmd = strdup("addFish PoissonRouge15     atos       abc      ,      10x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"addFish\" incorrect 'at' argument");

    cmd = strdup("addFish PoissonRouge16     at       0x10      ,      10x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"OK\n")==0);

    cmd = strdup("addFish PoissonRouge17     at       10x0      ,      10x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"OK\n")==0);
    CONSOLE_LOG_TEST("\"addFish\" with '0' in <x>x<y> position");

    cmd = strdup("addFish PoissonRouge18     at       10x10      ,      0x4     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);

    cmd = strdup("addFish PoissonRouge19     at       10x10      ,      10x0     ,      RandomWayPoint   \n");
    strtok(cmd,delim);
    res = send__add_fish(henry._client);
    assert(strcmp(res,"NOK\n")==0);
    CONSOLE_LOG_TEST("\"addFish\" with '0' for fish size");
}

void tst__send_delete_fish(){
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

    /* Adding some fishes */
    struct fish *poulpi = fish__create(OCTOPUS,10,10,"Poulpi",HORIZONTAL,(struct dimension) {10,10}, 5.0);
    struct fish *superpoulpi = fish__create(OCTOPUS,100,80,"Super-poulpi",VERTICAL,(struct dimension) {10,10}, 10.0);
    struct fish *reddy = fish__create(REDFISH,800,900,"Reddy",RANDOM,(struct dimension) {10,10}, 1.0);
    aq__add_fish_to_aqv(aquarium,"N1",poulpi);
    aq__add_fish_to_aqv(aquarium,"N1",superpoulpi);
    aq__add_fish_to_aqv(aquarium,"N4",reddy);

    cmd=strdup("delFish   Poulpi  \n");
    strtok(cmd,delim);
    res= send__delete_fish();
    assert(strcmp(res,"OK\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"delFish existing_fish\"");

    /* Missing argument */
    cmd=strdup("delFish   \n");
    strtok(cmd,delim);
    res= send__delete_fish();
    assert(strcmp(res,"NOK\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"delFish\" without argument");

    /* Too many arguments */
    cmd=strdup("delFish   Mandarine     suparg\n");
    strtok(cmd,delim);
    res= send__delete_fish();
    assert(strcmp(res,"NOK\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"delFish\" without too many arguments");

    /* With an not existing fish */
    cmd=strdup("delFish   not_existing_fish  \n");
    strtok(cmd,delim);
    res= send__delete_fish();
    assert(strcmp(res,"NOK\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"delFish not_existing_fish\"");
}

void tst__send_start_fish(){
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

    /* Adding some fishes */
    struct fish *poulpi = fish__create(OCTOPUS,10,10,"Poulpi",HORIZONTAL,(struct dimension) {10,10}, 5.0);
    struct fish *superpoulpi = fish__create(OCTOPUS,100,80,"Super-poulpi",VERTICAL,(struct dimension) {10,10}, 10.0);
    struct fish *reddy = fish__create(REDFISH,800,900,"Reddy",RANDOM,(struct dimension) {10,10}, 1.0);
    aq__add_fish_to_aqv(aquarium,"N1",poulpi);
    aq__add_fish_to_aqv(aquarium,"N1",superpoulpi);
    aq__add_fish_to_aqv(aquarium,"N4",reddy);

    cmd=strdup("startFish   Poulpi  \n");
    strtok(cmd,delim);
    res= send__start_fish();
    assert(strcmp(res,"OK\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"startFish existing_fish\"");

    /* Missing argument */
    cmd=strdup("startFish   \n");
    strtok(cmd,delim);
    res= send__start_fish();
    assert(strcmp(res,"NOK\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"startFish\" without argument");

    /* Too many arguments */
    cmd=strdup("delFish   Mandarine     suparg\n");
    strtok(cmd,delim);
    res= send__start_fish();
    assert(strcmp(res,"NOK\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"startFish\" without too many arguments");

    /* With an not existing fish */
    cmd=strdup("startFish   not_existing_fish  \n");
    strtok(cmd,delim);
    res= send__start_fish();
    assert(strcmp(res,"NOK\n")==0);
    free(res);
    CONSOLE_LOG_TEST("\"startFish not_existing_fish\"");
}

int main()
{
  _set_verbosity(FALSE);
  _set_test_verbosity(TRUE);

  // Intern communication
  tst__send_client_id();
  //tst__send_fishes();
  //tst__send_logout();
  //tst__send_ping();
  //tst__send_add_fish();
  //tst__send_delete_fish();
  //tst__send_start_fish();

  return 0;
}
