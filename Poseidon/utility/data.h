#ifndef POSEIDON_DATA_H
#define POSEIDON_DATA_H

#include "tools.h"

#define AQUARIUM_WIDTH 1000
#define AQUARIUM_HEIGHT 1000
#define AQUARIUM_DIMENSIONS (struct dimension){AQUARIUM_WIDTH, AQUARIUM_HEIGHT}

#define SERVER_CMD_DELIMITER " "

#define SIGNAL_STOP_SENDING SIGUSR1
#define SIGNAL_NOTIFICATION SIGUSR2

#define PARAM 2
#define LISTEN_QUEUE 10
#define CHOSE_AUTOMATICALLY 0

#define DEFAULT_OBSERVERS_QTY 10
#define TIME_BETWEEN_CHECKS 3
#define MAXIMUM_IDLE_TIME 4

#define LOCK(mutex) pthread_mutex_lock(mutex)
#define UNLOCK(mutex) pthread_mutex_unlock(mutex)

int SPEED_RATE;

#endif //POSEIDON_DATA_H
