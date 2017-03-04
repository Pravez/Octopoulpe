#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "verbosity.h"

void _set_verbosity(int value){
    verbosity = value;
}

void _console_log(int priority, char* message){
    switch(priority){
        case LOG_HIGH :
            printf(REDBOLD"%s"RESET"\n", message);
            break;
        case LOG_MEDIUM:
            if(verbosity)
                printf(YELLOW"%s"RESET"\n", message);
            break;
        case LOG_LOW:
            if(verbosity)
                printf("%s\n", message);
            break;
        default:break;
    }
}

char* concatenate_strings(int qty, ...){
    va_list list;
    va_start(list, qty);

    int total_length = 1;
    for(int i=0;i<qty;i++){
        total_length += strlen(va_arg(list, char*));
    }
    va_end(list);
    va_start(list, qty);

    char* final = (char*) malloc(sizeof(char) * total_length);
    final[0] = '\0';

    for(int i=0;i<qty;i++){
        m_strcat(final, va_arg(list, char*));
    }

    va_end(list);
    return final;
}

char* m_strcat(char* dest, char* src){
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return --dest;
}