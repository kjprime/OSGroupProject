Monitor AccountOperationsMonitor:
    Lock accountLock // This lock ensures mutual exclusion.

    // Create a some form of Queue or waiting.
    ConditionVariable sufficientBalanceCondition

    // Create Syncornizaiton for each function.
    Function CreateAccountSync(accountID):
        accountLock.Acquire()
        CreateAccount(accountID) // Call the Create Account function.
        accountLock.Release()

    Function DepositSync(accountID, amount):
        accountLock.Acquire()
        Deposit(accountID, amount) // Call the Deposit function.
        sufficientBalanceCondition.Signal() // Signal the withdraw and transfers functions that could be waiting.
        accountLock.Release()

    Function WithdrawSync(accountID, amount):
        accountLock.Acquire()
        While AccountsDatabase[accountID] < amount: // Wait until True.
            sufficientBalanceCondition.Wait(accountLock)
        Withdraw(accountID, amount) // Call the Withdraw function.
        accountLock.Release()

    Function TransferSync(fromAccountID, toAccountID, amount):
        accountLock.Acquire()
        While AccountsDatabase[fromAccountID] < amount: // Wait until true.
            sufficientBalanceCondition.Wait(accountLock)
        Transfer(fromAccountID, toAccountID, amount) // Call the Transfer function.
        sufficientBalanceCondition.Signal() // Signal the  Balance function if it is waiting.
        accountLock.Release()

    Function BalanceSync(accountID):
        accountLock.Acquire()
        Balance(accountID) // Call the Balance function.
        accountLock.Release()

    Function CloseAccSync(accountID):
        accountLock.Acquire()
        CloseAcc(accountID) // Call the Close account function.
        accountLock.Release()
