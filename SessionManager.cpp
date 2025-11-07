// Brandon Cotesta || 10/28/2025 | 1:00 PM

#include "SessionManager.h"
#include "OTPManager.h"
#include "DatabaseManager.h"
#include <iostream>

using namespace std;

// Constructor that grabs the singleton Authenticator instance
SessionManager::SessionManager()
    : auth_(Authenticator::getInstance()), currentUser_(nullptr), loggedIn_(false)
{
}

// Login method with 2FA support
bool SessionManager::login() {
    string username, password;
    
    if (!promptCredentials(username, password)) {
        return false;
    }
    
    // Verify credentials
    if (!auth_.verifyCredentials(username, password)) {
        cout << "Login failed: Invalid credentials" << endl;
        loggedIn_ = false;
        return false;
    }
    
    string userID = auth_.getUserID();
    
    // Check if 2FA is enabled
    if (auth_.isTwoFactorEnabled(userID)) {
        cout << "\n=== Two-Factor Authentication Required ===" << endl;
        
        // Get 2FA method (email or phone)
        string method = auth_.getTwoFactorMethod(userID);
        cout << "Sending verification code via " << method << "..." << endl;
        
        // Send OTP
        if (!auth_.sendOTP(userID, method)) {
            cout << "Failed to send verification code" << endl;
            return false;
        }
        
        cout << "A verification code has been sent to your " << method << endl;
        
        // Prompt for OTP
        string otpCode;
        cout << "Enter verification code: ";
        getline(cin, otpCode);
        
        // Verify OTP
        if (!auth_.verifyOTP(userID, otpCode)) {
            cout << "Login failed: Invalid or expired verification code" << endl;
            loggedIn_ = false;
            return false;
        }
        
        cout << "Two-factor authentication successful!" << endl;
    } else {
        // 2FA not enabled - prompt user to enable it
        cout << "\n=== Enhance Your Account Security ===" << endl;
        cout << "Two-factor authentication is not enabled on your account." << endl;
        cout << "Would you like to enable it now for better security? (y/n): ";
        
        string response;
        getline(cin, response);
        
        if (response == "y" || response == "Y" || response == "yes" || response == "Yes") {
            if (enable2FAForUser(userID)) {
                cout << "Two-factor authentication has been enabled successfully!" << endl;
            } else {
                cout << "Failed to enable 2FA. You can enable it later in settings." << endl;
            }
        } else {
            cout << "You can enable 2FA anytime in your account settings." << endl;
        }
    }
    
    // Create user object
    currentUser_ = make_unique<User>(auth_.getUserID(), username, password);
    loggedIn_ = true;
    
    cout << "Login successful!" << endl;
    return true;
}

// Helper method to enable 2FA for a user
bool SessionManager::enable2FAForUser(const std::string& userID) {
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    
    try {
        // Prompt for preferred method
        cout << "\nSelect 2FA method:" << endl;
        cout << "1. SMS (Phone)" << endl;
        cout << "2. Email" << endl;
        cout << "Enter choice (1-2): ";
        
        string methodChoice;
        getline(cin, methodChoice);
        
        string method = (methodChoice == "1") ? "phone" : "email";
        
        // Update database to enable 2FA
        std::string setClause = "twoFactorEnabled = '1', twoFactorMethod = '" + method + "'";
        std::string whereClause = "userID = '" + userID + "'";
        
        dbManager.updateTable("userinfo", setClause, whereClause);
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error enabling 2FA: " << e.what() << std::endl;
        return false;
    }
}

// logout method
void SessionManager::logout() {
    if (loggedIn_) {
        cout << "Logging out user: " << currentUser_->name() << endl;
        currentUser_.reset();
        loggedIn_ = false;
        cout << "Logout successful" << endl;
    }
}

bool SessionManager::isLoggedIn() const {
    return loggedIn_;
}

User* SessionManager::getCurrentUser() {
    return currentUser_.get();
}

const User* SessionManager::getCurrentUser() const {
    return currentUser_.get();
}

// Helper to prompt for username and password in console
bool SessionManager::promptCredentials(string& username, string& password) {
    cout << "Enter username (email): ";
    getline(cin, username);
    
    cout << "Enter password: ";
    getline(cin, password);
    
    if (username.empty() || password.empty()) {
        cout << "Username and password cannot be empty" << endl;
        return false;
    }
    
    return true;
}

// Registration method - phone now MANDATORY
bool SessionManager::registerUser() {
    string name, email, phone, password;
    
    if (!promptRegistrationInfo(name, email, phone, password)) {
        return false;
    }
    
    // Register the user
    if (!auth_.registerNewUser(name, email, phone, password)) {
        cout << "Registration failed" << endl;
        return false;
    }
    
    // Create user object (auth has already set valid info)
    currentUser_ = make_unique<User>(auth_.getUserID(), email, password);
    loggedIn_ = true;
    
    cout << "\nRegistration successful! You are now logged in." << endl;
    
    // Prompt to enable 2FA immediately after registration
    cout << "\n=== Secure Your Account ===" << endl;
    cout << "Would you like to enable two-factor authentication? (Recommended) (y/n): ";
    
    string response;
    getline(cin, response);
    
    if (response == "y" || response == "Y" || response == "yes" || response == "Yes") {
        if (enable2FAForUser(auth_.getUserID())) {
            cout << "Two-factor authentication enabled successfully!" << endl;
        }
    }
    
    return true;
}

// Helper to prompt for registration info in console - PHONE NOW MANDATORY
bool SessionManager::promptRegistrationInfo(string& name, string& email, 
                                           string& phone, string& password) {
    cout << "\n=== User Registration ===" << endl;
    
    cout << "Enter full name: ";
    getline(cin, name);
    
    cout << "Enter email: ";
    getline(cin, email);
    
    // Phone is now MANDATORY
    cout << "Enter phone number (required, e.g., +1234567890): ";
    getline(cin, phone);
    
    cout << "Enter password: ";
    getline(cin, password);
    
    cout << "Confirm password: ";
    string confirmPassword;
    getline(cin, confirmPassword);
    
    if (password != confirmPassword) {
        cout << "Passwords do not match!" << endl;
        return false;
    }
    
    // Validate required fields - PHONE NOW REQUIRED
    if (name.empty() || email.empty() || phone.empty() || password.empty()) {
        cout << "Name, email, phone number, and password are all required fields" << endl;
        return false;
    }
    
    // Basic phone validation
    if (phone.length() < 10) {
        cout << "Please enter a valid phone number with at least 10 digits" << endl;
        return false;
    }
    
    return true;
}