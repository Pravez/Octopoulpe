#ifndef POSEIDON_TOOLS_H
#define POSEIDON_TOOLS_H

#include "data.h"
#include <readline/readline.h>

#define boolean int
#define TRUE 1
#define FALSE 0

#define CHK_ERROR(test, message) if(test < 0){ perror(message); exit(1); }
#define CHCK_NULL(x, name) if(x == NULL){ fprintf(stderr, "FUNC %s : A VALUE IS NULL : %s\n", __func__, name); return; }
#define CHCK_NULL_INT(x, name) if(x == NULL){ fprintf(stderr, "FUNC %s : A VALUE IS NULL : %s\n", __func__, name); return -1; }
#define RAND_IN_RANGE(range, min) rand() % range + min

#define RED "\e[31m"
#define YELLOW "\e[93m"
#define GREEN "\e[92m"
#define BOLD "\e[1m"
#define REDBOLD "\e[91;1m"
#define YELLOWBOLD "\e[93;1m"
#define GREENBOLD "\e[32;1m"
#define RESET "\e[0m"

boolean verbosity;
boolean test_verbosity;
boolean readline_started;

#define CONSOLE_LOG_ERR(message, ...) if(verbosity){ \
        if(readline_started){ \
        rl_save_prompt();rl_message("[ "REDBOLD"ERROR"RESET" ]    " message "\n", ##__VA_ARGS__);rl_restore_prompt(); rl_clear_message();\
        }else{ \
        printf("[ "REDBOLD"ERROR"RESET" ] \t" message "\n", ##__VA_ARGS__);}}
#define CONSOLE_LOG_WARN(message, ...) if(verbosity){ \
        if(readline_started){ \
        rl_save_prompt();rl_message("[ "YELLOW"WARN"RESET" ]     " message "\n", ##__VA_ARGS__);rl_restore_prompt(); rl_clear_message();\
        }else{ \
        printf("[ "YELLOW"WARN"RESET" ] \t" message "\n", ##__VA_ARGS__);}}
#define CONSOLE_LOG_INFO(message, ...) if(verbosity){ \
        if(readline_started){ \
        rl_save_prompt();rl_message("[ "GREEN"INFO"RESET" ]     " message "\n", ##__VA_ARGS__);rl_restore_prompt(); rl_clear_message();\
        }else{ \
        printf("[ "GREEN"INFO"RESET" ] \t" message "\n", ##__VA_ARGS__);}}

#define CONSOLE_LOG_TEST(message, ...) if(test_verbosity) fprintf(CONSOLE_OUTPUT, "[ "GREENBOLD" OK "RESET" ] \t" message "\n", ##__VA_ARGS__);

struct position {
    double x;
    double y;
};

struct movement {
    int x;
    int y;
};

struct relative_position {
    int x;
    int y;
};

struct dimension {
    int width;
    int height;
};

struct bounds {
    struct dimension _dimensions;
    struct position _starting_position;
};

struct array {
    int _length;
    void *_values;
};

struct position __position(int x, int y);

struct dimension __dimension(int width, int height);

boolean check_in_screen(struct position s_pos, struct dimension dimensions, struct position position);


void _set_verbosity(int value);

void _set_test_verbosity(int value);

char *concatenate_strings(int qty, ...);

char *m_strcat(char *dest, char *src);

double add_to_coordinate(double start, double val, int max_value);

struct position add_to_position(struct position p, double x, double y);

struct position add_to_position_basic(struct position p, double x, double y);

int position_equals(struct position pos1, struct position pos2);

int in_bounds(struct position starting_point, struct dimension dim, struct position pos);

int msleep(unsigned long milisec);

/**
 * Taken from gnu.org https://www.gnu.org/software/libc/manual/html_node/Backtraces.html
 */
void print_trace();

#endif //POSEIDON_TOOLS_H
