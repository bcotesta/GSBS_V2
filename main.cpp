// Brandon Cotesta | 10/27/2025 | 8:43 PM
// MOOD: Neutral. not excited to spend the night doing this
// but ready to get it done. Blue Jays are down 1-0 >:(.

// VERY IMPORTANT NOTE - ALL PAGE STUFF MUST BE DONE IN MAINWINDOW.CPP
// THAT IS WHERE YOU ADD THE RESPECTIVE PAGES TO BE USED IN THE APP

#include "SessionManager.h"
#include "AccountManager.h"
#include "UIManager.h"
#include "PageManager.h"
#include "BankingOperationsManager.h"
#include "MainWindow.h"
#include "User.h"

#include <QApplication>
#include <QFont>
#include <vector>
#include <iostream>

using namespace std;

// ==================== QT WINDOW INITIALIZATION ====================

static void initializeQtWindow(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Set application style
    app.setStyle("Fusion");
    
    // Set application-wide font
    QFont appFont("Segoe UI", 10);
    app.setFont(appFont);

    // Create page manager - MUST outlive mainWindow
    PageManager pageManager;

    // Create and show main window - use scope or pointer to control lifetime
    {
        MainWindow mainWindow(&pageManager);
        mainWindow.show();

        // Run application event loop
        app.exec();
    } // mainWindow destructs here BEFORE pageManager
}

// ==================== CONSOLE-BASED BANKING APP (Legacy) ====================
// i'm keeping this here for debug purposes, and using actual banking functions
// while w build GUI

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
    // Choose between Qt GUI or console mode
    cout << "=== GSBS Banking System ===" << endl;
    cout << "Select mode:" << endl;
    cout << "1. GUI Mode (Mobile-like Interface)" << endl;
    cout << "2. Console Mode (Legacy)" << endl;
    cout << "Enter choice (1-2): ";
    
    int modeChoice;
    cin >> modeChoice;
    cin.ignore();
    
    if (modeChoice == 1) {
        // Launch Qt GUI
        initializeQtWindow(argc, argv);
    } else {
        // Launch console app
        QApplication app(argc, argv); 
        UIManager::displayWelcome();
        runBankingApp();
    }
    
    return 0;
}

