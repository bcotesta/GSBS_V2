// Brandon Cotesta || 10/28/2025 | 3:00 PM
// Dashboard page for displaying user and account information

#include "DashboardPage.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QScrollArea>
#include <iomanip>
#include <sstream>

DashboardPage::DashboardPage()
    : Page("Dashboard"),
      scrollArea_(nullptr),
      containerWidget_(nullptr),
      titleLabel_(nullptr),
      welcomeLabel_(nullptr),
      userInfoCard_(nullptr),
      userIdLabel_(nullptr),
      nameLabel_(nullptr),
      emailLabel_(nullptr),
      phoneLabel_(nullptr),
      accountsContainer_(nullptr),
      currentUser_(nullptr)
{
}

DashboardPage::~DashboardPage() {
    // Qt's parent-child system handles cleanup
}

void DashboardPage::setUser(User* user) {
    currentUser_ = user;
}

void DashboardPage::buildUI() {
    QWidget* centralWidget = getCentralWidget();
    
    // Set background color for the page - EXACT match
    centralWidget->setStyleSheet("QWidget { background-color: #f5f5f5; }");
    
    // Create a centered container like LoginPage
    containerWidget_ = new QWidget(centralWidget);
    containerWidget_->setFixedWidth(400);
    containerWidget_->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   color: black;"
        "   border-radius: 12px;"
        "}"
    );
    
    QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget_);
    containerLayout->setSpacing(15);
    containerLayout->setContentsMargins(40, 35, 40, 35);
    
    // Title - matching LoginPage title style
    titleLabel_ = new QLabel("Dashboard", containerWidget_);
    QFont titleFont("Segoe UI", 28, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    titleLabel_->setMinimumHeight(45);
    containerLayout->addWidget(titleLabel_);
    
    containerLayout->addSpacing(5);
    
    // Welcome message - matching LoginPage subtitle style
    welcomeLabel_ = new QLabel("Welcome back!", containerWidget_);
    QFont welcomeFont("Segoe UI", 11);
    welcomeLabel_->setFont(welcomeFont);
    welcomeLabel_->setAlignment(Qt::AlignCenter);
    welcomeLabel_->setStyleSheet("QLabel { color: #7f8c8d; }");
    containerLayout->addWidget(welcomeLabel_);
    
    containerLayout->addSpacing(25);
    
    // User Information Section
    QLabel* userInfoLabel = new QLabel("User Information", containerWidget_);
    userInfoLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; font-size: 12px; }");
    containerLayout->addWidget(userInfoLabel);
    
    containerLayout->addSpacing(5);
    
    // User ID
    userIdLabel_ = new QLabel("User ID: -", containerWidget_);
    QFont infoFont("Segoe UI", 11);
    userIdLabel_->setFont(infoFont);
    userIdLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    containerLayout->addWidget(userIdLabel_);
    
    // Name
    nameLabel_ = new QLabel("Name: -", containerWidget_);
    nameLabel_->setFont(infoFont);
    nameLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    containerLayout->addWidget(nameLabel_);
    
    // Email
    emailLabel_ = new QLabel("Email: -", containerWidget_);
    emailLabel_->setFont(infoFont);
    emailLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    emailLabel_->setWordWrap(true);
    containerLayout->addWidget(emailLabel_);
    
    // Phone
    phoneLabel_ = new QLabel("Phone: -", containerWidget_);
    phoneLabel_->setFont(infoFont);
    phoneLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    containerLayout->addWidget(phoneLabel_);
    
    containerLayout->addSpacing(15);
    
    // Divider
    QFrame* divider = new QFrame(containerWidget_);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("QFrame { color: #e0e0e0; }");
    containerLayout->addWidget(divider);
    
    containerLayout->addSpacing(5);
    
    // Account Summary Label
    QLabel* accountSummaryLabel = new QLabel("Account Summary", containerWidget_);
    accountSummaryLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; font-size: 12px; }");
    containerLayout->addWidget(accountSummaryLabel);
    
    containerLayout->addSpacing(5);
    
    // Total Balance Display
    userInfoCard_ = new QWidget(containerWidget_);
    userInfoCard_->setStyleSheet(
        "QWidget {"
        "   background-color: #3498db;"
        "   border-radius: 8px;"
        "}"
    );
    
    QVBoxLayout* balanceLayout = new QVBoxLayout(userInfoCard_);
    balanceLayout->setSpacing(5);
    balanceLayout->setContentsMargins(20, 15, 20, 15);
    
    QLabel* totalLabel = new QLabel("Total Balance", userInfoCard_);
    totalLabel->setStyleSheet("QLabel { color: white; font-size: 11px; }");
    totalLabel->setAlignment(Qt::AlignCenter);
    balanceLayout->addWidget(totalLabel);
    
    // This will be updated in onShow
    QLabel* totalAmountLabel = new QLabel("$0.00", userInfoCard_);
    QFont totalFont("Segoe UI", 24, QFont::Bold);
    totalAmountLabel->setFont(totalFont);
    totalAmountLabel->setStyleSheet("QLabel { color: white; }");
    totalAmountLabel->setAlignment(Qt::AlignCenter);
    totalAmountLabel->setObjectName("totalAmountLabel");
    balanceLayout->addWidget(totalAmountLabel);
    
    containerLayout->addWidget(userInfoCard_);
    
    containerLayout->addSpacing(10);
    
    // Account count info
    accountsContainer_ = new QWidget(containerWidget_);
    QVBoxLayout* accountsLayout = new QVBoxLayout(accountsContainer_);
    accountsLayout->setSpacing(5);
    accountsLayout->setContentsMargins(0, 0, 0, 0);
    
    // This will show number of accounts
    QLabel* accountCountLabel = new QLabel("No accounts", accountsContainer_);
    accountCountLabel->setObjectName("accountCountLabel");
    accountCountLabel->setStyleSheet("QLabel { color: #7f8c8d; font-size: 11px; }");
    accountCountLabel->setAlignment(Qt::AlignCenter);
    accountsLayout->addWidget(accountCountLabel);
    
    containerLayout->addWidget(accountsContainer_);
    
    // Add container to main layout with centering - EXACTLY like LoginPage
    mainLayout_->addStretch();
    
    // Horizontal centering
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(containerWidget_);
    hLayout->addStretch();
    mainLayout_->addLayout(hLayout);
    
    mainLayout_->addStretch();
}

