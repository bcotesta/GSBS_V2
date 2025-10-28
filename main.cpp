// Brandon Cotesta | 10/27/2025 | 8:43 PM
// MOOD: Neutral. not excited to spend the night doing this
// but ready to get it done. Blue Jays are down 1-0 >:(.

#include "DatabaseManager.h"
#include "Authenticator.h"
#include "User.h"
#include "Account.h"

#include <QtCore/QCoreApplication>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

// ==================== HELPER FUNCTIONS ====================

// Helper function to generate a random account number
string generateAccountNumber() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(100000000, 999999999);
    return to_string(dis(gen));
}

// Helper function to convert string to AccountType enum
AccountType stringToAccountType(const string& typeStr) {
    if (typeStr == "CHEQUING") return AccountType::CHEQUING;
    if (typeStr == "SAVINGS") return AccountType::SAVINGS;
    if (typeStr == "CREDIT") return AccountType::CREDIT;
    if (typeStr == "LOAN") return AccountType::LOAN;
    return AccountType::CHEQUING; // Default
}

// Helper function to sanitize name for table construction
string sanitizeName(const string& name) {
    string cleanName = name;
    cleanName.erase(std::remove(cleanName.begin(), cleanName.end(), ' '), cleanName.end());
    cleanName.erase(std::remove(cleanName.begin(), cleanName.end(), '-'), cleanName.end());
    cleanName.erase(std::remove(cleanName.begin(), cleanName.end(), '.'), cleanName.end());
    return cleanName;
}

// ==================== AUTHENTICATION FUNCTIONS ====================

// Prompt user for credentials and authenticate
bool authenticateUser(Authenticator& auth, string& username, string& password) {
    cout << "Enter username (email): ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);
    
    return auth.verifyCredentials(username, password);
}

// Display user information
void displayUserInfo(const User& user) {
    cout << "User " << user.email() << " logged in successfully." << endl;
    cout << "User ID: " << user.userId() << endl;
    cout << "Name: " << user.name() << endl;
    cout << "Email: " << user.email() << endl;
}

// ==================== TABLE MANAGEMENT FUNCTIONS ====================

// Construct user-specific table names
pair<string, string> getUserTableNames(const User& user) {
    string cleanName = sanitizeName(user.name());
    string userIdStr = to_string(user.userId());
    
    string accountsTable = userIdStr + "_" + cleanName + "_acc";
    string transactionsTable = userIdStr + "_" + cleanName + "_transactions";
    
    return make_pair(accountsTable, transactionsTable);
}

// Check if user tables exist, create if they don't
void ensureUserTablesExist(DatabaseManager& db, Authenticator& auth, 
                           const User& user, const string& accountsTable) {
    cout << "Checking for user tables..." << endl;
    cout << "Looking for table: " << accountsTable << endl;
    
    // Check if table exists by querying information_schema
    string whereClause = "TABLE_SCHEMA = 'bankdatabase' AND TABLE_NAME = '" + accountsTable + "'";
    auto tableCheck = db.retrieveTable("information_schema.TABLES", whereClause);
    
    if (tableCheck.empty()) {
        cout << "Accounts table not found. Creating..." << endl;
        db.createUserAccountsTable(auth.getUserID(), user.name());
        db.createUserTransactionsTable(auth.getUserID(), user.name());
        cout << "Tables created successfully." << endl;
    } else {
        cout << "Accounts table exists." << endl;
    }
}

// ==================== ACCOUNT MANAGEMENT FUNCTIONS ====================

// Load existing accounts from database
vector<Account> loadExistingAccounts(DatabaseManager& db, const string& accountsTable) {
    vector<Account> existingAccounts;
    
    auto accResults = db.retrieveTable(accountsTable, "");
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
            existingAccounts.push_back(acc);
            
            cout << "Loaded account: " << accNumber 
                 << " (" << accTypeStr << ") - Balance: $" << balanceStr << endl;
        }
        cout << "Total accounts loaded: " << existingAccounts.size() << endl;
    }
    
    return existingAccounts;
}

