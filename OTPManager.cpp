#include "OTPManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>

OTPManager& OTPManager::getInstance() {
    static OTPManager instance;
    return instance;
}

OTPManager::OTPManager() {
    // Seed random number generator
    std::random_device rd;
    rng_.seed(rd());
}

std::string OTPManager::generateOTP(const std::string& identifier) {
    // Generate 6-digit code
    std::uniform_int_distribution<int> dist(100000, 999999);
    int code = dist(rng_);
    
    // Store OTP with expiration time
    OTPData data;
    data.code = std::to_string(code);
    data.expirationTime = std::chrono::system_clock::now() + 
                          std::chrono::seconds(expirationSeconds_);
    
    otpStore_[identifier] = data;
    
    std::cout << "[OTP] Generated OTP for " << identifier << ": " << data.code << std::endl;
    std::cout << "[OTP] Expires in " << expirationSeconds_ << " seconds" << std::endl;
    
    return data.code;
}

bool OTPManager::verifyOTP(const std::string& identifier, const std::string& code) {
    auto it = otpStore_.find(identifier);
    
    if (it == otpStore_.end()) {
        std::cout << "[OTP] No OTP found for: " << identifier << std::endl;
        return false;
    }
    
    // Check if expired
    if (std::chrono::system_clock::now() > it->second.expirationTime) {
        std::cout << "[OTP] OTP expired for: " << identifier << std::endl;
        otpStore_.erase(it);
        return false;
    }
    
    // Check if code matches
    if (it->second.code == code) {
        std::cout << "[OTP] OTP verified successfully for: " << identifier << std::endl;
        otpStore_.erase(it); // Remove used OTP
        return true;
    }
    
    std::cout << "[OTP] Invalid OTP for: " << identifier << std::endl;
    return false;
}

void OTPManager::clearOTP(const std::string& identifier) {
    otpStore_.erase(identifier);
    std::cout << "[OTP] Cleared OTP for: " << identifier << std::endl;
}