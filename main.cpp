// Brandon Cotesta | 10/27/2025 | 8:43 PM
// MOOD: Neutral. not excited to spend the night doing this
// but ready to get it done. Blue Jays are down 1-0 >:(.

#include "SessionManager.h"
#include "AccountManager.h"
#include "UIManager.h"
#include <QtCore/QCoreApplication>
#include <vector>

using namespace std;

// ==================== BANKING APP RUN FUNCTION ====================

static void runBankingApp() {
    // Step 1: Handle login
    SessionManager session;
    if (!session.login()) {
        UIManager::showError("Login failed. Exiting...");
        return;
    }
    
    User* user = session.getCurrentUser();
    UIManager::displayUserInfo(*user);
    
    // Step 2: Initialize account manager
    AccountManager accountMgr(*user);
    
    // Step 3: Load existing accounts
    vector<Account> accounts = accountMgr.loadUserAccounts();
    UIManager::displayAccountList(accounts);
    
    // Step 4: Create new account if desired
    if (UIManager::promptYesNo("Would you like to create a new account?")) {
        AccountType type = UIManager::promptAccountType();
        Account newAccount = accountMgr.createAccount(type);
        accounts.push_back(newAccount);
        UIManager::showSuccess("Account created successfully!");
    }
    
    // Step 5: Display final summary
    accountMgr.displayAccountSummary(accounts);
}

// ==================== MAIN ENTRY POINT ====================

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    UIManager::displayWelcome();
    runBankingApp();
    
    return app.exec();
}

