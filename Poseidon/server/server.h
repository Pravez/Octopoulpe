#ifndef POSEIDON_SERVER_H
#define POSEIDON_SERVER_H

void * start(void* arg);
void connexion(int portno);

void* server_process(void* arg);

#endif //POSEIDON_SERVER_H
