#pragma once
#include "Account.h"
#include "DatabaseManager.h"
#include "User.h"
#include <vector>
#include <string>

class AccountManager {
public:
    // Constructor that initializes the manager with a specific user
    AccountManager(const User& user);
    
    // Account operations
    // Loads all accounts belonging to the current user from the database and returns a vector of Account objects.
    std::vector<Account> loadUserAccounts();
    // Creates a new account of the specified type and inserts it into the database.
    Account createAccount(AccountType type);
	// Displays a summary of all accounts to the console.
    void displayAccountSummary(const std::vector<Account>& accounts);
    
    // Banking operations
    // Deposits funds into the given account and returns true if successful and false if validation fails.
    bool deposit(Account& account, double amount);
    // Withdraws funds from the given account and returns true if the account has sufficient balance.
    bool withdraw(Account& account, double amount);
    // Transfers funds between two accounts and eturns true if the transaction succeeds.
    bool transfer(Account& fromAccount, Account& toAccount, double amount);
    
    // Database sync operations
    // Updates the account's balance in the database after deposits, withdrawals, or transfers.
    void syncAccountToDatabase(Account& account);
	// Records a transaction in the user's transactions table in the database.
    void recordTransaction(const Account& account, double amount, 
                          TransactionType type, const std::string& description);
    
    // Utility
    // Returns the database table name that stores the user's accounts.
    std::string getAccountsTableName() const;
	// Returns the database table name that stores the user's transactions.
    std::string getTransactionsTableName() const;
    
private:
    // Reference to the authenticated user who owns the accounts.
    const User& user_;

    std::string accountsTableName_;
    std::string transactionsTableName_;
    
    void initializeTableNames();
    // Ensures that the required tables exist in the database
    void ensureTablesExist();
};