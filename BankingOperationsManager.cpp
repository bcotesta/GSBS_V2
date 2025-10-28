#include "BankingOperationsManager.h"
#include "UIManager.h"
#include <iostream>

using namespace std;

// Constructor to set members
BankingOperationsManager::BankingOperationsManager(AccountManager& accountMgr, std::vector<Account>& accounts)
    : accountMgr_(accountMgr), accounts_(accounts)
{
}

// Account selection method
Account* BankingOperationsManager::selectAccount() {
    if (accounts_.empty()) {
        UIManager::showError("No accounts available.");
        return nullptr;
    }
    
    cout << "\n=== Select an Account ===" << endl;
    for (size_t i = 0; i < accounts_.size(); ++i) {
        cout << (i + 1) << ". Account #" << accounts_[i].accountNumber()
             << " - Balance: $" << accounts_[i].getBalance() << endl;
    }
    cout << "Enter account number (1-" << accounts_.size() << "): ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice < 1 || choice > static_cast<int>(accounts_.size())) {
        UIManager::showError("Invalid account selection.");
        return nullptr;
    }
    
    return &accounts_[choice - 1];
}

// for console input of amount
bool BankingOperationsManager::promptAmount(const std::string& prompt, double& amount) {
    cout << prompt;
    cin >> amount;
    cin.ignore();
    return cin.good();
}

// console deposit handler
void BankingOperationsManager::handleDeposit() {
    Account* account = selectAccount();
    if (!account) return;
    
    double amount;
    if (!promptAmount("Enter deposit amount: $", amount)) {
        UIManager::showError("Invalid amount.");
        return;
    }
    
    if (accountMgr_.deposit(*account, amount)) {
        UIManager::showSuccess("Deposit completed successfully!");
    } else {
        UIManager::showError("Deposit failed.");
    }
}

// console  for withdrawal
void BankingOperationsManager::handleWithdraw() {
    Account* account = selectAccount();
    if (!account) return;
    
    cout << "Current balance: $" << account->getBalance() << endl;
    
    double amount;
    if (!promptAmount("Enter withdrawal amount: $", amount)) {
        UIManager::showError("Invalid amount.");
        return;
    }
    
    if (accountMgr_.withdraw(*account, amount)) {
        UIManager::showSuccess("Withdrawal completed successfully!");
    } else {
        UIManager::showError("Withdrawal failed.");
    }
}

// console transfer
void BankingOperationsManager::handleTransfer() {
    if (accounts_.size() < 2) {
        UIManager::showError("You need at least 2 accounts to transfer funds.");
        return;
    }
    
    cout << "\n--- Select SOURCE account ---" << endl;
    Account* fromAccount = selectAccount();
    if (!fromAccount) return;
    
    cout << "\n--- Select DESTINATION account ---" << endl;
    Account* toAccount = selectAccount();
    if (!toAccount) return;
    
    if (fromAccount == toAccount) {
        UIManager::showError("Cannot transfer to the same account.");
        return;
    }
    
    cout << "Source account balance: $" << fromAccount->getBalance() << endl;
    
    double amount;
    if (!promptAmount("Enter transfer amount: $", amount)) {
        UIManager::showError("Invalid amount.");
        return;
    }
    
    if (accountMgr_.transfer(*fromAccount, *toAccount, amount)) {
        UIManager::showSuccess("Transfer completed successfully!");
    } else {
        UIManager::showError("Transfer failed.");
    }
}

// console account creation
void BankingOperationsManager::handleCreateAccount() {
    AccountType type = UIManager::promptAccountType();
    Account newAccount = accountMgr_.createAccount(type);
    accounts_.push_back(newAccount);
    UIManager::showSuccess("Account created successfully!");
}

void BankingOperationsManager::handleViewAccounts() {
    UIManager::displayAccountList(accounts_);
}

void BankingOperationsManager::handleViewSummary() {
    accountMgr_.displayAccountSummary(accounts_);
}