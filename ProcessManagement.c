/* ProcessManagement.c
Group Members:
Kevin Thomas, kevin.j.thomas@okstate.edu
Lucas Sager, lucas.sager@okstate.edu
Allison Meredith, allison.meredith@okstate.edu
Group: C
Author: Lucas Sager
Date:4/7/2024

File Discription:
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "Transaction.h"
#include <time.h>

#include "IPCModule.c"




// Transaction function prototypes
void processTransaction(Transaction transaction);
void createAccount(Transaction transaction);
void deposit(Transaction transaction);
void withdraw(Transaction transaction);
void transfer(Transaction transaction);
void inquiry(Transaction transaction);
void closeAccount(Transaction transaction);
int get_acc_numb(char *str);
void history_tostring(char processedAccountIDs[][10],int n);
void transaction_tostring(Transaction transaction);
void writefiles(Transaction transaction);

int ProcessManagment(Transaction transactions[], int n) {
   /* Transaction transactions[] = {
        {"A332131", "Withdraw", 40, ""},
        {"A342131", "Create", 40, ""},
        {"A342131", "Inquiry", 0, ""},
        {"A382131", "Create", 0, ""},
        {"A382131", "Deposit", 10, ""},
        {"A342131", "Transfer", 40, "A382131"},
        {"A342131", "Inquiry", 0, ""},
        {"A382131", "Inquiry", 0, ""},
        {"A342131", "Close", 0, ""}
    }; */   
char currentAccountID[10];
char processedAccountIDs[100][10]; // Assuming a maximum of 100 unique account IDs
int numProcessed = 0;
pid_t pid = 1;

for (int i = 0; i < n; ++i) {
    if (pid != 0) {
        int isNewAccount = 1;
        for (int j = 0; j < numProcessed; j++) {
            if (strcmp(processedAccountIDs[j], transactions[i].accountID) == 0) {
                isNewAccount = 0;
                break;
            }
        }
        if (isNewAccount) {
            strcpy(processedAccountIDs[numProcessed], transactions[i].accountID);
            strcpy(currentAccountID, transactions[i].accountID);
            numProcessed++;

            pid = fork();
            if (pid == -1) {
                printf("Error forking\n");
                exit(1);
            } else if (pid == 0) {
                // Child process
                printf("Child process %d created for account %s\n", getpid(), transactions[i].accountID);
            }
        }
    }
        if (pid == 0) { // Child process
            if (strcmp(transactions[i].accountID, currentAccountID) == 0)
            {
                //printf("I'm %d, and %s I'm processing trasaction %d,id: %s\n",getpid(),currentAccountID,i,transactions[i].accountID); // debug print statment
                processTransaction(transactions[i]);
            }
            //else printf("found id of %s, and I'm account id %s\n",transactions[i].accountID,currentAccountID);    //debug print statment
        }
    
    }

    if(pid!=0)
    {
            //printf("begening wait for returns from childs\n");    //debug stuff
        for(int i = 0; i < numProcessed; ++i) wait(NULL);
            //printf("done waiting for children\n");        //debug stuff
        history_tostring(processedAccountIDs,numProcessed);
    }
    //else printf("child process ending %d\n",getpid());      //debug stuff

    return 0;
}
void transaction_tostring(Transaction transaction){
    printf("Account:%s Ballance:%10d Transaction preformed:%s, %s, by %d | %s",transaction.accountID,transaction.amount,transaction.transactionType,transaction.status,getpid(),asctime(localtime(&transaction.history.time[transaction.history_length-1])));
}
void final_transaction_tostring(Transaction transaction){
    char status[10];
    if(!strcmp(transaction.accountID,"\0"))
    {
        strcpy(status, "Closed");
    }
    else strcpy(status, "Active");

    //printf("%s\n",transaction.status); //debug
    printf("Account:%10s Ballance:%10d Account Status:%s\n",transaction.accountID,transaction.amount,status);
}
void history_tostring(char processedAccountIDs[][10],int n){ //processed account id's and number of accoutns processed
    //look up all of the trasactions, print out history, and print out transaction to string
    printf("\nTracked accounts and history:\n");
    for(int i=0;i<n;i++)
    {
        Transaction temp;
        strcpy(temp.accountID,processedAccountIDs[i]);    //set the index number for use in memory
        temp.index = get_acc_numb(temp.accountID)% 71;
        temp = readquiet(temp);  //change this to dedicated
        printf("------------------------ Account: %10s ------------------------\n",temp.accountID);
        for(int ii=0;ii<temp.history_length;ii++){ //the history in the memory
            printf("history %d of %d Account:%s,%15s,%3d, %7s, %s | %s",ii,temp.history_length,temp.history.accountID,temp.history.transactionType[ii],temp.history.amount[ii],temp.history.targetAccountID[ii],temp.history.status[ii],asctime(localtime(&temp.history.time[ii])));
            //printf("```````````````````d````````````-------d--------`-`-`-`-`-`-`- time %s\n",asctime(temp.history.time[temp.history.history_length-1]));
            //  printf("tidmer timer timer %s",asctime(temp.history.time[temp.history.history_length-1]));
        }
    }
        printf("-----------------Final Ballances--------------------\n");
    for(int i=0;i<n;i++)
    {
        Transaction temp;
        strcpy(temp.accountID,processedAccountIDs[i]);    //set the index number for use in memory
        temp.index = get_acc_numb(temp.accountID)% 71;
        temp = readquiet(temp);  //change this to dedicated
        final_transaction_tostring(temp);
        writefiles(temp);
    }
}
void writefiles(Transaction transaction){
    char filename[20];
    sprintf(filename, "%s.txt", transaction.accountID);
    if(strcmp(transaction.accountID,"\0")){
    FILE* file = fopen(filename, "w");
    if (file) {
        fseek(file, 0, SEEK_SET);
        fprintf(file, "%d", transaction.amount);
        fclose(file);
    } else {
        printf("Account %s not found for final\n", transaction.accountID);
    }
    }
}

