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

bool Authenticator::verifyCredentials(const std::string& username, const std::string& password) {
	// Safety check
	if (username.empty() || password.empty()) {
		std::cerr << "Username or password cannot be empty" << std::endl;
		return false;
	}

	// First, load the user info from database
	setValidInfo(username, password);

	// Check if user was found and password matches
	if (validUserID_.empty()) {
		std::cerr << "User not found" << std::endl;
		return false;
	}

	// Verify the password matches
	if (validPassword_ == password) {
		std::cout << "Credentials verified successfully for user: " << validUsername_ << std::endl;
		return true;
	}

	std::cerr << "Invalid password for user: " << username << std::endl;
	return false;
}

bool Authenticator::registerNewUser(const std::string& name, const std::string& email, 
                                    const std::string& phone, const std::string& password) {
    // Safety checks
    if (name.empty() || email.empty() || password.empty()) {
        std::cerr << "Name, email, and password are required fields" << std::endl;
        return false;
    }
    
    // Check if user already exists
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    std::string whereClause = "email = '" + email + "'";
    std::string existingUserID = static_cast<std::string>(
        dbManager.retStringW("userID", "userinfo", whereClause, "userID")
    );
    
    if (!existingUserID.empty()) {
        std::cerr << "User with email " << email << " already exists" << std::endl;
        return false;
    }
    
    // Get the next available userID
    auto allUsers = dbManager.retrieveTable("userinfo", "");
    int nextUserID = allUsers.size() + 1;  // Simple increment (you may want a better approach)
    
    // Insert new user into database
    std::string columns = "(userID, name, email, phone, password)";
    std::string values = "('" + std::to_string(nextUserID) + "', '" +
                        name + "', '" +
                        email + "', '" +
                        phone + "', '" +
                        password + "')";
    
    try {
        dbManager.addtoTable("userinfo " + columns, values);
        std::cout << "User registered successfully! UserID: " << nextUserID << std::endl;
        
        // Set valid info for immediate login
        validUserID_ = std::to_string(nextUserID);
        validName_ = name;
        validUsername_ = email;
        validPhone_ = phone;
        validPassword_ = password;
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error registering user: " << e.what() << std::endl;
        return false;
    }
}

// Destructor implementation
Authenticator::~Authenticator() {
	// Cleanup if necessary
}