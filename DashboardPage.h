#pragma once
#include "Page.h"
#include "User.h"
#include "Account.h"
#include "AccountManager.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <vector>
#include <functional>

class DashboardPage : public Page
{
public:
    DashboardPage();
    ~DashboardPage() override;
    
    // Lifecycle hooks
    void onShow() override;
    
    // Set the current user
    void setUser(User* user);
    
protected:
    void buildUI() override;
    
private:
    // UI Components
    QScrollArea* scrollArea_;
    QWidget* containerWidget_;
    QLabel* titleLabel_;
    QLabel* welcomeLabel_;
    
    // User info section
    QWidget* userInfoCard_;
    QLabel* userIdLabel_;
    QLabel* nameLabel_;
    QLabel* emailLabel_;
    QLabel* phoneLabel_;
    
    // Accounts section
    QWidget* accountsContainer_;
    std::vector<QWidget*> accountCards_;
    
    // Current user
    User* currentUser_;
    
    // Helper methods
    void buildUserInfoSection(QVBoxLayout* layout);
    void buildAccountsSection(QVBoxLayout* layout);
    void refreshAccountsDisplay();
    QWidget* createAccountCard(const Account& account);
    QString accountTypeToString(AccountType type) const;
};