// Brandon Cotesta | 10/27/2025 | 9:26 PM
// Original Script - Everest Ashley
// MOOD: Happy - Blue Jays just went up 4-2 in the 4th.

#include "DatabaseManager.h"
#include <string>
#include <iostream>
#include <sstream>

// Singleton instance getter
DatabaseManager& DatabaseManager::getInstance() {
	static DatabaseManager instance;
	return instance;
}

// DatabaseManager constructor
// Initializes the database connection and statement
// Handles sql errors during connection setup

DatabaseManager::DatabaseManager()
    : stmt(nullptr), connection(nullptr), driver(nullptr)
{
    try {
        std::cout << "Initializing DatabaseManager..." << std::endl;
        std::cout << "Connecting to host: " << host << std::endl;

        driver = sql::mysql::get_driver_instance();
        connection = driver->connect(host, user, password);
        connection->setSchema(schema);
        stmt = connection->createStatement();

        // Ensure UTF-8 encoding for all database operations
        stmt->execute("SET NAMES 'utf8mb4'");
        stmt->execute("SET CHARACTER SET utf8mb4");

        std::cout << "Database connected successfully" << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error in constructor: " << e.what() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        // Clean up on error
        if (stmt) {
            delete stmt;
            stmt = nullptr;
        }
        if (connection) {
            delete connection;
            connection = nullptr;
        }
    }
}

