/* ProcessManagment.c
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sager, lucas.sager@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Author: Lucas Sager
Date:4/7/2024

File Discription:
*/
// synchronization.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex;

void init_synchronization() {
    pthread_mutex_init(&mutex, NULL);
}

void acquire_lock() {
    pthread_mutex_lock(&mutex);
}

void release_lock() {
    pthread_mutex_unlock(&mutex);
}

void cleanup_synchronization() {
    pthread_mutex_destroy(&mutex);
}
//Still needs queue. Working on Queue.
