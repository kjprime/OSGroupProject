/* Synchronization.c
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sager, lucas.sager@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Author: Kevin Thomas
Date:4/7/2024

File Discription:
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Synchronization.h"

#define MAX_QUEUE_SIZE 10



// Global variables for shared memory and semaphore
int shmid;
SharedQueue *queue;
sem_t *mutex, *full, *empty;

// Create shared memory segment for the queue
void create_shared_memory() {
    shmid = shmget(IPC_PRIVATE, sizeof(SharedQueue), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    queue = (SharedQueue *)shmat(shmid, NULL, 0);
    if (queue == (void *)-1) {
        perror("shmat");
        exit(1);
    }
    queue->front = queue->rear = -1;
    queue->size = MAX_QUEUE_SIZE;
}

// Initialize semaphores
void init_semaphores() {
    mutex = sem_open("/queue_mutex", O_CREAT | O_EXCL, 0666, 1);
    full = sem_open("/queue_full", O_CREAT | O_EXCL, 0666, 0);
    empty = sem_open("/queue_empty", O_CREAT | O_EXCL, 0666, MAX_QUEUE_SIZE);
}
void display_queue() {
    sem_wait(mutex);
    
    if (queue->front == -1) {
        printf("Queue is empty\n");
        sem_post(mutex);
        return;
    }

    printf("Queue elements:");
    int i = queue->front;
    do {
        printf("acccount index:%d, pid:%d ;", queue->elements[i].data,queue->elements[i].pid);
        i = (i + 1) % queue->size;
    } while (i != (queue->rear + 1) % queue->size);
    printf("\n");
    
    sem_post(mutex);
}

// Enqueue an element into the queue
void enqueue(QueueElement element) {
    //display_queue();//duplicate for seeing before que is added to.
    sem_wait(empty);
    sem_wait(mutex);
    
    if (queue->front == -1)
        queue->front = 0;
    queue->rear = (queue->rear + 1) % queue->size;
    queue->elements[queue->rear] = element;
    
    sem_post(mutex);
    sem_post(full);

    display_queue();
}
void enqueue_quiet(QueueElement element) {  //duplicate reader for reading final printout
    sem_wait(empty);
    sem_wait(mutex);
    
    if (queue->front == -1)
        queue->front = 0;
    queue->rear = (queue->rear + 1) % queue->size;
    queue->elements[queue->rear] = element;
    
    sem_post(mutex);
    sem_post(full);
}

// Dequeue an element from the queue
QueueElement dequeue() {
    sem_wait(full);
    sem_wait(mutex);
    
    QueueElement element = queue->elements[queue->front];
    if (queue->front == queue->rear)
        queue->front = queue->rear = -1;
    else
        queue->front = (queue->front + 1) % queue->size;
    
    sem_post(mutex);
    sem_post(empty);
    
    return element;
}

pid_t peek() {
    sem_wait(mutex);

    if (queue->front == -1) {
        printf("Queue is empty\n");
        sem_post(mutex);
        return -1;
    }

    pid_t pid = queue->elements[queue->front].pid;

    sem_post(mutex);

    return pid;
}
void cleanup(int end) { // error message on or off
    // Detach shared memory
    if (shmdt(queue) == -1 && end) {
        perror("shmdt");
    }

    // Remove shared memory segment
   // if (shmctl(shmid, IPC_RMID, NULL) == -1) {
   //     perror("shmctl");
    //} //this is the section that gives errors cuz we have mutlie shared memory sections.

    // Close and unlink semaphores
    sem_close(mutex);
    sem_unlink("/queue_mutex");
    sem_close(full);
    sem_unlink("/queue_full");
    sem_close(empty);
    sem_unlink("/queue_empty");
}




/*
int main() {
    create_shared_memory();
    init_semaphores();
    pid_t parent = getpid();
    fork();
    // Example usage
    QueueElement element;
    element.data = 42;
    element.pid = getpid();
    enqueue(element);
    if (getpid() == parent)
    {
        wait(NULL);
    }
    display_queue();
    QueueElement dequeued_element = dequeue();
    printf("Dequeued element: %d\n", dequeued_element.data);

    if (getpid() == parent)
    {
        cleanup(1);
    }
    return 0;
}
*/
