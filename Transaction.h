#ifndef TRANSACTION_H
#define TRANSACTION_H

typedef struct {
    char accountID[10];
    char transactionType[10];
    int amount;
    char targetAccountID[10]; // For transfer operations
} Transaction;

#endif
