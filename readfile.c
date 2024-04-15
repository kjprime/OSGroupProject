/* driver.c
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sager, lucas.sager@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Autor: Kevin Thomas
Date:4/10/2024

File Discription: Reads the input file and passes infomation to the process managment file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Transaction.h"
#include "ProcessManagement.c"

#define MAX_LINE_LENGTH 100
Transaction transactions[100];

void readfile() {
    FILE *file;
    char line[MAX_LINE_LENGTH];

    // Open the file for reading
    file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return ;
    }

    // Skip the first line (assuming it contains the number of users)
    fgets(line, MAX_LINE_LENGTH, file);
    int numberOfUsers = atoi(line);
    printf("The number of users:%d\n",numberOfUsers);
    int lineNumber =0;

    // Read and parse each line from line 2 until end of file
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        // Remove the newline character at the end of the line
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        char accountNum[MAX_LINE_LENGTH];
        char taskType[MAX_LINE_LENGTH];
        int value ;  // Default value for value
        char destAccountNum [MAX_LINE_LENGTH];  // Default value for destination account number

        // Tokenize the line based on spaces
        char *token = strtok(line, " ");
        if (token != NULL) {
            strcpy(accountNum, token);  // Copy the first part as the account number
            token = strtok(NULL, " ");  // Move to the next part

            if (token != NULL) {
                strcpy(taskType,token);  // Copy the next part as the task type

                // If there's a third part, it's the value
                token = strtok(NULL, " ");
                if (token != NULL) {
                    value = atoi(token);  // Convert the third part to an integer

                    // If there's a fourth part, it's the destination account number
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        strcpy(destAccountNum,token);  
                    }
                }




                // Process the transaction
                strcpy(transactions[lineNumber].accountID,accountNum);
                strcpy(transactions[lineNumber].transactionType,taskType);
                transactions[lineNumber].amount = value;
                strcpy(transactions[lineNumber].targetAccountID,destAccountNum);
                //printf("Account: %s, Task: %s, Value: %d, Dest Account: %s\n",accountNum, taskType, value, destAccountNum); //debug printout

            }

        }
        lineNumber++;
    }
    //Processs managment call...
    ProcessManagment(transactions, lineNumber);


    // Close the file
    fclose(file);

    return ;
}
