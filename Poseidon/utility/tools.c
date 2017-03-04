#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <execinfo.h>


#include "tools.h"

struct position __position(int x, int y) {
    struct position pos = {x, y};
    return pos;
}

struct dimension __dimension(int width, int height) {
    struct dimension dim = {width, height};
    return dim;
}

void _set_verbosity(int value) {
    verbosity = value;
}

void _console_log(int priority, char *message) {
    switch (priority) {
        case LOG_HIGH :
            printf(REDBOLD"%s"RESET"\n", message);
            break;
        case LOG_MEDIUM:
            if (verbosity)
                printf(YELLOW"%s"RESET"\n", message);
            break;
        case LOG_LOW:
            if (verbosity)
                printf("%s\n", message);
            break;
        default:
            break;
    }
}

char *concatenate_strings(int qty, ...) {
    va_list list;
    va_start(list, qty);

    int total_length = 1;
    for (int i = 0; i < qty; i++) {
        total_length += strlen(va_arg(list, char*));
    }
    va_end(list);
    va_start(list, qty);

    char *final = (char *) malloc(sizeof(char) * total_length);
    final[0] = '\0';

    for (int i = 0; i < qty; i++) {
        m_strcat(final, va_arg(list, char*));
    }

    va_end(list);
    return final;
}

char *m_strcat(char *dest, char *src) {
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return --dest;
}


void print_trace() {
    void *array[10];
    size_t size;
    char **strings;
    size_t i;

    size = (size_t) backtrace(array, 10);
    strings = backtrace_symbols(array, (int) size);

    printf("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++)
        printf("%s\n", strings[i]);

    free(strings);
}