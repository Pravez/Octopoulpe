#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <readline.h>
#include <assert.h>

#include "../src/server/parser.h"
#include "../src/server/server.h"
#include "../src/model/aquarium.h"
#include "../src/view/view.h"
#include "../src/server/world/world.h"
#include "test.h"

extern struct _tvector* config_vector;
extern boolean readline_started;
struct aquarium* aquarium;

pthread_mutex_t mutex_observers;

pthread_t thread_menu;
pthread_t thread_server;
pthread_t thread_world;

int WORLD_READY;
int SERVER_READY;

void tst__parse_config_file(){

    CONSOLE_TEST_TITLE("parse_config_file");
    int nb_tst = 0;

    parse_config_file("controller.cfg");

    /* Verifying the correctness of the values obtained */
    assert(_get_value(config_vector, "controller-port") == 12345); nb_tst++;
    CONSOLE_LOG_TEST("Correct value of controller-port");

    assert(_get_value(config_vector, "display-timeout-value") == 45); nb_tst++;
    CONSOLE_LOG_TEST("Correct value of display-timeout-value");

    assert(_get_value(config_vector, "fish-update-interval") == 1); nb_tst++;
    CONSOLE_LOG_TEST("Correct value of fish-update-interval");

    /* Free */
    _delete_tvector(config_vector);

    CONSOLE_TEST_RES(nb_tst);
}

int main() {

    fprintf(CONSOLE_OUTPUT,"#######################################################\n######## Controller config file parser testing ########\n#######################################################\n");
    _set_verbosity(FALSE);
    _set_test_verbosity(TRUE);

    /* Tests */
    tst__parse_config_file();

    return 0;
}