// Prompt user to select account type
AccountType selectAccountType() {
    cout << "\n=== Creating New Account ===" << endl;
    cout << "Select account type:" << endl;
    cout << "1. CHEQUING" << endl;
    cout << "2. SAVINGS" << endl;
    cout << "3. CREDIT" << endl;
    cout << "4. LOAN" << endl;
    cout << "Enter choice (1-4): ";
    
    int choice;
    cin >> choice;
    cin.ignore(); // Clear newline from buffer
    
    AccountType accType;
    switch (choice) {
        case 1:
            accType = AccountType::CHEQUING;
            break;
        case 2:
            accType = AccountType::SAVINGS;
            break;
        case 3:
            accType = AccountType::CREDIT;
            break;
        case 4:
            accType = AccountType::LOAN;
            break;
        default:
            cout << "Invalid choice. Defaulting to CHEQUING." << endl;
            accType = AccountType::CHEQUING;
    }
    
    return accType;
}

// Create a new account and save to database
Account createAndSaveAccount(const User& user, AccountType accType) {
    string accountNumber = generateAccountNumber();
    
    Account newAccount(accountNumber, accType);
    cout << "Created account: " << accountNumber << endl;
    
    newAccount.saveToDatabase(user.name());
    
    return newAccount;
}

// Display account details from database
void displayAccountDetails(DatabaseManager& db, const string& accountsTable) {
    cout << "\n=== Verifying Account Creation ===" << endl;
    auto accResults = db.retrieveTable(accountsTable, "");
    cout << "Total accounts now: " << accResults.size() << endl;
    
    // Display all accounts
    for (const auto& account : accResults) {
        cout << "\nAccount Details:" << endl;
        cout << "  Account Number: " << static_cast<string>(account.at("accountNumber")) << endl;
        cout << "  Account Type: " << static_cast<string>(account.at("accountType")) << endl;
        cout << "  Balance: $" << static_cast<string>(account.at("balance")) << endl;
        cout << "  Created Date: " << static_cast<string>(account.at("createdDate")) << endl;
    }
}

// Display summary of account objects in memory
void displayAccountObjectsSummary(const vector<Account>& accounts) {
    cout << "\n=== Account Objects Summary ===" << endl;
    cout << "You now have " << accounts.size() << " Account objects in memory." << endl;
    for (size_t i = 0; i < accounts.size(); ++i) {
        cout << i+1 << ". Account #" << accounts[i].accountNumber() 
             << " - Balance: $" << accounts[i].getBalance() << endl;
    }
}

// ==================== MAIN DEBUG FUNCTION ====================

static void DEBUG() {
    // Step 1: Authenticate user
    Authenticator& auth = Authenticator::getInstance();
    string username, password;
    
    if (!authenticateUser(auth, username, password)) {
        cout << "Login failed for user: " << username << endl;
        return;
    }
    
    // Step 2: Create user object and display info
    User user(auth.getUserID(), username, password);
    displayUserInfo(user);
    
    // Step 3: Get database manager and table names
    DatabaseManager& db = DatabaseManager::getInstance();
    auto tableNames = getUserTableNames(user);
    string accountsTable = tableNames.first;
    string transactionsTable = tableNames.second;
    
    // Step 4: Ensure user tables exist
    ensureUserTablesExist(db, auth, user, accountsTable);
    
    // Step 5: Load existing accounts
    vector<Account> existingAccounts = loadExistingAccounts(db, accountsTable);
    
    // Step 6: Create new account
    AccountType accType = selectAccountType();
    Account newAccount = createAndSaveAccount(user, accType);
    existingAccounts.push_back(newAccount);
    
    // Step 7: Display results
    displayAccountDetails(db, accountsTable);
    displayAccountObjectsSummary(existingAccounts);
}

// ==================== MAIN ENTRY POINT ====================

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    DEBUG();
    return app.exec();
}

