// Brandon Cotesta | 10/28/2025 | 2:00 PM


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

// initialize table names based on user info
void AccountManager::initializeTableNames() {
    string cleanName = sanitizeName(user_.name());
    string userIdStr = to_string(user_.userId());
    
    accountsTableName_ = userIdStr + "_" + cleanName + "_acc";
    transactionsTableName_ = userIdStr + "_" + cleanName + "_transactions";
}

// ensure user-specific tables exist, create if not
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

// load existing accounts from database
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
            
            // Set balance from database
            acc.setBalance(std::stod(balanceStr));
            
            accounts.push_back(acc);
            
            cout << "Loaded account: " << accNumber 
                 << " (" << accTypeStr << ") - Balance: $" << balanceStr << endl;
        }
        cout << "Total accounts loaded: " << accounts.size() << endl;
    }
    
    return accounts;
}

// create a new account and save to database
Account AccountManager::createAccount(AccountType type) {
    string accountNumber = generateAccountNumber();
    
    Account newAccount(accountNumber, type);
    cout << "Created account: " << accountNumber << endl;
    
    // Save to database
    newAccount.saveToDatabase(user_.name());
    
    return newAccount;
}

// display summary of accounts to console
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

// perform deposit operation
bool AccountManager::deposit(Account& account, double amount) {
    if (amount <= 0) {
        cerr << "Deposit amount must be positive." << endl;
        return false;
    }
    
    // 1. Perform deposit on account object (updates in-memory balance)
    account.deposit(amount);
    
    // 2. Sync updated balance to database
    syncAccountToDatabase(account);
    
    // 3. Record transaction in transactions table
    recordTransaction(account, amount, TransactionType::DEPOSIT, 
                     "Deposit to account " + account.accountNumber());
    
    cout << "[SUCCESS] Deposited $" << amount << " to account " 
         << account.accountNumber() << endl;
    cout << "New balance: $" << account.getBalance() << endl;
    
    return true;
}

// perform withdrawal operation
bool AccountManager::withdraw(Account& account, double amount) {
    if (amount <= 0) {
        cerr << "Withdrawal amount must be positive." << endl;
        return false;
    }
    
    if (amount > account.getBalance()) {
        cerr << "Insufficient funds. Current balance: $" 
             << account.getBalance() << endl;
        return false;
    }
    
    // 1. Perform withdrawal on account object
    account.withdraw(amount);
    
    // 2. Sync updated balance to database
    syncAccountToDatabase(account);
    
    // 3. Record transaction
    recordTransaction(account, amount, TransactionType::WITHDRAWAL,
                     "Withdrawal from account " + account.accountNumber());
    
    cout << "[SUCCESS] Withdrew $" << amount << " from account " 
         << account.accountNumber() << endl;
    cout << "New balance: $" << account.getBalance() << endl;
    
    return true;
}

// perform transfer operation between two accounts
bool AccountManager::transfer(Account& fromAccount, Account& toAccount, double amount) {
    if (amount <= 0) {
        cerr << "Transfer amount must be positive." << endl;
        return false;
    }
    
    if (amount > fromAccount.getBalance()) {
        cerr << "Insufficient funds for transfer. Current balance: $" 
             << fromAccount.getBalance() << endl;
        return false;
    }
    
    // 1. Withdraw from source account
    fromAccount.withdraw(amount);
    
    // 2. Deposit to destination account
    toAccount.deposit(amount);
    
    // 3. Sync both accounts to database
    syncAccountToDatabase(fromAccount);
    syncAccountToDatabase(toAccount);
    
    // 4. Record transactions for both accounts
    recordTransaction(fromAccount, amount, TransactionType::TRANSFER,
                     "Transfer to account " + toAccount.accountNumber());
    recordTransaction(toAccount, amount, TransactionType::TRANSFER,
                     "Transfer from account " + fromAccount.accountNumber());
    
    cout << "[SUCCESS] Transferred $" << amount 
         << " from account " << fromAccount.accountNumber()
         << " to account " << toAccount.accountNumber() << endl;
    cout << "Source account new balance: $" << fromAccount.getBalance() << endl;
    cout << "Destination account new balance: $" << toAccount.getBalance() << endl;
    
    return true;
}

// sync account balance to database
void AccountManager::syncAccountToDatabase(Account& account) {
    // Update the balance in the accounts table
    account.updateBalanceInDatabase(accountsTableName_);
}

// record a transaction in the transactions table
void AccountManager::recordTransaction(const Account& account, double amount,
                                      TransactionType type, const std::string& description) {
    DatabaseManager& db = DatabaseManager::getInstance();
    
    // Convert transaction type to string
    string typeStr;
    switch (type) {
        case TransactionType::DEPOSIT:
            typeStr = "DEPOSIT";
            break;
        case TransactionType::WITHDRAWAL:
            typeStr = "WITHDRAWAL";
            break;
        case TransactionType::TRANSFER:
            typeStr = "TRANSFER";
            break;
        case TransactionType::PAYMENT:
            typeStr = "PAYMENT";
            break;
    }
    
    // Format: (accountNumber, transactionType, amount, description, balanceAfter)
    // transactionDate will be auto-populated by DEFAULT CURRENT_TIMESTAMP
    string columns = "(accountNumber, transactionType, amount, description, balanceAfter)";
    string values = "('" + account.accountNumber() + "', '" +
                   typeStr + "', " +
                   to_string(amount) + ", '" +
                   description + "', " +
                   to_string(account.getBalance()) + ")";
    
    db.addtoTable(transactionsTableName_ + " " + columns, values);
    
    cout << "[INFO] Transaction recorded in database" << endl;
}

string AccountManager::getAccountsTableName() const {
    return accountsTableName_;
}

string AccountManager::getTransactionsTableName() const {
    return transactionsTableName_;
}