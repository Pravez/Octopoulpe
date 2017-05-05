#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <execinfo.h>
#include <time.h>

#include "tools.h"
#include "data.h"


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
        struct position e_pos = __position((int) (s_pos.x + dimensions.width), (int) (s_pos.y + dimensions.height)); //end_position
        if (position.x <= e_pos.x && position.y <= e_pos.y)
            return TRUE;
    }

    return FALSE;
}

void _set_verbosity(int value) {
    verbosity = value;
}

void _set_test_verbosity(int value) {
    test_verbosity = value;
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

double add_to_coordinate(double start, double val, int max_value){
    if(start + val > max_value)
        return (max_value - (start + val));
    else if(start + val < 0)
        return (max_value + (start + val));
    else
        return (start + val);
}

struct position add_to_position(struct position p, double x, double y) {
    struct position new;
    new.x = add_to_coordinate(p.x, x, AQUARIUM_WIDTH);
    new.y = add_to_coordinate(p.y, y, AQUARIUM_HEIGHT);

    //If the position goes beyond the limits
    if(new.x > AQUARIUM_WIDTH)
        new.x = new.x - AQUARIUM_WIDTH;
    else if(new.x < 0)
        new.x = AQUARIUM_WIDTH + new.x;

    if(new.y > AQUARIUM_HEIGHT)
        new.y = new.y - AQUARIUM_HEIGHT;
    else if(new.y < 0)
        new.y = AQUARIUM_HEIGHT + new.y;

    return new;
}

struct position add_to_position_basic(struct position p, double x, double y){
    return (struct position){p.x + x, p.y + y};
}

int position_equals(struct position pos1, struct position pos2){
    return ((int)pos1.x == (int)pos2.x) && ((int)pos1.y == (int)pos2.y);
}

int in_bounds(struct position starting_point, struct dimension dim, struct position pos){
    return (pos.x >= starting_point.x && pos.x <= starting_point.x + dim.width)
            && (pos.y >= starting_point.y && pos.y <= starting_point.y + dim.height);
}

int msleep(unsigned long milisec) {
    struct timespec req={0};
    time_t sec=(int)(milisec/1000);
    milisec=milisec-(sec*1000);
    req.tv_sec=sec;
    req.tv_nsec=milisec*1000000L;
    while(nanosleep(&req,&req)==-1)
        continue;
    return 1;
}
