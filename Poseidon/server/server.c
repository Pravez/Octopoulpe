#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "server.h"
#include "answer.h"
#include "../utility/tools.h"

#define PARAM 2
#define LISTEN_QUEUE 10
#define CHOSE_AUTOMATICALLY 0

LIST_HEAD(listhead, thread_entry) thread_head = LIST_HEAD_INITIALIZER(thread_head);

static const char* delim = " ";


#ifdef _PROCESS_
void *server_process(void *arg) {
    int portno = *(int *) arg;
    //Initializing list
    LIST_INIT(&thread_head);

    //Then we launch listening
    wait_connection(portno);
    return 0;
}
#endif //_PROCESS_



char* parse(char buffer[BUFFER_SIZE]) {
    int i = 0;
    int j = 0;
    char arg[BUFFER_SIZE];
    char *res;// = malloc(sizeof(char)*MAX);

    char* cmd;
    asprintf(&cmd, "%s", buffer);

    char *token = strtok(cmd, delim);
    if(!strcmp(token, "hello")){
        return check_client_id();
    }else if(!strcmp(token, "getFishes")){

    }else if(!strcmp(token, "getFishesContinuously")){

    }else if(!strcmp(token, "log")){

    }else if(!strcmp(token, "ping")){

    }else if(!strcmp(token, "addFish")){

    }else if(!strcmp(token, "delFish")){

    }else if(!strcmp(token, "startFish")){

    }

    return "Unknown command\n";

    //Je crois que tout ça devient assez obsolète (et fastidieux)
    /*while ((buffer[i] != ' ') && (buffer[i] != '\0')) {
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
    printf("4\n");*/
}

char* check_client_id(){
    char* id = strtok(NULL, delim);
    //Verify ID here
    //asw__hello(id, res, NULL);
    return "greeting or not ?";
}


void *start(void *arg) {

    char buffer[BUFFER_SIZE];
    int newsockfd = *((int *) arg);
    int n;
    char* response;

    bzero(buffer, BUFFER_SIZE);
    while (1) {
        n = read(newsockfd, buffer, BUFFER_SIZE-1);
        printf("n: %d\n", n);
        CHK_ERROR(n, "Error reading from socket")
        else if (n == 2) { //fix the segfault in case of empty message
            n = write(newsockfd, "\n", 1);
            CHK_ERROR(n, "Error writing to socket")
        } else {
            //ça sert à rien d'écrire ça, ça va juste obliger à parser encore plus coté client
            //n = write(newsockfd, "Parsing your command...\n", 25);
            response = parse(buffer);
            //traitement
            n = write(newsockfd, response, strlen(response));
            CHK_ERROR(n, "Error writing to socket")
        }
    }
}

void wait_connection(int portno) {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in server_addr, client_addr;

    clilen = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, CHOSE_AUTOMATICALLY);
    CHK_ERROR(sockfd, "Error opening socket")

    bzero((char *) &server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons((uint16_t) portno);
    CHK_ERROR(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)), "Error on binding")

    listen(sockfd, LISTEN_QUEUE);

    while ((newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &clilen))) {
        struct thread_entry* en = malloc(sizeof(struct thread_entry));
        LIST_INSERT_HEAD(&thread_head, en, entries);

        CHK_ERROR(pthread_create(&en->_thread, NULL, start, &newsockfd), "Error creating thread")
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
