// Brandon Cotesta | 10/27/2025 | 8:43 PM
// MOOD: Neutral. not excited to spend the night doing this
// but ready to get it done. Blue Jays are down 1-0 >:(.

#include "SessionManager.h"
#include "AccountManager.h"
#include "UIManager.h"
#include "PageManager.h"
#include "BankingOperationsManager.h"

#include <QApplication>
#include <vector>
#include <iostream>

using namespace std;

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
    
    // Step 4: Initialize banking operations manager
    BankingOperationsManager bankingOps(accountMgr, accounts);
    
    // Main menu loop
    bool running = true;
    while (running) {
        displayMainMenu();
        
        int menuChoice;
        cin >> menuChoice;
        cin.ignore(); // Clear newline from buffer
        
        switch (menuChoice) {
            case 1: // View All Accounts
                bankingOps.handleViewAccounts();
                break;
                
            case 2: // Deposit
                bankingOps.handleDeposit();
                break;
                
            case 3: // Withdraw
                bankingOps.handleWithdraw();
                break;
                
            case 4: // Transfer
                bankingOps.handleTransfer();
                break;
                
            case 5: // Create Account
                bankingOps.handleCreateAccount();
                break;
                
            case 6: // View Summary
                bankingOps.handleViewSummary();
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
    QApplication app(argc, argv);
    
    UIManager::displayWelcome();
    runBankingApp();
    
    return app.exec();
}

