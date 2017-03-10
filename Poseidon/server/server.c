//
// Created by paubreton on 03/03/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "server.h"
#define MAX 256
#define PARAM 2
#define CLIENTS 5

void * start(void* arg) {
  char buffer[MAX];
  int sockfd = *((int *) arg);
  int n;
  struct sockaddr_in cli_addr;
  int clilen = sizeof(cli_addr);
  int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
  if (newsockfd < 0) {perror("ERROR on accept"); exit(1);}
  bzero(buffer, 256);
  while(1) {
    n = read(newsockfd, buffer, 255);
    if (n < 0) {perror("ERROR reading from socket"); exit(1);}
    printf("Here is the message :%s\n", buffer);
    n = write(newsockfd, "I got your message", 18);
    if (n < 0) {perror("ERROR writing to socket");exit(1);}
  }
}

void connexion(int portno){
  int sockfd, newsockfd, clilen;
  
  struct sockaddr_in serv_addr, cli_addr;
  int i;
  pthread_t threads[CLIENTS];
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {perror("ERROR opening socket"); exit(1);}
  
  bzero((char *) &serv_addr, sizeof(serv_addr));

  
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");
  listen(sockfd, 5);
  for(i = 0; i< CLIENTS; ++i) {
    if(pthread_create(&threads[i], NULL, start, &sockfd) < 0) {perror("pthread_create"); exit(1);}
  }
  for(i = 0; i < CLIENTS; ++i)
    pthread_join(threads[i], NULL);

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
