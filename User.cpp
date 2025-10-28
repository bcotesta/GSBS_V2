// Brandon Cotesta | 10/27/2025 | 8:51 PM
#include "User.h"
#include "DatabaseManager.h"
#include "Authenticator.h"
#include <iostream>

// Constructor
User::User(string userID, string username, string password)
    : userId_(0), name_(""), email_(username), phone_(""), address_(""), passwordHash_(password)
{
    try {
        // Convert userID string to int
        userId_ = std::stoi(userID);
        
        // Load user information from database
        DatabaseManager& db = DatabaseManager::getInstance();
        std::string whereClause = "userID = '" + userID + "'";
        std::string selectCols = "name, email, phone";
        
        auto results = db.retMultipleColumns(selectCols, "userinfo", whereClause);
        
        if (!results.empty()) {
            name_ = static_cast<std::string>(results["name"]);
            email_ = static_cast<std::string>(results["email"]);
            phone_ = static_cast<std::string>(results["phone"]);
        }
        
        std::cout << "User object created for: " << name_ << " (ID: " << userId_ << ")" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in User constructor: " << e.what() << std::endl;
    }
}

// Getters
int User::userId() const {
    return userId_;
}

string User::name() const {
    return name_;
}

string User::email() const {
    return email_;
}

string User::phone() const {
    return phone_;
}

string User::address() const {
    return address_;
}

string User::passwordHash() const {
    return passwordHash_;
}

// Login - verify credentials match
bool User::login(string u, string p) const {
    Authenticator& auth = Authenticator::getInstance();
    auth.setValidInfo(u, p);
    
    // Check if credentials match this user
    if (auth.getUserID() == std::to_string(userId_) && !auth.getUserID().empty()) {
        std::cout << "Login successful for user: " << name_ << std::endl;
        return true;
    }
    
    std::cerr << "Login failed: Invalid credentials" << std::endl;
    return false;
}

// Logout - clear session
void User::logout() const {
    std::cout << "User " << name_ << " logged out successfully" << std::endl;
    // Additional logout logic can be added here
}

// Update user profile in database
void User::updateProfile(string id, string n, string e, string p, string ph) {
    DatabaseManager& db = DatabaseManager::getInstance();
    
    try {
        // Build SET clause with new values
        std::string setClause = "name = '" + n + "', " +
                                "email = '" + e + "', " +
                                "password = '" + p + "', " +
                                "phone = '" + ph + "'";
        
        // Build WHERE clause using userID
        std::string whereClause = "userID = '" + id + "'";
        
        // Update the database
        db.updateTable("userinfo", setClause, whereClause);
        
        // Update local member variables
        name_ = n;
        email_ = e;
        phone_ = ph;
        passwordHash_ = p;
        
        std::cout << "Profile updated successfully for user: " << name_ << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error updating profile: " << e.what() << std::endl;
    }
}

// Save user to database (for new users)
void User::saveToDatabase() {
    DatabaseManager& db = DatabaseManager::getInstance();
    
    try {
        // Format: (userID, name, email, phone, password)
        std::string values = "('" + std::to_string(userId_) + "', '" +
                             name_ + "', '" +
                             email_ + "', '" +
                             phone_ + "', '" +
                             passwordHash_ + "')";
        
        db.addtoTable("userinfo", values);
        
        std::cout << "User saved to database: " << name_ << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving user to database: " << e.what() << std::endl;
    }
}
