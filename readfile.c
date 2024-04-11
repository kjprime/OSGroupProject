/* driver.c
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sanger, lucas.sanger@okstate.edu
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

#define MAX_LINE_LENGTH 100

int main() {
    FILE *file;
    char line[MAX_LINE_LENGTH];

    // Open the file for reading
    file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    // Skip the first line (assuming it contains the number of users)
    fgets(line, MAX_LINE_LENGTH, file);
    int numberOfUsers = atoi(line);
    printf("The number of users:%d\n",numberOfUsers);

    // Read and parse each line from line 2 until end of file
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        // Remove the newline character at the end of the line
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        char accountNum[MAX_LINE_LENGTH];
        char taskType[MAX_LINE_LENGTH];
        int value = -1;  // Default value for value
        int destAccountNum = -1;  // Default value for destination account number

        // Tokenize the line based on spaces
        char *token = strtok(line, " ");
        if (token != NULL) {
            strcpy(accountNum, token);  // Copy the first part as the account number
            token = strtok(NULL, " ");  // Move to the next part

            if (token != NULL) {
                strcpy(taskType, token);  // Copy the next part as the task type

                // If there's a third part, it's the value
                token = strtok(NULL, " ");
                if (token != NULL) {
                    value = atoi(token);  // Convert the third part to an integer

                    // If there's a fourth part, it's the destination account number
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        destAccountNum = atoi(token);  // Convert the fourth part to an integer
                    }
                }

                // Process the transaction
                if (destAccountNum != -1) {
                    if (value != -1) {
                        printf("Account: %s, Task: %s, Value: %d, Dest Account: %d\n",
                               accountNum, taskType, value, destAccountNum);
                    } else {
                        printf("Account: %s, Task: %s, Dest Account: %d\n",
                               accountNum, taskType, destAccountNum);
                    }
                } else {
                    if (value != -1) {
                        printf("Account: %s, Task: %s, Value: %d\n",
                               accountNum, taskType, value);
                    } else {
                        printf("Account: %s, Task: %s\n",
                               accountNum, taskType);
                    }
                }
            }
        }
    }

    // Close the file
    fclose(file);

    return 0;
}
