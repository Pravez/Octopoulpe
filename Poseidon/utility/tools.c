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

boolean check_in_screen(struct position s_pos, struct dimension dimensions, struct position position) {
    if (position.x >= s_pos.x && position.y >= s_pos.y) {
        struct position e_pos = __position(s_pos.x + dimensions.width, s_pos.y + dimensions.height); //end_position
        if (position.x <= e_pos.x && position.y <= e_pos.y)
            return TRUE;
    }

    return FALSE;
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

unsigned int add_to_coordinate(int start, int val){
    if(start + val > 1000)
        return (unsigned int) (1000 - (start + val));
    else if(start + val < 0)
        return (unsigned int) (1000 + (start + val));
    else
        return (unsigned int) (start + val);
}

struct position add_to_position(struct position p, int x, int y) {
    struct position new;
    new.x = add_to_coordinate(p.x, x);
    new.y = add_to_coordinate(p.y, y);

    return new;
}