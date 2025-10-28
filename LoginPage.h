#pragma once
#include "Page.h"
#include "SessionManager.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <functional>

class LoginPage : public Page
{
public:
    LoginPage();
    ~LoginPage() override;
    
    // Lifecycle hooks
    void onShow() override;
    
    // Signals (using Qt's signal/slot mechanism would be better, but for simplicity)
    void setLoginSuccessCallback(std::function<void()> callback);
    
protected:
    void buildUI() override;
    
private:
    // UI Components
    QLineEdit* usernameInput_;
    QLineEdit* passwordInput_;
    QPushButton* loginButton_;
    QPushButton* registerButton_;
    QLabel* statusLabel_;
    
    // Callback
    std::function<void()> onLoginSuccess_;
    
    // Handlers
    void handleLogin();
    void handleRegister();
};