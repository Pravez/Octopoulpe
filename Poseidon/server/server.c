#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "server.h"
#include "answer.h"
#define MAX 256
#define PARAM 2
#define CLIENTS 5

void* server_process(void* arg){
    int portno = *(int*)arg;
    connexion(portno);
    return 0;
}


void parse(char buffer[MAX], char buf_res[MAX]) {
  char cmd[MAX];
  int i = 0;
  int j = 0;
  char arg[MAX];
  char *res;
  printf("1\n");
  while((buffer[i] != ' ') && (buffer[i] != '\0')) {
    printf("i: %d, c: %c\n", i, buffer[i]);
    cmd[i] = buffer[i];
    ++i;
  }
  //give just the end of the command to the function
  while((buffer[i+j] != '\0') && (buffer[i+j] != '\n')) {
    printf("i+j: %d, c: %c\n", i+j, buffer[i+j]);
    arg[j] = buffer[i+j];
    j++;
  }
  printf("2\n");
  arg[j+1] = '\0';
  if(strcmp(cmd, "hello") == 0)
    res = asw__hello(arg);
  else
    res = strcpy(res, "Unknown command\n");
  printf("3\n");
  buf_res = strcpy(buf_res, res); //this certainly prints garbage
  printf("4\n");
  return;
}


void * start(void* arg) {
  char buffer[MAX];
  int newsockfd = *((int *) arg);
  int n;
  char buf_res[MAX];
  
  bzero(buffer, 256);
  while(1) {
    n = read(newsockfd, buffer, 255);
    printf("n: %d\n", n);
    if (n < 0) {perror("ERROR reading from socket"); exit(1);}
    else if (n == 2) { //fix the segfault in case of empty message
      n = write(newsockfd, "\n", 1);
      if (n < 0) { perror("ERROR writing to socket"); exit(1); }
    }
    else {
      n = write(newsockfd, "Parsing your command...\n", 25);
      parse(buffer, buf_res);
      //printf("Here is the message :%s\n", buffer); //sprintf pour string
      //traitement
      n = write(newsockfd, buf_res, sizeof(buf_res));
      if (n < 0) {perror("ERROR writing to socket");exit(1);}
    }
  }
}

void connexion(int portno){
  int sockfd, newsockfd, clilen;
  struct sockaddr_in serv_addr, cli_addr;
  int i;
  clilen = sizeof(cli_addr);
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {perror("ERROR opening socket"); exit(1);}
  
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");
  listen(sockfd, 5);
  while(newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)){
      pthread_t thread;
      if(pthread_create(&thread, NULL, start, &newsockfd) < 0) {perror("pthread_create"); exit(1);}
  }
}

int main(int argc, char *argv[]) {
    if (argc < PARAM) {
    fprintf(stderr, "ERROR no port provided\n");
    exit(1);
  }
  int portno = atoi(argv[1]);
  connexion(portno);
  return 0;
}