// Ensure the database connection is active
void DatabaseManager::ensureConnection() {
    try {
		// Reconnect if the connection is closed or null
        if (connection == nullptr || connection->isClosed()) {
            if (connection) {
                delete connection;
                connection = nullptr;
            }
			// Re-establish the connection
            connection = driver->connect(host, user, password);
            connection->setSchema(schema);
			// Recreate the statement object
            if (stmt) {
                delete stmt;
            }
            stmt = connection->createStatement();
            // Ensure UTF-8 encoding for all database operations
            stmt->execute("SET NAMES 'utf8mb4'");
            stmt->execute("SET CHARACTER SET utf8mb4");
            std::cout << "Database reconnected successfully" << std::endl;
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error in ensureConnection: " << e.what() << std::endl;
		throw; // Rethrow exception for caller to handle
    }
}

// Create a user-specific accounts table
// Table name format: userID_username_acc
void DatabaseManager::createUserAccountsTable(std::string userID, std::string username)
{
    try {
        ensureConnection();

        // Remove spaces and special characters from username for table name
        std::string cleanUsername = username;
        cleanUsername.erase(std::remove(cleanUsername.begin(), cleanUsername.end(), ' '), cleanUsername.end());
        cleanUsername.erase(std::remove(cleanUsername.begin(), cleanUsername.end(), '-'), cleanUsername.end());
        cleanUsername.erase(std::remove(cleanUsername.begin(), cleanUsername.end(), '.'), cleanUsername.end());

        // Create table name: userID_username_acc
        std::string tableName = userID + "_" + cleanUsername + "_acc";

        // Create table with account details
        std::string statement = ctable + tableName +
            " (accountNumber VARCHAR(50) PRIMARY KEY, " +
            "accountType VARCHAR(20) NOT NULL, " +
            "balance DECIMAL(15,2) NOT NULL DEFAULT 0.00, " +
            "createdDate DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP)";

        stmt->execute(statement);
        std::cout << "User accounts table created successfully: " << tableName << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error in createUserAccountsTable: " << e.what() << std::endl;
    }
}

// Create a user-specific transactions table
// Table name format: userID_username_transactions
void DatabaseManager::createUserTransactionsTable(std::string userID, std::string username)
{
    try {
        ensureConnection();

        // Remove spaces and special characters from username for table name
        std::string cleanUsername = username;
        cleanUsername.erase(std::remove(cleanUsername.begin(), cleanUsername.end(), ' '), cleanUsername.end());
        cleanUsername.erase(std::remove(cleanUsername.begin(), cleanUsername.end(), '-'), cleanUsername.end());
        cleanUsername.erase(std::remove(cleanUsername.begin(), cleanUsername.end(), '.'), cleanUsername.end());

        // Create table name: userID_username_transactions
        std::string tableName = userID + "_" + cleanUsername + "_transactions";

        // Create table with transaction details
        std::string statement = ctable + tableName +
            " (transactionID INT AUTO_INCREMENT PRIMARY KEY, " +
            "accountNumber VARCHAR(50) NOT NULL, " +
            "transactionType VARCHAR(20) NOT NULL, " +
            "amount DECIMAL(15,2) NOT NULL, " +
            "transactionDate DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP, " +
            "description VARCHAR(255), " +
            "balanceAfter DECIMAL(15,2) NOT NULL)";

        stmt->execute(statement);
        std::cout << "User transactions table created successfully: " << tableName << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error in createUserTransactionsTable: " << e.what() << std::endl;
    }
}

// Add data to a specified table
/// <param name="tab">Table name</param>
/// <param name="val">Values to insert (formatted as SQL VALUES clause)</param>
void DatabaseManager::addtoTable(std::string tab, std::string val)
{
    try {
        ensureConnection();

        std::string statement = insert + tab + " " + values + val;
        stmt->execute(statement);
        std::cout << "Data added to table successfully" << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error in addtoTable: " << e.what() << std::endl;
    }
}

sql::SQLString DatabaseManager::retString(std::string col, std::string tab, std::string specval)
{
    sql::SQLString tempstring;
    sql::ResultSet* res = nullptr;

    try {
        ensureConnection();

        statement = select + col + " " + from + tab;
        res = stmt->executeQuery(statement);

        if (res && res->next()) {
            tempstring = res->getString(specval);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error in retString: " << e.what() << std::endl;
    }

    if (res) {
        delete res;
    }

    return tempstring;
}

sql::SQLString DatabaseManager::retStringW(std::string col, std::string tab, std::string val, std::string specval)
{
    sql::SQLString tempstring;
    sql::ResultSet* res = nullptr;

    try {
        ensureConnection();

        statement = select + col + " " + from + tab + " " + where + val;
        res = stmt->executeQuery(statement);

        if (res && res->next()) {
            tempstring = res->getString(specval);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error in retStringW: " << e.what() << std::endl;
    }

    if (res) {
        delete res;
    }

    return tempstring;
}

void DatabaseManager::updateTable(std::string tab, std::string setv, std::string cond)
{
    try {
        ensureConnection();

        statement = update + tab + " " + set + setv + " " + where + cond;
        stmt->execute(statement);
        std::cout << "Table updated successfully" << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error in updateTable: " << e.what() << std::endl;
    }
}

std::map<std::string, sql::SQLString> DatabaseManager::retMultipleColumns(
    std::string cols,
    std::string tab,
    std::string whereClause)
{
    std::map<std::string, sql::SQLString> results;
    sql::ResultSet* res = nullptr;

    try {
        ensureConnection();

        statement = select + cols + " " + from + tab + " " + where + whereClause;
        res = stmt->executeQuery(statement);

        if (res && res->next()) {
            // Parse the column names from the cols string
            std::istringstream ss(cols);
            std::string colName;

            while (std::getline(ss, colName, ',')) {
                // Trim whitespace
                colName.erase(0, colName.find_first_not_of(" \t"));
                colName.erase(colName.find_last_not_of(" \t") + 1);

                try {
                    results[colName] = res->getString(colName);
                }
                catch (sql::SQLException& e) {
                    std::cerr << "Error getting column '" << colName << "': " << e.what() << std::endl;
                }
            }
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error in retMultipleColumns: " << e.what() << std::endl;
    }

    if (res) {
        delete res;
    }

    return results;
}

// Destructor to clean up resources
DatabaseManager::~DatabaseManager() {
    if (stmt) {
        delete stmt;
        stmt = nullptr;
    }
    if (connection) {
        delete connection;
        connection = nullptr;
    }
}