// OTP Manager for 2FA authentication
#pragma once
#include <string>
#include <map>
#include <chrono>
#include <random>

class OTPManager {
public:
    static OTPManager& getInstance();
    
    // Delete copy constructor and assignment operator
    OTPManager(const OTPManager&) = delete;
    OTPManager& operator=(const OTPManager&) = delete;
    
    // Generate a 6-digit OTP code
    std::string generateOTP(const std::string& identifier);
    
    // Verify OTP code (returns true if valid and not expired)
    bool verifyOTP(const std::string& identifier, const std::string& code);
    
    // Clear OTP for user
    void clearOTP(const std::string& identifier);
    
    // Set OTP expiration time in seconds (default: 5 minutes)
    void setExpirationTime(int seconds) { expirationSeconds_ = seconds; }
    
private:
    OTPManager();
    ~OTPManager() = default;
    
    struct OTPData {
        std::string code;
        std::chrono::system_clock::time_point expirationTime;
    };
    
    std::map<std::string, OTPData> otpStore_;
    int expirationSeconds_ = 300; // 5 minutes default
    std::mt19937 rng_;
};