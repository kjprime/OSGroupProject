/* driver.c
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sager, lucas.sager@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Autor: Kevin Thomas
Date:3/30/2024

File Discription: Main function driver file for OS project
*/


#include <stdio.h>
#include <stdlib.h>
#include "readfile.c"


//#include <ProcessManagment.c>
//#include <Syncronization.c>


int main() {
    char input_txt[] = "input.txt"; //input file name
    sharedMemorycleanup(0);  // for issues. 
    sharedMemorysetup();
    readfile();
    //loop for input file content avalablity

    // Process input text for data related to account functions

    //Pass input perameters to procecss managment based uppon function needed.

    //report progress

    //execute deadlock prevention routine

    //exit loop condition EOF

    //wait for all processes to have finished

    //report completion status
    sharedMemorycleanup(1);
    
    // Exit the program
    return 0;
}
