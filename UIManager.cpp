#include "UIManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

void UIManager::displayWelcome() {
    cout << "========================================" << endl;
    cout << "   Welcome to GSBS Banking System" << endl;
    cout << "========================================" << endl;
    cout << endl;
}

void UIManager::displayUserInfo(const User& user) {
    cout << "\n=== User Information ===" << endl;
    cout << "User ID: " << user.userId() << endl;
    cout << "Name: " << user.name() << endl;
    cout << "Email: " << user.email() << endl;
    cout << "Phone: " << user.phone() << endl;
    cout << "=========================" << endl;
}

void UIManager::displayAccountDetails(const Account& account) {
    cout << "\n--- Account Details ---" << endl;
    cout << "Account Number: " << account.accountNumber() << endl;
    
    // Convert AccountType enum to string
    string typeStr;
    switch (account.accountType()) {
        case AccountType::CHEQUING:
            typeStr = "CHEQUING";
            break;
        case AccountType::SAVINGS:
            typeStr = "SAVINGS";
            break;
        case AccountType::CREDIT:
            typeStr = "CREDIT";
            break;
        case AccountType::LOAN:
            typeStr = "LOAN";
            break;
    }
    
    cout << "Account Type: " << typeStr << endl;
    cout << fixed << setprecision(2);
    cout << "Balance: $" << account.getBalance() << endl;
    cout << "-----------------------" << endl;
}

void UIManager::displayAccountList(const vector<Account>& accounts) {
    if (accounts.empty()) {
        cout << "\nNo accounts found." << endl;
        return;
    }
    
    cout << "\n=== Your Accounts ===" << endl;
    for (size_t i = 0; i < accounts.size(); ++i) {
        cout << "\n" << (i + 1) << ". ";
        displayAccountDetails(accounts[i]);
    }
    cout << "=====================" << endl;
}

bool UIManager::promptYesNo(const string& question) {
    cout << "\n" << question << " (y/n): ";
    string response;
    getline(cin, response);
    
    return !response.empty() && (response[0] == 'y' || response[0] == 'Y');
}

AccountType UIManager::promptAccountType() {
    cout << "\n=== Select Account Type ===" << endl;
    cout << "1. CHEQUING" << endl;
    cout << "2. SAVINGS" << endl;
    cout << "3. CREDIT" << endl;
    cout << "4. LOAN" << endl;
    cout << "Enter choice (1-4): ";
    
    int choice;
    cin >> choice;
    cin.ignore(); // Clear newline from buffer
    
    switch (choice) {
        case 1:
            return AccountType::CHEQUING;
        case 2:
            return AccountType::SAVINGS;
        case 3:
            return AccountType::CREDIT;
        case 4:
            return AccountType::LOAN;
        default:
            cout << "Invalid choice. Defaulting to CHEQUING." << endl;
            return AccountType::CHEQUING;
    }
}

int UIManager::promptMenuChoice(const vector<string>& options) {
    cout << "\n=== Menu ===" << endl;
    for (size_t i = 0; i < options.size(); ++i) {
        cout << (i + 1) << ". " << options[i] << endl;
    }
    cout << "Enter choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore(); // Clear newline from buffer
    
    if (choice < 1 || choice > static_cast<int>(options.size())) {
        cout << "Invalid choice." << endl;
        return -1;
    }
    
    return choice;
}

void UIManager::showError(const string& message) {
    cout << "\n[ERROR] " << message << endl;
}

void UIManager::showSuccess(const string& message) {
    cout << "\n[SUCCESS] " << message << endl;
}

void UIManager::showInfo(const string& message) {
    cout << "\n[INFO] " << message << endl;
}