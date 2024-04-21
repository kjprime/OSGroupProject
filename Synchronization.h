/* Synchronization.h
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sager, lucas.sager@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Author: Kevin Thomas
Date:4/7/2024

File Discription:
*/
//que.h
#ifndef QUE_HEAD_H
#define QUE_HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_QUEUE_SIZE 10

// Structure for an element in the queue
typedef struct {
    int data;
    pid_t pid;
    int account;
} QueueElement;

// Structure for the shared queue
typedef struct {
    int front;
    int rear;
    int size;
    QueueElement elements[MAX_QUEUE_SIZE];
} SharedQueue;




#endif
