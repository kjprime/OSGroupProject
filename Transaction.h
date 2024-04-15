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

typedef struct {
    char accountID[10];
    char transactionType[10];
    int amount;
    char targetAccountID[10]; // For transfer operations
} Transaction;

#endif
