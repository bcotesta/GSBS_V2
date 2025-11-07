
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
#include <QtWidgets/QFrame>
#include <vector>
#include <functional>

class DashboardPage : public Page
{
    Q_OBJECT

public:
    DashboardPage();
    ~DashboardPage() override;
    
    // Lifecycle hooks
    void onShow() override;
    
    // Set the current user
    void setUser(User* user);
    
    // Callback for when an account card is clicked
    void setAccountClickCallback(std::function<void(const Account&)> callback);
    
protected:
    void buildUI() override;
    
private slots:
    void onAccountCardClicked();
    
private:
    // UI Components
    QScrollArea* scrollArea_;
    QWidget* scrollContent_;
    QVBoxLayout* contentLayout_;
    
    // Header section
    QWidget* headerWidget_;
    QLabel* titleLabel_;
    
    // Account sections
    QWidget* depositAccountsSection_;
    QVBoxLayout* depositAccountsLayout_;
    QLabel* depositAccountsLabel_;
    QLabel* depositTotalLabel_;
    
    QWidget* creditCardsSection_;
    QVBoxLayout* creditCardsLayout_;
    QLabel* creditCardsLabel_;
    
    // Account cards storage
    std::vector<QPushButton*> accountCards_;
    std::vector<Account> accounts_;
    
    // Current user
    User* currentUser_;
    
    // Callback
    std::function<void(const Account&)> onAccountClick_;
    
    // Helper methods
    void refreshAccountsDisplay();
    void clearAccountCards();
    QPushButton* createAccountCard(const Account& account, bool isDeposit);
    QString accountTypeToString(AccountType type) const;
    QString formatCurrency(double amount) const;
    QWidget* createSectionHeader(const QString& title);
};