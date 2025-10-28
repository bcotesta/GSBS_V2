// Brandon Cotesta | 10/27/2025 | 10:00 PM
// MOOD: Tired - long day, but i have much more night ahead of me.
// Dodgers just scored to make it 4-3 jays.
#pragma once
#include <string>
#include <iostream>

using namespace std;
class Authenticator
{
public:
	// Singleton instance accessor
	static Authenticator& getInstance();
	// Delete copy constructor and assignment operator
	Authenticator(const Authenticator&) = delete;
	Authenticator& operator=(const Authenticator&) = delete;

	// Member functions
	bool verifyCredentials(const std::string& username, const std::string& password);
	void setValidInfo(const std::string& username, const std::string& password);
	std::string getUserID() const { return validUserID_; }
	std::string getUsername() const { return validUsername_; }
private:
	Authenticator();
	~Authenticator();

	string validUserID_;
	string validUsername_;
	string validPassword_;
	string validName_;
	string validPhone_;
	string validAddress_;
	string validAccountNum_;
};