void DashboardPage::buildUserInfoSection(QVBoxLayout* layout) {
    // Not used anymore - keeping for compatibility
}

void DashboardPage::buildAccountsSection(QVBoxLayout* layout) {
    // Not used anymore - keeping for compatibility
}

QWidget* DashboardPage::createAccountCard(const Account& account) {
    // Not used anymore - keeping for compatibility
    return nullptr;
}

QString DashboardPage::accountTypeToString(AccountType type) const {
    switch (type) {
        case AccountType::CHEQUING:
            return "Chequing";
        case AccountType::SAVINGS:
            return "Savings";
        case AccountType::CREDIT:
            return "Credit";
        case AccountType::LOAN:
            return "Loan";
        default:
            return "Unknown";
    }
}

void DashboardPage::refreshAccountsDisplay() {
    if (!accountsContainer_ || !currentUser_) {
        return;
    }
    
    // Load accounts for current user
    AccountManager accountMgr(*currentUser_);
    std::vector<Account> accounts = accountMgr.loadUserAccounts();
    
    // Calculate total balance
    double totalBalance = 0.0;
    for (const Account& account : accounts) {
        totalBalance += account.getBalance();
    }
    
    // Update total balance display
    QLabel* totalAmountLabel = userInfoCard_->findChild<QLabel*>("totalAmountLabel");
    if (totalAmountLabel) {
        std::stringstream balanceStream;
        balanceStream << std::fixed << std::setprecision(2) << totalBalance;
        totalAmountLabel->setText(QString("$%1").arg(QString::fromStdString(balanceStream.str())));
    }
    
    // Update account count
    QLabel* accountCountLabel = accountsContainer_->findChild<QLabel*>("accountCountLabel");
    if (accountCountLabel) {
        if (accounts.empty()) {
            accountCountLabel->setText("No accounts");
        } else if (accounts.size() == 1) {
            accountCountLabel->setText("1 account");
        } else {
            accountCountLabel->setText(QString("%1 accounts").arg(accounts.size()));
        }
    }
}

void DashboardPage::onShow() {
    // Update user information display
    if (currentUser_) {
        welcomeLabel_->setText(QString("Welcome back, %1!").arg(QString::fromStdString(currentUser_->name())));
        
        userIdLabel_->setText(QString("User ID: %1").arg(currentUser_->userId()));
        nameLabel_->setText(QString("Name: %1").arg(QString::fromStdString(currentUser_->name())));
        emailLabel_->setText(QString("Email: %1").arg(QString::fromStdString(currentUser_->email())));
        
        QString phone = QString::fromStdString(currentUser_->phone());
        if (phone.isEmpty() || phone == "") {
            phoneLabel_->setText("Phone: Not provided");
        } else {
            phoneLabel_->setText(QString("Phone: %1").arg(phone));
        }
        
        // Refresh accounts display
        refreshAccountsDisplay();
    }
}