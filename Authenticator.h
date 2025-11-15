// Brandon Cotesta | 10/27/2025 | 10:00 PM

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
    // Verifies username and password against the database.
    bool verifyCredentials(const std::string& username, const std::string& password);
    // Stores validated login information.
    void setValidInfo(const std::string& username, const std::string& password);
    // Registers a new user in the database.
    bool registerNewUser(const std::string& name, const std::string& email, 
                         const std::string& phone, const std::string& password);
    
    // 2FA methods
    // Checks if a user has two-factor authentication enabled.
    bool isTwoFactorEnabled(const std::string& userID) const;
    // Returns the user's preferred 2FA method ("email" or "phone").
    std::string getTwoFactorMethod(const std::string& userID) const;
    // Sends the OTP code using the selected method.
    bool sendOTP(const std::string& userID, const std::string& method = "");
    // Verifies the entered OTP.
    bool verifyOTP(const std::string& userID, const std::string& code);
    
    // Getters
    std::string getUserID() const { return validUserID_; }
    std::string getUsername() const { return validUsername_; }
    std::string getName() const { return validName_; }
    std::string getPhone() const { return validPhone_; }

private:
    Authenticator();
    ~Authenticator();
    // Stored user information after successful login and authentication.
    string validUserID_;
    string validUsername_;
    string validPassword_;
    string validName_;
    string validPhone_;
    string validAddress_;
    string validAccountNum_;
};