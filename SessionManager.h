#pragma once
#include "Authenticator.h"
#include "User.h"
#include <memory>
#include <string>

class SessionManager {
public:
    SessionManager();
    
    // Session operations
    bool login();
    bool registerUser();
    void logout();
    bool isLoggedIn() const;
    
    // User access
    User* getCurrentUser();
    const User* getCurrentUser() const;
    
private:
    Authenticator& auth_;
    std::unique_ptr<User> currentUser_;
    bool loggedIn_;
    
    bool promptCredentials(std::string& username, std::string& password);
    bool promptRegistrationInfo(std::string& name, std::string& email,
                                std::string& phone, std::string& password);
};