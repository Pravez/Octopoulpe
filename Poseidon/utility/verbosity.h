#ifndef POSEIDON_VERBOSITY_H
#define POSEIDON_VERBOSITY_H

#define boolean int
#define TRUE 1
#define FALSE 0

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

#endif //POSEIDON_VERBOSITY_H
