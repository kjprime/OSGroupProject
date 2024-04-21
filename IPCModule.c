/* IPCModule.c
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sager, lucas.sager@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Author: Allison Meredith
Date: 4/10/2024

File Description: Inter-process Communication (IPC) Module:
*/
// shared memory THIS IS THE IPC FILE.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/wait.h>
#include "Transaction.h"

#include "que.h"
#include "que.c"

#define SHARED_MEMORY_SIZE 100
#define SHM_NAME "/data1_shared_memory"
#define SEM_NAME "/que_semaphore"
int shm_fd;


Transaction *sharedMemory;
SharedQueue *queue;
sem_t *semaphore;

void writeProcess(Transaction transaction) {//value needs to be a struct detailing the process // index needs to be account number.
    QueueElement element;
    element.data = transaction.index;  //trasaction number?
    element.pid = getpid();
    enqueue(element);
  //printf("I've made it here 0 in pid:%d\n",getpid());
    //printf("Enqueued process %d\n",element.pid);
    while(peek() != getpid());
    sem_wait(semaphore);
    Transaction temp = sharedMemory[transaction.index];
    sharedMemory[transaction.index] = transaction;
    //printf("Wrote value %d at index %d, prior value was %d\n", transaction.amount, transaction.index, temp.amount);
    sem_post(semaphore);
    dequeue(queue);
    //printf("Dequeued writing process %d\n", getpid());
}

Transaction readProcess(Transaction transaction) {
    QueueElement element;
    element.data = transaction.index;
    element.pid = getpid();
    enqueue(element);
    //printf("Enqueued process %d\n", getpid());
    while(peek() != getpid());
    sem_wait(semaphore);
    int value = sharedMemory[transaction.index].amount;
    //printf("Read value %d from index %d\n", value, transaction.index);
    sem_post(semaphore);
    dequeue(queue);
    //printf("Dequeued reading process %d\n", getpid());
    return sharedMemory[transaction.index];
}
Transaction readquiet(Transaction transaction) {    // for printing out nicely the history and final balances.
    QueueElement element;
    element.data = transaction.index;
    element.pid = getpid();
    enqueue_quiet(element);
    //printf("Enqueued process %d\n", getpid());
    while(peek() != getpid());
    sem_wait(semaphore);
    int value = sharedMemory[transaction.index].amount;
    //printf("Read value %d from index %d\n", value, transaction.index);
    sem_post(semaphore);
    dequeue(queue);
    //printf("Dequeued reading process %d\n", getpid());
    return sharedMemory[transaction.index];
}



void sharedMemorysetup()
{

    create_shared_memory();
    init_semaphores();


    // Create the shared memory object
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Set the size of the shared memory object
    ftruncate(shm_fd, SHARED_MEMORY_SIZE * sizeof(Transaction));

    // Map the shared memory object into the address space
    sharedMemory = (Transaction*)mmap(NULL, SHARED_MEMORY_SIZE * sizeof(Transaction), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedMemory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Initialize the semaphore
    semaphore = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

}

void sharedMemorycleanup(int pass)
{
    cleanup(pass);

    // Unmap and close the shared memory object
    munmap(sharedMemory, SHARED_MEMORY_SIZE * sizeof(Transaction));
    close(shm_fd);

    // Remove the shared memory object
    shm_unlink(SHM_NAME);

    // Close and unlink the semaphore
    sem_close(semaphore);
    sem_unlink(SEM_NAME);
    

}



// int main() {
//     /*
//     //cleanup(0);    //cleanup if you have issues with closing
//     // munmap(sharedMemory, SHARED_MEMORY_SIZE * sizeof(Transaction));
//     //close(shm_fd);

//     // Remove the shared memory object
//     shm_unlink(SHM_NAME);

//     // Close and unlink the semaphore
//     sem_close(semaphore);
//     sem_unlink(SEM_NAME);


//     // Remove the shared memory object
//     shm_unlink(SHM_NAME);
//     */
//    sharedMemorysetup();


//     int index = rand() % SHARED_MEMORY_SIZE;
//     int value;
//     Transaction transaction;
//     transaction.index = index;
//     // Create a child process
//     pid_t pid = fork();
//     if(pid == 0) pid = fork();
//     if(pid == 0) pid = fork();
//     if (pid == -1) {
//         perror("fork");
//         exit(1);
//     } else if (pid == 0) {
//         // Child process
//         int iterations = 0;
//         while (iterations < 10) {
//             readProcess(transaction);
//             iterations++;
//         }
//         exit(0);
//     } else {
//         // Parent process
//         int iterations = 0;
//         while (iterations < 10) {
//             value = rand() % 100;
//             transaction.amount = value;
//             writeProcess(transaction);
//             iterations++;
//         }
//         // Wait for the child process to finish
//         wait(NULL);
//         wait(NULL);
//         wait(NULL);
//     }
//     sharedMemorycleanup(1);
    
//     return 0;
// }
