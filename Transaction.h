/* Transaction.h
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sager, lucas.sager@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Author: Kevin Thomas
Date:4/14/2024

File Discription: struct def for trsaction sepperation
*/
#ifndef TRANSACTION_H
#define TRANSACTION_H

typedef struct {    // using 
    char accountID[10];
    char transactionType[10][17];
    int amount[10];
    char targetAccountID[10][10]; 
    char status[10][10];
    int history_length;
} History;

typedef struct {
    int index;
    int account;
    char accountID[10];
    char transactionType[17];
    int amount;
    char targetAccountID[10]; // For transfer operations
    char status[10];    //status, pending, processing, processed, error (declined)
    int history_length;
    History history;
} Transaction;


#endif
