/* DeadlockHandling.c
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sager, lucas.sager@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Author: 
Date:4/10/2024

File Description: deadlock handling file 
--implement a deadlock prevention mechanism by eliminating circular wait
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Synchronization.h"


#define MAX_COUNT 3
#define SLEEP_TIMER 10

// Global variables for shared memory and semaphore
int shmid;
SharedQueue *queue;
sem_t *mutex, *full, *empty;


// Function to access shm
int check_queue(pid_t pid) {
    if(pid == -1){
        //if able to access element successfully
        return -1;
    }
    else{
        //unable to access resource
        return 0;
        }
}

// Function to acquire element with timeout
int timeout_handler() {
    int count = 0;
    while (1) {

        //element acquired sucessfully 
        if(check_queue(-1)==-1){
            return 1; 
        }

        //maximum retries reached...process failed
        if (count >= MAX_COUNT) {
            printf("Process failed...\n");
            return 0;
        }

        //retry after sleep
        printf("Retrying after %d seconds...\n", SLEEP_TIMER);
        sleep(SLEEP_TIMER);
        count++;
        return 0; 
    }
}



