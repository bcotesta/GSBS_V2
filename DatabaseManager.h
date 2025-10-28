// Brandon Cotesta | 10/27/2025 | 8:51 PM
#pragma once
#include <iostream>
#include <mysql/jdbc.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <string>

using std::string;
using namespace std;
class DatabaseManager
{
public:
	// Singleton instance accessor
	static DatabaseManager& getInstance();

	// Delete copy constructor and assignment operator
	DatabaseManager(const DatabaseManager&) = delete;
	DatabaseManager& operator=(const DatabaseManager&) = delete;
	// public member functions
	
	// These are used to create user-specific tables upon account creation
	void createUserAccountsTable(std::string userID, std::string username);
	void createUserTransactionsTable(std::string userID, std::string username);
	void addtoTable(std::string tab, std::string val);
	void updateTable(std::string tab, std::string setv, std::string cond);

	// SQL query functions
	sql::SQLString retString(std::string col, std::string tab, std::string specval);
	sql::SQLString retStringW(std::string col, std::string tab, std::string val, std::string specval);
	std::map<std::string, sql::SQLString> retMultipleColumns(
		std::string cols,
		std::string tab,
		std::string whereClause);
private:
	// Private constructor for Singleton
	DatabaseManager();
	~DatabaseManager();

	void ensureConnection();

	std::string statement;
	sql::Statement* stmt;
	sql::Connection* connection;
	sql::mysql::MySQL_Driver* driver;

	// Connection details
	const std::string host = "tcp://136.114.146.175:3306";
	const std::string user = "client";
	const std::string password = "gsbsTeam20$";
	const std::string schema = "bankdatabase";

	// SQL keywords for easier queries
	const std::string insert = "INSERT INTO ";
	const std::string ctable = "CREATE TABLE ";
	const std::string values = "VALUES ";
	const std::string update = "UPDATE ";
	const std::string set = "SET ";
	const std::string select = "SELECT ";
	const std::string from = "FROM ";
	const std::string where = "WHERE ";
};