void processTransaction(Transaction transaction) {
    transaction.account = get_acc_numb(transaction.accountID);    //set the index number for use in memory
    transaction.index = transaction.account% 71;
    //printf("the account thingy is this right now. %s",transaction.accountID); //debuging
    if (strcmp(transaction.transactionType, "Create") == 0) {
        createAccount(transaction);
    } else if (strcmp(transaction.transactionType, "Deposit") == 0) {
        deposit(transaction);
    } else if (strcmp(transaction.transactionType, "Withdraw") == 0) {
        withdraw(transaction);
    } else if (strcmp(transaction.transactionType, "Transfer") == 0) {
        transfer(transaction);
    } else if (strcmp(transaction.transactionType, "Inquiry") == 0) {
        inquiry(transaction);
    } else if (strcmp(transaction.transactionType, "Close") == 0) {
        closeAccount(transaction);
    }
}

History history(Transaction transaction){
    time_t current_time;
    struct tm *local_time;
    current_time = time(NULL);
    local_time = localtime(&current_time);
    //printf("running the history fucntion for %s\n",transaction.transactionType);
    History newhist;
    newhist = transaction.history;
    //  for(int i=0;i<transaction.history_length;i++)
    //  {
    //  printf("--orig hist---history at index %d,# %s,%s,%d,%s,%s\n",i,newhist.accountID,newhist.transactionType[i],newhist.amount[i],newhist.targetAccountID[i],newhist.status[i]);
    //  }
    //printf("````````````````````11````1``````--1-------------`-`-`-`-`-`-`- time %s\n",asctime(local_time));
    strcpy(newhist.accountID, transaction.accountID);
    strcpy(newhist.transactionType[transaction.history_length],transaction.transactionType);
    //printf("HISSSSSSSSSSSSSSSSSSSSSSSSSSSSTRY :trans type:%s\n",newhist.transactionType[transaction.history_length]);
    newhist.amount[transaction.history_length] = transaction.amount;
    strcpy(newhist.targetAccountID[transaction.history_length],transaction.targetAccountID);
    strcpy(newhist.status[transaction.history_length],transaction.status);
    newhist.time[newhist.history_length] = current_time;
    //printf("timer timer timer %s",asctime(localtime(&newhist.time[newhist.history_length])));
    newhist.history_length= transaction.history_length+1;
    
    //printf("history length returned %d,history length in %d\n",newhist.history_length,transaction.history_length);
    // for(int i=0;i<newhist.history_length;i++)
    // {
    // printf("---added to hist--history at index %d,# %s,%s,%d,%s,%s\n",i,newhist.accountID,newhist.transactionType[i],newhist.amount[i],newhist.targetAccountID[i],newhist.status[i]);
    // }
    return newhist;
}

