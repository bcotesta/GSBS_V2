// Brandon Cotesta 10/08/2025

#include "Account.h"
#include "Transaction.h" 
#include "Types.h"
#include "DatabaseManager.h"
#include "Authenticator.h"
#include <iostream>
#include <string>
#include <chrono>
using namespace std;

Account::Account(string accountNumber, AccountType accType)
{
    accountNumber_ = std::move(accountNumber);
    accountType_ = accType;
    balance_ = 0.0;
}

// Getters
string Account::accountNumber() const { return accountNumber_; }
AccountType Account::accountType() const { return accountType_; }
list<Transaction>& Account::transactionHistory() { return transactionHistory_; }
const list<Transaction>& Account::transactionHistory() const { return transactionHistory_; }

// Deposit into the account
void Account::deposit(double amount)
{
    if (amount <= 0) {
        cerr << "Deposit amount must be positive." << endl;
        return;
    }
    
    // Simply update the balance
    // Transaction recording will be handled by AccountManager
    balance_ += amount;
}

// Withdraw from the account
void Account::withdraw(double amount)
{
    if (amount <= 0) {
        cerr << "Withdrawal amount must be positive." << endl;
        return;
    }
    if (amount > balance_) {
        cerr << "Insufficient funds for withdrawal." << endl;
        return;
    }
    
    // Simply update the balance
    // Transaction recording will be handled by AccountManager
    balance_ -= amount;
}

double Account::getBalance() const { return balance_; }

void Account::saveToDatabase(string uname) {
    DatabaseManager& db = DatabaseManager::getInstance();
    Authenticator& auth = Authenticator::getInstance();
    
    std::string accT = "";

    switch (accountType_)
    {
    case AccountType::CHEQUING:
        accT = "CHEQUING";
        break;
    case AccountType::SAVINGS:
        accT = "SAVINGS";
        break;
    case AccountType::CREDIT:
        accT = "CREDIT";
        break;
    case AccountType::LOAN:
        accT = "LOAN";
        break;
    }
    
    // Get user information from Authenticator
    std::string userID = auth.getUserID();
    std::string username = uname;
    
    // Sanitize username for table name
    std::string cleanUsername = username;
    cleanUsername.erase(std::remove(cleanUsername.begin(), cleanUsername.end(), ' '), cleanUsername.end());
    cleanUsername.erase(std::remove(cleanUsername.begin(), cleanUsername.end(), '@'), cleanUsername.end());
    cleanUsername.erase(std::remove(cleanUsername.begin(), cleanUsername.end(), '.'), cleanUsername.end());
    cleanUsername.erase(std::remove(cleanUsername.begin(), cleanUsername.end(), '-'), cleanUsername.end());
    
    // Construct table name: userID_username_acc
    std::string tableName = userID + "_" + cleanUsername + "_acc";
    
    // Specify column names explicitly to avoid column count mismatch
    // This allows createdDate to use its DEFAULT CURRENT_TIMESTAMP
    std::string columns = "(accountNumber, accountType, balance)";
    std::string values = "('" + accountNumber_ + "', '" + accT + "', " + std::to_string(balance_) + ")";
    
    try {
        // Construct full INSERT statement with column names
        db.addtoTable(tableName + " " + columns, values);
        std::cout << "Account " << accountNumber_ << " saved to database table: " << tableName << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving account to database: " << e.what() << std::endl;
    }
}

void Account::setBalance(double balance) {
    balance_ = balance;
}

void Account::updateBalanceInDatabase(const std::string& tableName) {
    DatabaseManager& db = DatabaseManager::getInstance();
    
    // Update balance in database
    std::string setClause = "balance = " + std::to_string(balance_);
    std::string whereClause = "accountNumber = '" + accountNumber_ + "'";
    
    db.updateTable(tableName, setClause, whereClause);
}
