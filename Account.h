#pragma once
#include "Types.h"
#include "Transaction.h"

#include <string>
#include <list>

class Account
{
public:
	Account(std::string accountNumber, AccountType accType);
	
	// Getters
	std::string accountNumber() const;
	AccountType accountType() const;

	// Balance operations
	double getBalance() const;
	void setBalance(double balance);

	// Expose references if external code must inspect/append history
	std::list<Transaction>& transactionHistory();
	const std::list<Transaction>& transactionHistory() const;
	
	// Core banking operations
	void deposit(double amount);
	void withdraw(double amount);
	
	// Database operations
	void saveToDatabase(std::string uname);
	void updateBalanceInDatabase(const std::string& tableName); 

private:
	std::string accountNumber_;
	AccountType accountType_;
	std::list<Transaction> transactionHistory_;
	double balance_{ 0.0 };
};