void createAccount(Transaction transaction) {
    //printf("STARTED CREAT ACCOUNT\n");    //debug
    Transaction temp = readProcess(transaction);
        //printf("382131plz:%d  history index:%d, account#%s, amount%d, condtion %s\n",temp.history_length,0,temp.history.accountID,temp.history.amount[0],temp.history.status[0]);
        //printf("past CREAT ACCOUNT read\n");
    transaction.history_length=temp.history_length;
    transaction.history = temp.history;
    //  printf("```````````````temp length history:%d\n",temp.history_length);  //debug
    if(temp.account != transaction.account) 
    {
    strcpy(transaction.status,"Success");
    transaction.history = history(transaction);
    transaction.history_length = transaction.history.history_length;
    // printf("```````````````length history:%d",transaction.history_length);   //debug
    writeProcess(transaction); //this is fixed.
    }
    else    //there already exist a account
    {
        strcpy(transaction.status,"Failure");
        temp.index = transaction.index; // for if there is no index in the memory already.
        temp.history = history(transaction);
        temp.history_length = temp.history.history_length;
        writeProcess(temp); //writing only the history 
    }
    //printf("withdraw things stuff %d       debgy history, length %d:%s, %s, %d, %s, %s\n",transaction.history_length-1,transaction.history.history_length,transaction.history.accountID,transaction.history.transactionType[transaction.history_length-1],transaction.history.amount[transaction.history_length-1],transaction.history.targetAccountID[transaction.history_length-1],transaction.history.status[transaction.history_length-1]);
    transaction_tostring(transaction);
    //     char temporary[1][10];
    // strcpy(temporary[0], transaction.accountID);
    //history_tostring(temporary,1);
}

void deposit(Transaction transaction) {//char* accountID, int amount
    Transaction temp = readProcess(transaction);
    transaction.history_length=temp.history_length;
    transaction.history = temp.history;
    if(temp.account == transaction.account) 
    {
        strcpy(transaction.status,"Success");
        //printf("382131plz: history index:%d, account#%s, amount%d, condtion %s\n",temp.history.history_length-1,temp.history.accountID,temp.history.amount[temp.history.history_length-1],temp.history.status[temp.history.history_length-1]);
        temp.history = history(transaction);
        temp.amount = temp.amount + transaction.amount;
        temp.history_length = temp.history.history_length;
       // printf("timer timer timer %s",asctime(localtime(&temp.history.time[temp.history.history_length-1])));
        writeProcess(temp); 
    }
    else    //log failre
    {
        strcpy(transaction.status,"Failure");
        temp.index = transaction.index; // for if there is no index in the memory already.
        temp.history = history(transaction);
        temp.history_length = temp.history.history_length;
        writeProcess(temp); //writing only the history 
    }
    //printf("withdraw things stuff        debgy history, length %d:%s, %s, %d, %s, %s\n",temp.history.history_length,temp.history.accountID,temp.history.transactionType[temp.history_length-1],temp.history.amount[temp.history_length-1],temp.history.targetAccountID[temp.history_length-1],temp.history.status[temp.history_length-1]);
    transaction_tostring(temp);
    
}

