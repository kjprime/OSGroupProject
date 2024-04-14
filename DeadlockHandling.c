/* ProcessManagment.c
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sager, lucas.sager@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Autor: Allison Meredith
Date:4/7/2024

File Discription: deadlock handling file (pseudocode draft)
--implment a deadlock prevention mechanism by eliminating circular wait
*/

// Define the processes and their requested resources
n = 6; //6 total processes
r = 4; 
int alloc[4][6] = {
    
    {"Create", "R1", "R2", "R3"},
    {"Deposit", "R1", "R2", "R3"},
    {"Withdraw", "R1", "R2", "R3"},
    {"Inquiry", "R1", "R2", "R3"},
    {"Transfer", "R2", "R3", "R1"},
    {"Close", "R1", "R2", "R3"}

};
//create max matrix
 int max[4][6] =      { }, //Create
                      { }, //Deposit
                      { }, //Withdraw
                      { }, //Inquiry 
                      { },  //Transsfer
                      { } }; //Close

//available resource matrix
 int available[3] = { 3, 3, 2 };

//calculate need matrix
 int need[n][m];
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
    }
//use banker's algorithm to check if system is safe

//check for circular wait
if (circular wait detected){
    return false;
}
