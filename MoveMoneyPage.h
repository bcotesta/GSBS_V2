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
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTableWidget>
#include <vector>
#include <functional>

class MoveMoneyPage : public Page
{
    Q_OBJECT

public:
    MoveMoneyPage();
    ~MoveMoneyPage() override;

    // Lifecycle hooks
    void onShow() override;

    // Set the current user
    void setUser(User* user);
    
    // Callback for when transactions are completed (to refresh dashboard)
    void setTransactionCompleteCallback(std::function<void()> callback);

protected:
    void buildUI() override;

private slots:
    // Card click handlers
    void onETransferClicked();
    void onTransferFundsClicked();
    void onMiniStatementClicked();
    void onEDepositClicked();
    
    // Overlay navigation
    void showOverlay(int overlayIndex);
    void hideOverlay();
    
    // Action handlers
    void handleETransferSend();
    void handleTransfer();
    void handleGenerateMiniStatement();
    void handleExportPdf();

private:
    // Helper function to create action cards
    QPushButton* createActionCard(const QString& title, const QString& iconPath, QWidget* parent);
    
    // Overlay creation functions
    QWidget* createETransferOverlay();
    QWidget* createTransferFundsOverlay();
    QWidget* createMiniStatementOverlay();
    QWidget* createEDepositOverlay();
    
    // Helper for creating overlay header with back button
    QWidget* createOverlayHeader(const QString& title, QWidget* parent);
    
    // Load user accounts using AccountManager
    void loadUserAccounts();
    
    // Helper to format account display text with type, number, and balance
    QString formatAccountDisplay(const Account& account) const;
    
    // Helper to get account type as string
    QString accountTypeToString(AccountType type) const;
    
    // Notify parent that a transaction was completed
    void notifyTransactionComplete();

    // UI Components
    QStackedWidget* stackedWidget_;
    QWidget* mainMenuWidget_;
    QWidget* containerWidget_;
    QLabel* titleLabel_;

    // Overlay widgets
    QWidget* eTransferOverlay_;
    QWidget* transferFundsOverlay_;
    QWidget* miniStatementOverlay_;
    QWidget* eDepositOverlay_;
    
    // eTransfer input fields
    QComboBox* eTransferAccountSelect_;
    QLineEdit* eTransferRecipientInput_;
    QLineEdit* eTransferAmountInput_;
    QLineEdit* eTransferMessageInput_;
    
    // Transfer input fields
    QComboBox* transferFromAccount_;
    QComboBox* transferToAccount_;
    QLineEdit* transferAmountInput_;
    
    // Mini statement components
    QComboBox* miniStatementAccountSelect_;
    QTableWidget* miniStatementTable_;
    QPushButton* generateStatementButton_;
    QPushButton* exportPdfButton_;

    // Current user and accounts (loaded from DB via AccountManager)
    User* currentUser_;
    std::vector<Account> accounts_;
    
    // Callback for transaction completion
    std::function<void()> onTransactionComplete_;
    
    // Overlay indices
    enum OverlayIndex {
        MAIN_MENU = 0,
        ETRANSFER = 1,
        TRANSFER_FUNDS = 2,
        MINI_STATEMENT = 3,
        EDEPOSIT = 4
    };
};