void withdraw(Transaction transaction) {//char* accountID, int amount
    Transaction temp = readProcess(transaction);
    transaction.history_length=temp.history_length;
    transaction.history = temp.history;
    //printf("the history length is %d\n",temp.history_length);
    if(temp.account == transaction.account) 
    {
        strcpy(transaction.status,"Success");
        temp.history = history(transaction);
        temp.amount = temp.amount - transaction.amount;
        temp.history_length = temp.history.history_length;
        writeProcess(temp); 
    }
    else    //log failre
    {
        strcpy(transaction.status,"Failure");
        temp.index = transaction.index; // for if there is no index in the memory already.
        temp.history = history(transaction);
        temp.history_length = temp.history.history_length;
        //printf("withdraw things stuff   222   debgy history, length %d:%s, %s\n",temp.history.history_length-1,temp.history.accountID,temp.history.transactionType[temp.history_length-1]);
        writeProcess(temp); //writing only the history 
    }
    //printf("withdraw things stuff        debgy history, length %d:%s, %s, %d, %s, %s\n",temp.history.history_length,temp.history.accountID,temp.history.transactionType[temp.history_length-1],temp.history.amount[temp.history_length-1],temp.history.targetAccountID[temp.history_length-1],temp.history.status[temp.history_length-1]);
    transaction_tostring(temp);
    
}

void transfer(Transaction transaction) {//char* accountID, char* targetAccountID, int amount
    //printf("doing trasfer\n");
    // First withdraw from source account
    Transaction transaction2;
    strcpy(transaction.transactionType,"Transfer out ->");
    //printf("doing trasfer post check\n");
    while(search_queue(get_acc_numb(transaction.targetAccountID)));
    Transaction temp = transferCheckin(transaction);  //change this for checking if failure?
    transaction.history_length=temp.history_length;
    transaction.history = temp.history;
    //printf("the history length is %d\n",temp.history_length);
    if(temp.account == transaction.account) 
    {
        strcpy(transaction.status,"Success");
        temp.history = history(transaction);
        temp.amount = temp.amount - transaction.amount;
        temp.history_length = temp.history.history_length;
        //writeProcess(temp); 
    }
    else    //log failre
    {
        strcpy(transaction.status,"Failure");
        temp.index = transaction.index; // for if there is no index in the memory already.
        temp.history = history(transaction);
        temp.history_length = temp.history.history_length;
        //printf("withdraw things stuff   222   debgy history, length %d:%s, %s\n",temp.history.history_length-1,temp.history.accountID,temp.history.transactionType[temp.history_length-1]);
        //writeProcess(temp); //writing only the history 
    }
    transaction2 = transaction;
    strcpy(transaction2.transactionType,"Transfer in <-");
    //printf("doing trasfer half way\n"); // debug
    char tempst[10];
    strcpy(transaction2.accountID,transaction.targetAccountID);
    strcpy(transaction2.targetAccountID,transaction.accountID);
    transaction2.account = get_acc_numb(transaction2.accountID);    //set the index number for use in memory
    transaction2.index = transaction2.account% 71;
    //printf("trasaction `````````````````````````---- index %d\n",transaction2.index); //debug
    Transaction temp2 = readforce(transaction2);
    transaction2.history_length=temp2.history_length;
    transaction2.history = temp2.history;
    transaction2.history_length = transaction2.history.history_length;
     //   transaction_tostring(transaction2);   //debug
    if(temp2.account == transaction2.account) 
    {
        strcpy(transaction2.status,"Success");
        //printf("382131plz: history index:%d, account#%s, amount%d, condtion %s\n",temp.history.history_length-1,temp.history.accountID,temp.history.amount[temp.history.history_length-1],temp.history.status[temp.history.history_length-1]);
        transaction2.history = history(transaction2);
        transaction2.amount = temp2.amount + transaction2.amount;
        transaction2.history_length = transaction2.history.history_length;
       /// writeProcess(transaction2); 
    }
    else    //log failre
    {
        //printf("trascation fialure 2 was  %d, and %d",temp2.account,transaction2.account);
        strcpy(transaction2.status,"Failure");
        transaction2.history = history(transaction2);
        //printf("trans2 history length %d,trans2 history.historyleth %d\n",transaction2.history_length,transaction2.history.history_length);
        transaction2.history_length = transaction2.history.history_length;
        //printf("```trans2 history length %d,trans2 history.historyleth %d\n",transaction2.history_length,transaction2.history.history_length);
        // writeProcess(temp); //writing only the history 
        transaction2.amount=0;
    }
    transaction = temp;
    transaction_tostring(transaction);
    transaction_tostring(transaction2);
    //printf("382131plz:%d  history index:%d, account#%s, amount%d, condtion %s\n",transaction2.history_length,transaction2.history.history_length-1,transaction2.history.accountID,transaction2.history.amount[transaction2.history.history_length-1],transaction2.history.status[transaction2.history.history_length-1]);
    transferrelease(transaction,transaction2);

    
    // Then deposit to the target account
    //deposit(transaction);


}

