#include "SessionManager.h"
#include <iostream>

using namespace std;

SessionManager::SessionManager()
    : auth_(Authenticator::getInstance()), currentUser_(nullptr), loggedIn_(false)
{
}

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