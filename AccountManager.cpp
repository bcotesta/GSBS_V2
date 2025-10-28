#include "AccountManager.h"
#include "Authenticator.h"
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

// Helper function to generate a random account number
static string generateAccountNumber() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(100000000, 999999999);
    return to_string(dis(gen));
}

// Helper function to convert string to AccountType enum
static AccountType stringToAccountType(const string& typeStr) {
    if (typeStr == "CHEQUING") return AccountType::CHEQUING;
    if (typeStr == "SAVINGS") return AccountType::SAVINGS;
    if (typeStr == "CREDIT") return AccountType::CREDIT;
    if (typeStr == "LOAN") return AccountType::LOAN;
    return AccountType::CHEQUING; // Default
}

// Helper function to sanitize name for table construction
static string sanitizeName(const string& name) {
    string cleanName = name;
    cleanName.erase(remove(cleanName.begin(), cleanName.end(), ' '), cleanName.end());
    cleanName.erase(remove(cleanName.begin(), cleanName.end(), '@'), cleanName.end());
    cleanName.erase(remove(cleanName.begin(), cleanName.end(), '-'), cleanName.end());
    cleanName.erase(remove(cleanName.begin(), cleanName.end(), '.'), cleanName.end());
    return cleanName;
}

AccountManager::AccountManager(const User& user)
    : user_(user)
{
    initializeTableNames();
    ensureTablesExist();
}

void AccountManager::initializeTableNames() {
    string cleanName = sanitizeName(user_.name());
    string userIdStr = to_string(user_.userId());
    
    accountsTableName_ = userIdStr + "_" + cleanName + "_acc";
    transactionsTableName_ = userIdStr + "_" + cleanName + "_transactions";
}

void AccountManager::ensureTablesExist() {
    DatabaseManager& db = DatabaseManager::getInstance();
    Authenticator& auth = Authenticator::getInstance();
    
    cout << "Checking for user tables..." << endl;
    cout << "Looking for table: " << accountsTableName_ << endl;
    
    // Check if table exists by querying information_schema
    string whereClause = "TABLE_SCHEMA = 'bankdatabase' AND TABLE_NAME = '" + accountsTableName_ + "'";
    auto tableCheck = db.retrieveTable("information_schema.TABLES", whereClause);
    
    if (tableCheck.empty()) {
        cout << "Accounts table not found. Creating..." << endl;
        db.createUserAccountsTable(auth.getUserID(), user_.name());
        db.createUserTransactionsTable(auth.getUserID(), user_.name());
        cout << "Tables created successfully." << endl;
    } else {
        cout << "Accounts table exists." << endl;
    }
}

vector<Account> AccountManager::loadUserAccounts() {
    DatabaseManager& db = DatabaseManager::getInstance();
    vector<Account> accounts;
    
    auto accResults = db.retrieveTable(accountsTableName_, "");
    cout << "Found " << accResults.size() << " existing account(s)." << endl;
    
    if (!accResults.empty()) {
        cout << "\n=== Loading Existing Accounts ===" << endl;
        for (const auto& accountData : accResults) {
            // Extract account information from database result
            string accNumber = static_cast<string>(accountData.at("accountNumber"));
            string accTypeStr = static_cast<string>(accountData.at("accountType"));
            string balanceStr = static_cast<string>(accountData.at("balance"));
            
            // Convert account type string to enum
            AccountType accType = stringToAccountType(accTypeStr);
            
            // Create Account object
            Account acc(accNumber, accType);
            
            // TODO: Set balance if setter is added to Account class
            // acc.setBalance(std::stod(balanceStr));
            
            accounts.push_back(acc);
            
            cout << "Loaded account: " << accNumber 
                 << " (" << accTypeStr << ") - Balance: $" << balanceStr << endl;
        }
        cout << "Total accounts loaded: " << accounts.size() << endl;
    }
    
    return accounts;
}

Account AccountManager::createAccount(AccountType type) {
    string accountNumber = generateAccountNumber();
    
    Account newAccount(accountNumber, type);
    cout << "Created account: " << accountNumber << endl;
    
    // Save to database
    newAccount.saveToDatabase(user_.name());
    
    return newAccount;
}

void AccountManager::saveAccount(const Account& account) {
    // The account should already have its own saveToDatabase method
    // This is a wrapper if needed
    cout << "Saving account: " << account.accountNumber() << endl;
}

void AccountManager::displayAccountSummary(const vector<Account>& accounts) {
    DatabaseManager& db = DatabaseManager::getInstance();
    
    cout << "\n=== Account Summary ===" << endl;
    
    // Get latest data from database
    auto accResults = db.retrieveTable(accountsTableName_, "");
    cout << "Total accounts in database: " << accResults.size() << endl;
    
    // Display all accounts from database
    for (const auto& account : accResults) {
        cout << "\nAccount Details:" << endl;
        cout << "  Account Number: " << static_cast<string>(account.at("accountNumber")) << endl;
        cout << "  Account Type: " << static_cast<string>(account.at("accountType")) << endl;
        cout << "  Balance: $" << static_cast<string>(account.at("balance")) << endl;
        cout << "  Created Date: " << static_cast<string>(account.at("createdDate")) << endl;
    }
    
    // Display in-memory account objects
    cout << "\n=== Account Objects in Memory ===" << endl;
    cout << "You have " << accounts.size() << " Account objects loaded." << endl;
    for (size_t i = 0; i < accounts.size(); ++i) {
        cout << i+1 << ". Account #" << accounts[i].accountNumber() 
             << " - Balance: $" << accounts[i].getBalance() << endl;
    }
}

string AccountManager::getAccountsTableName() const {
    return accountsTableName_;
}

string AccountManager::getTransactionsTableName() const {
    return transactionsTableName_;
}