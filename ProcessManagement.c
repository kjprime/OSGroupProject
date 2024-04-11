/* ProcessManagment.c
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sanger, lucas.sanger@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Autor: Lucas Sanger
Date:4/7/2024

File Discription:
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

typedef struct {
    char accountID[10];
    char transactionType[10];
    int amount;
    char targetAccountID[10]; // For transfer operations
} Transaction;

// Transaction function prototypes
void processTransaction(Transaction transaction);
void createAccount(char* accountID, int initialBalance);
void deposit(char* accountID, int amount);
void withdraw(char* accountID, int amount);
void transfer(char* accountID, char* targetAccountID, int amount);
void inquiry(char* accountID);
void closeAccount(char* accountID);

int main() {
    Transaction transactions[] = {
        {"A332131", "Withdraw", 40, ""},
        {"A342131", "Create", 40, ""},
        {"A342131", "Inquiry", 0, ""},
        {"A382131", "Create", 0, ""},
        {"A382131", "Deposit", 10, ""},
        {"A342131", "Transfer", 40, "A382131"},
        {"A342131", "Inquiry", 0, ""},
        {"A382131", "Inquiry", 0, ""},
        {"A342131", "Close", 0, ""}
    };
    int n = sizeof(transactions) / sizeof(Transaction);

    for (int i = 0; i < n; ++i) {
        pid_t pid = fork();

        if (pid == 0) { // Child process
            processTransaction(transactions[i]);
            exit(0); // Exit child process
        } else if (pid > 0) {
            // Parent process waits for child to complete
            wait(NULL);
        } else {
            printf("Fork failed\n");
            exit(1);
        }
    }

    return 0;
}

void processTransaction(Transaction transaction) {
    if (strcmp(transaction.transactionType, "Create") == 0) {
        createAccount(transaction.accountID, transaction.amount);
    } else if (strcmp(transaction.transactionType, "Deposit") == 0) {
        deposit(transaction.accountID, transaction.amount);
    } else if (strcmp(transaction.transactionType, "Withdraw") == 0) {
        withdraw(transaction.accountID, transaction.amount);
    } else if (strcmp(transaction.transactionType, "Transfer") == 0) {
        transfer(transaction.accountID, transaction.targetAccountID, transaction.amount);
    } else if (strcmp(transaction.transactionType, "Inquiry") == 0) {
        inquiry(transaction.accountID);
    } else if (strcmp(transaction.transactionType, "Close") == 0) {
        closeAccount(transaction.accountID);
    }
}

void createAccount(char* accountID, int initialBalance) {
    char filename[20];
    sprintf(filename, "%s.txt", accountID);
    FILE* file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%d", initialBalance);
        fclose(file);
    } else {
        printf("Failed to create account for %s\n", accountID);
    }
}

void deposit(char* accountID, int amount) {
    char filename[20];
    sprintf(filename, "%s.txt", accountID);
    FILE* file = fopen(filename, "r+");
    if (file) {
        int currentBalance;
        fscanf(file, "%d", &currentBalance);
        fseek(file, 0, SEEK_SET);
        fprintf(file, "%d", currentBalance + amount);
        fclose(file);
    } else {
        printf("Account %s not found for deposit\n", accountID);
    }
}

void withdraw(char* accountID, int amount) {
    char filename[20];
    sprintf(filename, "%s.txt", accountID);
    FILE* file = fopen(filename, "r+");
    if (file) {
        int currentBalance;
        fscanf(file, "%d", &currentBalance);
        if (currentBalance >= amount) {
            fseek(file, 0, SEEK_SET);
            fprintf(file, "%d", currentBalance - amount);
        } else {
            printf("Insufficient funds in account %s\n", accountID);
        }
        fclose(file);
    } else {
        printf("Account %s not found for withdrawal\n", accountID);
    }
}

void transfer(char* accountID, char* targetAccountID, int amount) {
    // First withdraw from source account
    withdraw(accountID, amount);
    // Then deposit to the target account
    deposit(targetAccountID, amount);
}

void inquiry(char* accountID) {
    char filename[20];
    sprintf(filename, "%s.txt", accountID);
    FILE* file = fopen(filename, "r");
    if (file) {
        int balance;
        fscanf(file, "%d", &balance);
        printf("Account %s has balance: $%d\n", accountID, balance);
        fclose(file);
    } else {
        printf("Account %s not found for inquiry\n", accountID);
    }
}

void closeAccount(char* accountID) {
    char filename[20];
    sprintf(filename, "%s.txt", accountID);
    if (remove(filename) != 0) {
        printf("Failed to close account %s\n", accountID);
    }
}
