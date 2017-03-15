#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "answer.h"

char * asw__hello(char * arg)
{
  if(!strcmp(arg,"\n"))
    printf("Par défault\n");
  char * res ="greetings\n";
  return res;
}

char * asw__get_fishes(char * arg){return NULL;}
char * asw__get_fishes_continuously(char * arg){return NULL;}
char * asw__ping(char * arg){return NULL;}
char * asw__log(char * arg){return NULL;}
char * asw__add_fish(char * arg){return NULL;}
char * asw__del_fish(char * arg){return NULL;}
char * asw__start_fish(char * arg){return NULL;}

int main(int argc, char * argv[])
{
  asw__hello("\n");
}
  
