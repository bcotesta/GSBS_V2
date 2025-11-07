#pragma once
#include "Page.h"
#include "Authenticator.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <functional>

class OTPPage : public Page {
public:
    OTPPage();
    ~OTPPage() override;
    
    void onShow() override;
    
    // Set the user ID for OTP verification
    void setUserID(const std::string& userID);
    
    // Set the method used for OTP delivery
    void setDeliveryMethod(const std::string& method);
    
    // Set callbacks
    void setVerificationSuccessCallback(std::function<void()> callback);
    void setVerificationFailedCallback(std::function<void()> callback);
    void setCancelCallback(std::function<void()> callback);
    
protected:
    void buildUI() override;
    
private:
    // UI Components
    QWidget* containerWidget_;
    QLabel* titleLabel_;
    QLabel* instructionLabel_;
    QLineEdit* otpInput_;
    QPushButton* verifyButton_;
    QPushButton* resendButton_;
    QPushButton* cancelButton_;
    QLabel* statusLabel_;
    QLabel* timerLabel_;
    
    // Data
    Authenticator& auth_;
    std::string userID_;
    std::string deliveryMethod_;
    QTimer* countdownTimer_;
    int remainingSeconds_;
    
    // Callbacks
    std::function<void()> onVerificationSuccess_;
    std::function<void()> onVerificationFailed_;
    std::function<void()> onCancel_;
    
    // Methods
    void handleVerify();
    void handleResend();
    void handleCancel();
    void startCountdown();
    void updateTimer();
    void sendOTP();
    void showError(const QString& message);
    void showSuccess(const QString& message);
    void showInfo(const QString& message);
};