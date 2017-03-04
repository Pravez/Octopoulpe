#ifndef POSEIDON_TOOLS_H
#define POSEIDON_TOOLS_H

#define boolean int
#define TRUE 1
#define FALSE 0

struct position{
    int x;
    int y;
};

struct dimension{
    int width;
    int height;
};

struct position __position(int x, int y);
struct dimension __dimension(int width, int height);
boolean check_in_screen(struct position s_pos, struct dimension dimensions, struct position position);

#define LOG_HIGH 4
#define LOG_MEDIUM 3
#define LOG_LOW 2
#define LOG_SECTION 1

#define RED "\e[31m"
#define YELLOW "\e[33m"
#define BOLD "\e[1m"
#define REDBOLD "\e[31;1m"
#define YELLOWBOLD "\e[33;1m"
#define RESET "\e[0m"

boolean verbosity;

void _set_verbosity(int value);
void _console_log(int priority, char* messages);
char* concatenate_strings(int qty, ...);
char* m_strcat(char* dest, char* src);

/**
 * Taken from gnu.org https://www.gnu.org/software/libc/manual/html_node/Backtraces.html
 */
void print_trace();

#endif //POSEIDON_TOOLS_H
