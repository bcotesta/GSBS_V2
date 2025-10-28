#pragma once
#include "Account.h"
#include "DatabaseManager.h"
#include "User.h"
#include <vector>
#include <string>

class AccountManager {
public:
    AccountManager(const User& user);
    
    // Account operations
    std::vector<Account> loadUserAccounts();
    Account createAccount(AccountType type);
    void saveAccount(const Account& account);
    void displayAccountSummary(const std::vector<Account>& accounts);
    
    // Utility
    std::string getAccountsTableName() const;
    std::string getTransactionsTableName() const;
    
private:
    const User& user_;
    std::string accountsTableName_;
    std::string transactionsTableName_;
    
    void initializeTableNames();
    void ensureTablesExist();
};