void inquiry(Transaction transaction) {//char* accountID
    Transaction temp = readProcess(transaction);// store 0 account id if failure
    transaction.history_length=temp.history_length;
    transaction.history = temp.history;
    if(temp.account == transaction.account)     //if the accoutn exist
    {
        strcpy(transaction.status,"Success");
        temp.history = history(transaction);
        temp.history_length = temp.history.history_length;
        temp.history.amount[temp.history.history_length-1] = temp.amount;  // writing the current value to history
        writeProcess(temp); 
    }
    else
    {
        temp.index = transaction.index; // for if there is no index in the memory already.
        strcpy(transaction.accountID,"\0");
        strcpy(transaction.status,"Failure");
        temp.history = history(transaction);
        temp.history_length = temp.history.history_length;
        writeProcess(temp); 
    }
    //printf("withdraw things stuff        debgy history, length %d:%s, %s, %d, %s, %s\n",temp.history.history_length,temp.history.accountID,temp.history.transactionType[temp.history_length-1],temp.history.amount[temp.history_length-1],temp.history.targetAccountID[temp.history_length-1],temp.history.status[temp.history_length-1]);
    transaction_tostring(temp);
    //we read the shared memory!
    
}

void closeAccount(Transaction transaction) {//char* accountID
    //printf("index thing,%d\n",transaction.account); debuging
    Transaction temp = readProcess(transaction);    //store history, setting accoutn status to closed, account to 0 and accoutn to null
    transaction.history_length=temp.history_length;
    transaction.history = temp.history;
    if(temp.account == transaction.account)    //if the accoutn exits
    {
        //printf("deleted acount %s\n",transaction.accountID);
        strcpy(transaction.status,"Success");
        int var;
        temp.account = var;
        temp.amount =0;
        strcpy(temp.accountID,"\0");
        strcpy(temp.status,"Closed");
        temp.history = history(transaction);
        temp.history_length = temp.history.history_length;
        //writeProcess(temp); 
        //printf("close account things stuff        debgy history, length %d:%s, %s, %d, %s, %s\n",temp.history.history_length,temp.history.accountID,temp.history.transactionType[temp.history_length-1],temp.history.amount[temp.history_length-1],temp.history.targetAccountID[temp.history_length-1],temp.history.status[temp.history_length-1]);
    
    }
    else
    {
        //printf("deleted acount failure %s,temp: %s,status:%d\n",transaction.accountID,temp.accountID,strcmp(temp.accountID,transaction.accountID));
        temp.index = transaction.index; // for if there is no index in the memory already.
        strcpy(transaction.status,"Failure");
        temp.history = history(transaction);
        temp.history_length = temp.history.history_length;
        strcpy(temp.accountID,"\0");
        //writeProcess(temp); 
    }
    //printf("withdraw things stuff        debgy history, length %d:%s, %s, %d, %s, %s\n",temp.history.history_length,temp.history.accountID,temp.history.transactionType[temp.history_length-1],temp.history.amount[temp.history_length-1],temp.history.targetAccountID[temp.history_length-1],temp.history.status[temp.history_length-1]);
    transaction_tostring(temp);
    //modify me. add a closed account thing...
    writeProcess(temp); //
   
}

int get_acc_numb(char *str) {   //converts account number like A123 to int 123 for index
    char temp[10];
    strcpy(temp,str);
    if (strlen(temp) > 0) {
        memmove(temp, temp + 1, strlen(temp));
       int ret = atoi(temp); 
       return ret;    // hashing function to keep it within memory, random number for prevntion of out of addressing, yes its small.
    }
    return 0;
}