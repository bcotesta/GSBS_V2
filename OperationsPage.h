#pragma once
#include "Page.h"
#include "User.h"
#include "Account.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QVBoxLayout>
#include <vector>

class OperationsPage : public Page {
    Q_OBJECT

public:
    OperationsPage();
    ~OperationsPage() override;

    void setUser(User* user);
    void setAccounts(std::vector<Account>* accounts);

protected:
    void buildUI() override;
    void onShow() override;

private slots:
    void handleDeposit();
    void handleWithdraw();
    void handleTransfer();

private:
    User* currentUser_;
    std::vector<Account>* accounts_;

    QWidget* containerWidget_;
    QLabel* titleLabel_;

    // Deposit
    QComboBox* accountSelectDeposit_;
    QLineEdit* depositAmount_;
    QPushButton* depositBtn_;

    // Withdraw
    QComboBox* accountSelectWithdraw_;
    QLineEdit* withdrawAmount_;
    QPushButton* withdrawBtn_;

    // Transfer
    QComboBox* accountSelectFrom_;
    QComboBox* accountSelectTo_;
    QLineEdit* transferAmount_;
    QPushButton* transferBtn_;

    void loadAccountsIntoDropdowns();
};
