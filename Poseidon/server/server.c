#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "server.h"
#include "answer.h"

#define MAX 256
#define PARAM 2

#define CHK_ERROR(test, msg) if(test < 0){ perror(msg); exit(1);}

#ifdef _PROCESS_
void *server_process(void *arg) {
    int portno = *(int *) arg;
    //Initializing list
    thread_head = LIST_HEAD_INITIALIZER(thread_head);
    LIST_INIT(&thread_head);

    //Then we launch listening
    wait_connection(portno);
    return 0;
}
#endif //_PROCESS_

void parse(char buffer[MAX], char buf_res[MAX]) {
    char cmd[MAX];
    int i = 0;
    int j = 0;
    char arg[MAX];
    char *res;// = malloc(sizeof(char)*MAX);
    printf("1\n");
    while ((buffer[i] != ' ') && (buffer[i] != '\0')) {
        //printf("i: %d, c: %c\n", i, buffer[i]);
        cmd[i] = buffer[i];
        ++i;
    }
    //give just the end of the command to the function
    while ((buffer[i + j] != '\0') && (buffer[i + j] != '\n')) {
        printf("i+j: %d, c: %c\n", i + j, buffer[i + j]);
        arg[j] = buffer[i + j];
        j++;
    }
    printf("2\n");
    arg[j + 1] = '\0';
    if (strcmp(cmd, "hello") == 0)
        asw__hello(arg, res, NULL);
    else
        res = strcpy(res, "Unknown command\n");
    printf("3\n");
    i = 0;
    while (res[i] != '\0') {
        //printf("i: %d, c: %c\n", i, res[i]);
        buf_res[i] = res[i];
        ++i;
    }
    buf_res[i] = '\0';
    //buf_res = strcpy(buf_res, res); //this certainly prints garbage -> YES IT DOES
    printf("4\n");
    return;
}


void *start(void *arg) {

    char buffer[MAX];
    int newsockfd = *((int *) arg);
    int n;
    char buf_res[MAX];

    bzero(buffer, 256);
    while (1) {
        n = read(newsockfd, buffer, 255);
        printf("n: %d\n", n);
        CHK_ERROR(n, "Error reading from socket")
        else if (n == 2) { //fix the segfault in case of empty message
            n = write(newsockfd, "\n", 1);
            CHK_ERROR(n, "Error writing to socket")
        } else {
            n = write(newsockfd, "Parsing your command...\n", 25);
            parse(buffer, buf_res);
            //printf("Here is the message :%s\n", buffer); //sprintf pour string
            //traitement
            n = write(newsockfd, buf_res, strlen(buf_res));
            CHK_ERROR(n, "Error writing to socket")
        }
    }
}

void wait_connection(int portno) {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in server_addr, client_addr;

    clilen = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    CHK_ERROR(sockfd, "Error opening socket")

    bzero((char *) &server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons((uint16_t) portno);
    CHK_ERROR(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)), "Error on binding")

    listen(sockfd, 5);

    while ((newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &clilen))) {
        struct thread_entry* en = malloc(sizeof(struct thread_entry));
        LIST_INSERT_HEAD(&thread_head, en, entries);

        CHK_ERROR(pthread_create(en->_thread, NULL, start, &newsockfd) < 0, "Error creating thread")
    }
}

#ifndef _PROCESS_
int main(int argc, char *argv[]) {
    if (argc < PARAM) {
    fprintf(stderr, "ERROR no port provided\n");
    exit(1);
  }
  int portno = atoi(argv[1]);
  wait_connection(portno);
  return 0;
}
#endif /*not _PROCESS_ */
