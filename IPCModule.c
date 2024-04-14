/* IPCModule.c
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sanger, lucas.sanger@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Author: Allison Meredith
Date: 4/10/2024

File Discription: Inter-process Communication (IPC) Module:
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>


//Function to write data into shared memory
void writeToSharedMemory(char*accountID, char *transactionType, int amount, char *targetAccountID) {
    key_t key = ftok("shmfile", 65); //Generate key
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT); //Create shared memory segment

    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    char* str = (char*)shmat(shmid, NULL, 0); //Attach process to shared memory

    //printf("Write Data: ");
    //fgets(str, 1024, accountID, transactionType, amount, targetAccountID); //read input 

   //printf("Data written in memory: %s", str);

    shmdt(str); //detach process from shared memory
}


// Function to read data from shared memory
void readFromSharedMemory() {
    key_t key = ftok("shmfile", 65); //Same key as writer process
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT); // Retrieve shared memory segment

    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    char* str = (char*)shmat(shmid, NULL, 0); //Attach process to shared memory

    //printf("Data read from memory: %s", str);

    shmdt(str); //Detach process from shared memory
    shmctl(shmid, IPC_RMID, NULL); //Destroy shared memory segment
}

int main() {

   /* writeToSharedMemory(); //call write function
    readFromSharedMemory(); //call read function
   */
    return 0;
}

