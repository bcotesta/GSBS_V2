// Brandon Cotesta || 10/28/2025 | 1:00 PM

#include "SessionManager.h"
#include <iostream>

using namespace std;

// Constructor that grabs the singleton Authenticator instance
SessionManager::SessionManager()
    : auth_(Authenticator::getInstance()), currentUser_(nullptr), loggedIn_(false)
{
}

// Login method
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
    
    // Create user object
    currentUser_ = make_unique<User>(auth_.getUserID(), username, password);
    loggedIn_ = true;
    
    cout << "Login successful!" << endl;
    return true;
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

// Registration method
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
    
    cout << "Registration successful! You are now logged in." << endl;
    return true;
}

// Helper to prompt for registration info in console
bool SessionManager::promptRegistrationInfo(string& name, string& email, 
                                           string& phone, string& password) {
    cout << "\n=== User Registration ===" << endl;
    
    cout << "Enter full name: ";
    getline(cin, name);
    
    cout << "Enter email: ";
    getline(cin, email);
    
    cout << "Enter phone number: ";
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
    
    if (name.empty() || email.empty() || password.empty()) {
        cout << "Name, email, and password are required fields" << endl;
        return false;
    }
    
    return true;
}