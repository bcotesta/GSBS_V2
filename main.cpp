// Brandon Cotesta | 10/27/2025 | 8:43 PM
// MOOD: Neutral. not excited to spend the night doing this
// but ready to get it done. Blue Jays are down 1-0 >:(.

#include "SessionManager.h"
#include "AccountManager.h"
#include "UIManager.h"
#include <QtCore/QCoreApplication>
#include <vector>
#include <iostream>

using namespace std;

// ==================== HELPER FUNCTION ====================

// Helper to select an account from the list
static Account* selectAccount(vector<Account>& accounts) {
    if (accounts.empty()) {
        UIManager::showError("No accounts available.");
        return nullptr;
    }
    
    cout << "\n=== Select an Account ===" << endl;
    for (size_t i = 0; i < accounts.size(); ++i) {
        cout << (i + 1) << ". Account #" << accounts[i].accountNumber()
             << " - Balance: $" << accounts[i].getBalance() << endl;
    }
    cout << "Enter account number (1-" << accounts.size() << "): ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice < 1 || choice > static_cast<int>(accounts.size())) {
        UIManager::showError("Invalid account selection.");
        return nullptr;
    }
    
    return &accounts[choice - 1];
}

// ==================== BANKING OPERATIONS ====================

static void handleDeposit(AccountManager& accountMgr, vector<Account>& accounts) {
    Account* account = selectAccount(accounts);
    if (!account) return;
    
    cout << "Enter deposit amount: $";
    double amount;
    cin >> amount;
    cin.ignore();
    
    if (accountMgr.deposit(*account, amount)) {
        UIManager::showSuccess("Deposit completed successfully!");
    } else {
        UIManager::showError("Deposit failed.");
    }
}

static void handleWithdraw(AccountManager& accountMgr, vector<Account>& accounts) {
    Account* account = selectAccount(accounts);
    if (!account) return;
    
    cout << "Current balance: $" << account->getBalance() << endl;
    cout << "Enter withdrawal amount: $";
    double amount;
    cin >> amount;
    cin.ignore();
    
    if (accountMgr.withdraw(*account, amount)) {
        UIManager::showSuccess("Withdrawal completed successfully!");
    } else {
        UIManager::showError("Withdrawal failed.");
    }
}

static void handleTransfer(AccountManager& accountMgr, vector<Account>& accounts) {
    if (accounts.size() < 2) {
        UIManager::showError("You need at least 2 accounts to transfer funds.");
        return;
    }
    
    cout << "\n--- Select SOURCE account ---" << endl;
    Account* fromAccount = selectAccount(accounts);
    if (!fromAccount) return;
    
    cout << "\n--- Select DESTINATION account ---" << endl;
    Account* toAccount = selectAccount(accounts);
    if (!toAccount) return;
    
    if (fromAccount == toAccount) {
        UIManager::showError("Cannot transfer to the same account.");
        return;
    }
    
    cout << "Source account balance: $" << fromAccount->getBalance() << endl;
    cout << "Enter transfer amount: $";
    double amount;
    cin >> amount;
    cin.ignore();
    
    if (accountMgr.transfer(*fromAccount, *toAccount, amount)) {
        UIManager::showSuccess("Transfer completed successfully!");
    } else {
        UIManager::showError("Transfer failed.");
    }
}

static void handleCreateAccount(AccountManager& accountMgr, vector<Account>& accounts) {
    AccountType type = UIManager::promptAccountType();
    Account newAccount = accountMgr.createAccount(type);
    accounts.push_back(newAccount);
    UIManager::showSuccess("Account created successfully!");
}

// ==================== MAIN MENU ====================

static void displayMainMenu() {
    cout << "\n========================================" << endl;
    cout << "           BANKING MAIN MENU" << endl;
    cout << "========================================" << endl;
    cout << "1. View All Accounts" << endl;
    cout << "2. Deposit Funds" << endl;
    cout << "3. Withdraw Funds" << endl;
    cout << "4. Transfer Funds" << endl;
    cout << "5. Create New Account" << endl;
    cout << "6. View Account Summary" << endl;
    cout << "7. Logout" << endl;
    cout << "========================================" << endl;
    cout << "Enter choice: ";
}

static void runBankingApp() {
    SessionManager session;
    
    // Offer login or registration
    cout << "\n=== Welcome to GSBS Banking System ===" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register New Account" << endl;
    cout << "Enter choice (1-2): ";
    
    int choice;
    cin >> choice;
    cin.ignore(); // Clear newline from buffer
    
    bool success = false;
    
    if (choice == 1) {
        // Existing login
        success = session.login();
        if (!success) {
            UIManager::showError("Login failed. Exiting...");
            return;
        }
    }
    else if (choice == 2) {
        // New registration
        success = session.registerUser();
        if (!success) {
            UIManager::showError("Registration failed. Exiting...");
            return;
        }
    }
    else {
        UIManager::showError("Invalid choice. Exiting...");
        return;
    }
    
    User* user = session.getCurrentUser();
    UIManager::displayUserInfo(*user);
    
    // Step 2: Initialize account manager
    AccountManager accountMgr(*user);
    
    // Step 3: Load existing accounts
    vector<Account> accounts = accountMgr.loadUserAccounts();
    
    // Main menu loop
    bool running = true;
    while (running) {
        displayMainMenu();
        
        int menuChoice;
        cin >> menuChoice;
        cin.ignore(); // Clear newline from buffer
        
        switch (menuChoice) {
            case 1: // View All Accounts
                UIManager::displayAccountList(accounts);
                break;
                
            case 2: // Deposit
                handleDeposit(accountMgr, accounts);
                break;
                
            case 3: // Withdraw
                handleWithdraw(accountMgr, accounts);
                break;
                
            case 4: // Transfer
                handleTransfer(accountMgr, accounts);
                break;
                
            case 5: // Create Account
                handleCreateAccount(accountMgr, accounts);
                break;
                
            case 6: // View Summary
                accountMgr.displayAccountSummary(accounts);
                break;
                
            case 7: // Logout
                UIManager::showInfo("Logging out...");
                session.logout();
                running = false;
                break;
                
            default:
                UIManager::showError("Invalid choice. Please try again.");
                break;
        }
    }
    
    UIManager::showSuccess("Thank you for using GSBS Banking System!");
}

// ==================== MAIN ENTRY POINT ====================

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    UIManager::displayWelcome();
    runBankingApp();
    
    return app.exec();
}

