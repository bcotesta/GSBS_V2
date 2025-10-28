// Brandon Cotesta | 10/27/2025 | 10:20 PM
// MOOD: Exhausted - legit feels like 4am.

#include "Authenticator.h"
#include "DatabaseManager.h"

// Constructor implementation
Authenticator::Authenticator() {
	// not needed yet
}

// Singleton instance getter
Authenticator& Authenticator::getInstance() {
	static Authenticator instance;
	return instance;
}

void Authenticator::setValidInfo(const std::string& username, const std::string& password) {
	// Saftey check
	if (username.empty() || password.empty())
		return;

	// Access DatabaseManager singleton
	DatabaseManager& dbManager = DatabaseManager::getInstance();

	// Retrieve and set valid credentials from the database
	// has to be email since it is a unique identifier
	std::string whereClause = "email ='" + username + "'"; 
	// Get userID
	std::string userIDResult = static_cast<std::string>(dbManager.retStringW("userID", "userinfo", whereClause, "userID"));

	if (userIDResult.empty()) {
		std::cerr << "No user found with username: " << username << std::endl;
		// clear data just in case
		validUserID_ = "";
		validName_ = "";
		validUsername_ = "";
		validPhone_ = "";
		validPassword_ = "";
		return;
	}

	// use userid to get other info
	std::string selectCols = "userID, name, email, phone, password";

	// fetches all columns using the primary key userID
	auto results = dbManager.retMultipleColumns(selectCols, "userinfo", whereClause);

	// grab each field
	validUserID_ = static_cast<std::string>(results["userID"]);
	validName_ = static_cast<std::string>(results["name"]);
	validUsername_ = static_cast<std::string>(results["email"]);
	validPhone_ = static_cast<std::string>(results["phone"]);
	validPassword_ = static_cast<std::string>(results["password"]);
	std::cout << "[DEBUG] setValidInfo() - Successfully loaded user info:" << std::endl;
	std::cout << "[DEBUG]   UserID: " << validUserID_ << std::endl;
	std::cout << "[DEBUG]   Name: " << validName_ << std::endl;
	std::cout << "[DEBUG]   Email: " << validUsername_ << std::endl;
	std::cout << "[DEBUG]   Phone: " << validPhone_ << std::endl;
	std::cout << "[DEBUG]   Password loaded: " << (!validPassword_.empty() ? "Yes" : "No") << std::endl;
}

// Destructor implementation
Authenticator::~Authenticator() {
	// Cleanup if necessary
}