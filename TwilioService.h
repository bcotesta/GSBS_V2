// Twilio SMS Service for 2FA Authentication
// Integrates with Twilio REST API to send SMS messages

#pragma once
#include <string>

class TwilioService {
public:
    static TwilioService& getInstance();
    
    // Delete copy constructor and assignment operator
    TwilioService(const TwilioService&) = delete;
    TwilioService& operator=(const TwilioService&) = delete;
    
    // Configure Twilio credentials
    void configure(const std::string& accountSid, 
                   const std::string& authToken, 
                   const std::string& fromNumber);
    
    // Send SMS message
    bool sendSMS(const std::string& toNumber, const std::string& message);
    
    // Check if Twilio is configured
    bool isConfigured() const { 
        return !accountSid_.empty() && !authToken_.empty() && !fromNumber_.empty(); 
    }
    
private:
    TwilioService();
    ~TwilioService();
    
    std::string accountSid_;
    std::string authToken_;
    std::string fromNumber_;
    
    // Helper function for URL encoding
    std::string urlEncode(const std::string& value);
};