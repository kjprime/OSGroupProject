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
// Global data structure to hold account information.
// All Sudo Code. Simply a representation of how the program will be laid out, but not including actual implementaion.
AccountInfo = {}

// Function to create a new account with an initial balance of 0.
Function CreateAccount(accountID):
    AccountInfo[accountID] = 0
    Print("Account created with ID:", accountID)

// Function to deposit money into an account.
Function Deposit(accountID, amount):
    If accountID not in AccountInfo:
        Print("Account does not exist.")
        Return
    AccountInfo[accountID] += amount
    Print(amount, "deposited into account ID:", accountID)

// Function to withdraw money from an account.
Function Withdraw(accountID, amount):
    If accountID not in AccountInfo or AccountInfo[accountID] < amount:
        Print("Insufficient funds or account does not exist.")
        Return
    AccountInfo[accountID] -= amount
    Print(amount, "withdrawn from account ID:", accountID)

// Function to transfer money between accounts.
Function Transfer(fromAccountID, toAccountID, amount):
    If fromAccountID not in AccountInfo or toAccountID not in AccountInfo or AccountsDatabase[fromAccountID] < amount:
        Print("Insufficient funds or account does not exist.")
        Return
    AccountInfo[fromAccountID] -= amount
    AccountInfo[toAccountID] += amount
    Print(amount, "transferred from account ID:", fromAccountID, "to account ID:", toAccountID)

// Function to check the balance of an account.
Function Balance(accountID):
    If accountID not in AccountInfo:
        Print("Account does not exist.")
        Return
    Print("The balance of account ID:", accountID, "is", AccountInfo[accountID])

// Function to close an account.
Function CloseAcc(accountID):
    If accountID not in AccountsInfo:
        Print("Account does not exist.")
        Return
    Delete AccountInfo[accountID]
    Print("Account ID:", accountID, "has been closed.")
