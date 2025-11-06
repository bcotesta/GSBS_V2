#include "OperationsPage.h"
#include <QtWidgets/QHBoxLayout>
#include <iostream>

OperationsPage::OperationsPage()
    : Page("operations"), currentUser_(nullptr), accounts_(nullptr) {
}

OperationsPage::~OperationsPage() {}

void OperationsPage::setUser(User* user) { currentUser_ = user; }
void OperationsPage::setAccounts(std::vector<Account>* accounts) { accounts_ = accounts; }

void OperationsPage::buildUI() {
    QWidget* central = getCentralWidget();

    // White background for entire page
    central->setStyleSheet("QWidget { background-color: #ffffff; }");

    // Centered card container (white, rounded)
    containerWidget_ = new QWidget(central);
    containerWidget_->setFixedWidth(420);
    containerWidget_->setStyleSheet(
        "QWidget {"
        " background-color: white;"
        " border-radius: 12px;"
        " color: black;"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(containerWidget_);
    layout->setSpacing(14);
    layout->setContentsMargins(35, 30, 35, 30);

    // Title
    titleLabel_ = new QLabel("Operations", containerWidget_);
    titleLabel_->setFont(QFont("Segoe UI", 26, QFont::Bold));
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("color: black;");
    layout->addWidget(titleLabel_);
    layout->addSpacing(10);

    // Inputs style
    QString inputStyle =
        "QLineEdit, QComboBox {"
        " padding: 10px;"
        " border: 2px solid #e0e0e0;"
        " border-radius: 8px;"
        " background: white;"
        " font-size: 14px;"
        "}";

    // Buttons style (GREEN BUTTONS, WHITE TEXT)
    QString greenBtnStyle =
        "QPushButton {"
        " background-color: #00cc00 !important;"
        " color: white !important;"
        " border: none;"
        " padding: 12px;"
        " border-radius: 8px;"
        " font-size: 15px;"
        " font-weight: 600;"
        "}"
        "QPushButton:hover { background-color: #02a802 !important; }"
        "QPushButton:pressed { background-color: #008f39 !important; }";

    // ==================== Deposit ====================
    layout->addWidget(new QLabel("Deposit", containerWidget_));

    accountSelectDeposit_ = new QComboBox(containerWidget_);
    accountSelectDeposit_->setStyleSheet(inputStyle);

    depositAmount_ = new QLineEdit(containerWidget_);
    depositAmount_->setPlaceholderText("Amount ($)");
    depositAmount_->setStyleSheet(inputStyle);

    depositBtn_ = new QPushButton("Deposit");
    depositBtn_->setStyleSheet(greenBtnStyle);

    layout->addWidget(accountSelectDeposit_);
    layout->addWidget(depositAmount_);
    layout->addWidget(depositBtn_);

    // ==================== Withdraw ====================
    layout->addSpacing(10);
    layout->addWidget(new QLabel("Withdraw", containerWidget_));

    accountSelectWithdraw_ = new QComboBox(containerWidget_);
    accountSelectWithdraw_->setStyleSheet(inputStyle);

    withdrawAmount_ = new QLineEdit(containerWidget_);
    withdrawAmount_->setPlaceholderText("Amount ($)");
    withdrawAmount_->setStyleSheet(inputStyle);

    withdrawBtn_ = new QPushButton("Withdraw");
    withdrawBtn_->setStyleSheet(greenBtnStyle);

    layout->addWidget(accountSelectWithdraw_);
    layout->addWidget(withdrawAmount_);
    layout->addWidget(withdrawBtn_);

    // ==================== Transfer ====================
    layout->addSpacing(10);
    layout->addWidget(new QLabel("Transfer Between Accounts", containerWidget_));

    accountSelectFrom_ = new QComboBox(containerWidget_);
    accountSelectFrom_->setStyleSheet(inputStyle);

    accountSelectTo_ = new QComboBox(containerWidget_);
    accountSelectTo_->setStyleSheet(inputStyle);

    transferAmount_ = new QLineEdit(containerWidget_);
    transferAmount_->setPlaceholderText("Amount ($)");
    transferAmount_->setStyleSheet(inputStyle);

    transferBtn_ = new QPushButton("Transfer");
    transferBtn_->setStyleSheet(greenBtnStyle);

    layout->addWidget(accountSelectFrom_);
    layout->addWidget(accountSelectTo_);
    layout->addWidget(transferAmount_);
    layout->addWidget(transferBtn_);

    // Center UI
    mainLayout_->addStretch();
    QHBoxLayout* h = new QHBoxLayout();
    h->addStretch();
    h->addWidget(containerWidget_);
    h->addStretch();
    mainLayout_->addLayout(h);
    mainLayout_->addStretch();

    // Signals
    connect(depositBtn_, &QPushButton::clicked, this, &OperationsPage::handleDeposit);
    connect(withdrawBtn_, &QPushButton::clicked, this, &OperationsPage::handleWithdraw);
    connect(transferBtn_, &QPushButton::clicked, this, &OperationsPage::handleTransfer);
}

void OperationsPage::onShow() {
    if (accounts_) loadAccountsIntoDropdowns();
}

void OperationsPage::loadAccountsIntoDropdowns() {
    accountSelectDeposit_->clear();
    accountSelectWithdraw_->clear();
    accountSelectFrom_->clear();
    accountSelectTo_->clear();

    for (const auto& acc : *accounts_) {
        QString label = QString("%1 - $%2")
            .arg(acc.accountNumber().c_str())
            .arg(acc.getBalance());

        accountSelectDeposit_->addItem(label);
        accountSelectWithdraw_->addItem(label);
        accountSelectFrom_->addItem(label);
        accountSelectTo_->addItem(label);
    }
}

// Debug actions (will wire to DB next)
void OperationsPage::handleDeposit() { std::cout << "Deposit clicked\n"; }
void OperationsPage::handleWithdraw() { std::cout << "Withdraw clicked\n"; }
void OperationsPage::handleTransfer() { std::cout << "Transfer clicked\n"; }

