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

int asw__hello(char * arg, char * res)
{
  char cmd[256];
  char * id;
  strcpy(cmd,"greetings ");
  if(!strcmp(arg,"\n"))
    {
      id = "default";
      strcat(strcat(cmd,id),"\n");
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
	      //printf("id : %s\n",id);
	      strcat(strcat(cmd,id),"\n");
	      // id = aq__available_id(strtok(NULL," "));
	    }
	  else
	    {
	      strcpy(res,"no greeting\n");
	      return HELLO_FAILURE;
	    }
	}
      else
	{
	  strcpy(res,"no greeting\n");
	  return HELLO_FAILURE;
	}
    }
  strcpy(res,cmd);
  return HELLO_SUCCESS;
}

/*
int main(int argc, char * argv[])
{
  char res[256];
  asw__hello("\n",res);
  printf("%s",res);
  return 0;
}
*/
