#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "answer.h"

/* PROBLEMS to see with lmouret */
/* 
   # Il faut allouer dynamiquement les chaînes de caractères
   --> il faut free le résultat
*/

char * asw__hello(char * arg)
{
  char cmd[100];
  char * id;
  strcpy(cmd,"greetings");
  if(!strcmp(arg,"\n"))
    {
      id = "default";
      sprintf(cmd,"greeting %s\n",id);
      // faire fonction auxiliaire qui donne sous forme de (char *) un identifiant libre
      //id = aq__available_id(NULL);
    }
  else
    {
      char str[strlen(arg)];
      strcpy(str,arg);
      // lecture du "in"
      if (strcmp(strtok(str," "),"in")==0)
	{
	  if(strcmp(strtok(NULL," "),"as")==0)
	    {
	      id = strtok(NULL," ");
	      if(id[strlen(id)-1]=='\n')
		id[strlen(id)-1]='\0';	      
	      sprintf(cmd,"greeting %s\n",id);
	      // id = aq__available_id(strtok(NULL," "));
	    }
	  else return "no greeting\n";
	}
      else return "no greeting\n";
    }
  
  char * ret = malloc(strlen(cmd)*sizeof(char));
  strcpy(ret,cmd);
  return ret;
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
  printf("%s",asw__hello("in as N1\n"));
}
  
