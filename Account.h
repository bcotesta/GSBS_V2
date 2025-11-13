#pragma once
#include "Types.h"
#include "Transaction.h"

#include <string>
#include <list>

class Account
{
public:
	Account(std::string accountNumber, AccountType accType);
	
	// Getters:
	
	// Get the unique account number
	std::string accountNumber() const;
	// Get the type of account (chequing, savings, credit, or loan.)
	AccountType accountType() const;

	// Balance operations:

	// Get the current balance stored in memory
	double getBalance() const;
	// Set the balance manually (mainly used when loading data from the database)
	void setBalance(double balance);

	// Expose references if external code must inspect/append history
	std::list<Transaction>& transactionHistory();
	const std::list<Transaction>& transactionHistory() const;
	
	// Core banking operations:
	
	// Deposit money into the account and record the transaction
	void deposit(double amount);
	// Withdraw money from the account and record the transaction
	void withdraw(double amount);
	
	// Database operations:
	 // Save a new account to the database when it is first created
	void saveToDatabase(std::string uname);
	// Update the account balance in the database after transactions
	void updateBalanceInDatabase(const std::string& tableName); 

private:
	std::string accountNumber_;   // Unique ID for the account
	AccountType accountType_;     // Type of account (chequing, savings, credit, or loan)
	std::list<Transaction> transactionHistory_;  // List of transactions associated with the account
	double balance_{ 0.0 }; 		// Current balance of the account
};

