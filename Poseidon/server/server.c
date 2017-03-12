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

/*
 * @function t_read
 * @brief    function reading input in a thread
 * @param    [int] newsockfd 
 * @return   ??
 */
/*
void * t_read(void * arg) {
  int n;
  int newsockfd = *((int *) arg);
  n = read(newsockfd,
}
*/
void *start(void *arg) {
    char buffer[MAX];
    int newsockfd = *((int *) arg);
    int n;

    bzero(buffer, 256);
    while (1) {
        n = read(newsockfd, buffer, 255);
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
        printf("Here is the message :%s\n", buffer);
        n = write(newsockfd, "I got your message", 18);
        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
    }
}

void connexion(int portno) {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int i;
    clilen = sizeof(cli_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");
    listen(sockfd, 5);
    while (newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) {
        pthread_t thread;
        if (pthread_create(&thread, NULL, start, &newsockfd) < 0) {
            perror("pthread_create");
            exit(1);
        }
    }
}

void* server_process(void* arg){
    int portno = *(int*)arg;
    connexion(portno);
    return 0;
}
