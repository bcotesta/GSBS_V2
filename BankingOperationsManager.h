#pragma once
#include "AccountManager.h"
#include "Account.h"
#include "Types.h"
#include <vector>

class BankingOperationsManager {
public:
    BankingOperationsManager(AccountManager& accountMgr, std::vector<Account>& accounts);
    
    // Account selection
    Account* selectAccount();
    
    // Banking operations
    void handleDeposit();
    void handleWithdraw();
    void handleTransfer();
    void handleCreateAccount();
    void handleViewAccounts();
    void handleViewSummary();
    
private:
    AccountManager& accountMgr_;
    std::vector<Account>& accounts_;
    
    // Helper for amount input
    bool promptAmount(const std::string& prompt, double& amount);
};