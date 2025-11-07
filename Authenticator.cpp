// Brandon Cotesta | 10/27/2025 | 10:20 PM
// MOOD: Exhausted - legit feels like 4am.

#include "Authenticator.h"
#include "DatabaseManager.h"
#include "OTPManager.h"
#include "TwilioService.h"

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

// Check if two-factor authentication is enabled for user
bool Authenticator::isTwoFactorEnabled(const std::string& userID) const {
    if (userID.empty()) {
        return false;
    }
    
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    std::string whereClause = "userID = '" + userID + "'";
    
    try {
        std::string twoFactorStatus = static_cast<std::string>(
            dbManager.retStringW("twoFactorEnabled", "userinfo", whereClause, "twoFactorEnabled")
        );
        
        // Convert to bool (1 or "1" means enabled)
        return (!twoFactorStatus.empty() && (twoFactorStatus == "1" || twoFactorStatus == "true"));
    }
    catch (const std::exception& e) {
        std::cerr << "Error checking 2FA status: " << e.what() << std::endl;
        return false;
    }
}

// Get the two-factor authentication method (email or phone)
std::string Authenticator::getTwoFactorMethod(const std::string& userID) const {
    if (userID.empty()) {
        return "email"; // Default to email
    }
    
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    std::string whereClause = "userID = '" + userID + "'";
    
    try {
        std::string method = static_cast<std::string>(
            dbManager.retStringW("twoFactorMethod", "userinfo", whereClause, "twoFactorMethod")
        );
        
        // Return email or phone, default to email if empty
        if (method.empty() || (method != "email" && method != "phone")) {
            return "email";
        }
        
        return method;
    }
    catch (const std::exception& e) {
        std::cerr << "Error getting 2FA method: " << e.what() << std::endl;
        return "email"; // Default fallback
    }
}

// Send OTP to user via specified method (auto-detects if empty)
bool Authenticator::sendOTP(const std::string& userID, const std::string& method) {
    if (userID.empty()) {
        std::cerr << "[AUTH] Cannot send OTP: Invalid user ID" << std::endl;
        return false;
    }
    
    // Auto-detect method if not specified
    std::string deliveryMethod = method;
    if (deliveryMethod.empty()) {
        deliveryMethod = getTwoFactorMethod(userID);
        std::cout << "[AUTH] Auto-detected 2FA method: " << deliveryMethod << std::endl;
    }
    
    // Generate OTP using OTPManager
    OTPManager& otpMgr = OTPManager::getInstance();
    std::string code = otpMgr.generateOTP(userID);
    
    // Get user's contact info from database
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    std::string whereClause = "userID = '" + userID + "'";
    
    try {
        std::string destination;
        
        if (deliveryMethod == "phone") {
            destination = static_cast<std::string>(
                dbManager.retStringW("phone", "userinfo", whereClause, "phone")
            );
            
            if (destination.empty()) {
                std::cerr << "[AUTH] No phone number found for user" << std::endl;
                return false;
            }
            
            // Send via Twilio SMS
            TwilioService& twilioSvc = TwilioService::getInstance();
            
            if (!twilioSvc.isConfigured()) {
                std::cerr << "[AUTH] Twilio not configured. Cannot send SMS." << std::endl;
                std::cout << "[FALLBACK] Your verification code is: " << code << std::endl;
                return false;
            }
            
            std::string message = "Your GSBS verification code is: " + code + 
                                ". This code expires in 5 minutes. Do not share this code.";
            
            bool sent = twilioSvc.sendSMS(destination, message);
            
            if (!sent) {
                std::cerr << "[AUTH] Failed to send SMS via Twilio" << std::endl;
                std::cout << "[FALLBACK] Your verification code is: " << code << std::endl;
                return false;
            }
            
            std::cout << "[AUTH] OTP sent via SMS to " << destination << std::endl;
            return true;
            
        } else { // email
            destination = static_cast<std::string>(
                dbManager.retStringW("email", "userinfo", whereClause, "email")
            );
            
            if (destination.empty()) {
                std::cerr << "[AUTH] No email found for user" << std::endl;
                return false;
            }
            
            // TODO: Integrate real email service (SendGrid, AWS SES, etc.)
            std::cout << "\n[SIMULATED EMAIL] Sending to: " << destination << std::endl;
            std::cout << "[SIMULATED EMAIL] Your verification code is: " << code << std::endl;
            std::cout << "[SIMULATED EMAIL] Code expires in 5 minutes\n" << std::endl;
            
            return true;
        }
        
    }
    catch (const std::exception& e) {
        std::cerr << "[AUTH] Error sending OTP: " << e.what() << std::endl;
        return false;
    }
}

// Verify OTP code provided by user
bool Authenticator::verifyOTP(const std::string& userID, const std::string& code) {
    if (userID.empty() || code.empty()) {
        std::cerr << "[AUTH] Invalid user ID or verification code" << std::endl;
        return false;
    }
    
    // Verify using OTPManager
    OTPManager& otpMgr = OTPManager::getInstance();
    bool verified = otpMgr.verifyOTP(userID, code);
    
    if (verified) {
        std::cout << "[AUTH] 2FA verification successful for user: " << userID << std::endl;
    } else {
        std::cerr << "[AUTH] 2FA verification failed for user: " << userID << std::endl;
    }
    
    return verified;
}

// Destructor implementation
Authenticator::~Authenticator() {
	// Cleanup if necessary
}