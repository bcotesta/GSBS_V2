#pragma once
#include "Page.h"
#include "Authenticator.h"
#include "User.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <functional>
#include <memory>

class RegistrationPage : public Page
{
public:
    RegistrationPage();
    ~RegistrationPage() override;
    
    // Lifecycle hooks
    void onShow() override;
    
    // Callbacks for navigation
    void setRegistrationSuccessCallback(std::function<void(User*)> callback);
    void setBackToLoginCallback(std::function<void()> callback);
    
protected:
    void buildUI() override;
    
private:
    // UI Components
    QWidget* containerWidget_;
    QLineEdit* nameInput_;
    QLineEdit* emailInput_;
    QLineEdit* phoneInput_;
    QLineEdit* passwordInput_;
    QLineEdit* confirmPasswordInput_;
    QPushButton* registerButton_;
    QPushButton* cancelButton_;
    QPushButton* backToLoginButton_;
    QLabel* statusLabel_;
    QLabel* titleLabel_;
    QCheckBox* showPasswordCheckbox_;
    
    // Callbacks
    std::function<void(User*)> onRegistrationSuccess_;
    std::function<void()> onBackToLoginClick_;
    
    // Backend
    Authenticator& auth_;
    
    // Handlers
    void handleRegister();
    void handleBackToLogin();
    void handleShowPasswordToggle(bool checked);
    
    // Helper
    void showError(const QString& message);
    void showSuccess(const QString& message);
    bool validateInputs();
};