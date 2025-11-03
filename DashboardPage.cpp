// Brandon Cotesta || 10/28/2025 | 3:00 PM
// Dashboard page for displaying user and account information

#include "DashboardPage.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <iomanip>
#include <sstream>

DashboardPage::DashboardPage()
    : Page("Dashboard"),
      scrollArea_(nullptr),
      scrollContent_(nullptr),
      contentLayout_(nullptr),
      headerWidget_(nullptr),
      titleLabel_(nullptr),
      depositAccountsSection_(nullptr),
      depositAccountsLayout_(nullptr),
      depositAccountsLabel_(nullptr),
      depositTotalLabel_(nullptr),
      creditCardsSection_(nullptr),
      creditCardsLayout_(nullptr),
      creditCardsLabel_(nullptr),
      currentUser_(nullptr)
{
}

DashboardPage::~DashboardPage() {
    // Qt's parent-child system handles cleanup
}

void DashboardPage::setUser(User* user) {
    currentUser_ = user;
}

void DashboardPage::setAccountClickCallback(std::function<void(const Account&)> callback) {
    onAccountClick_ = callback;
}

void DashboardPage::buildUI() {
    QWidget* centralWidget = getCentralWidget();
    
    // Set background color for the page
    centralWidget->setStyleSheet("QWidget { background-color: #f5f5f5; }");
    
    // Create scroll area for content
    scrollArea_ = new QScrollArea(centralWidget);
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea_->setFrameShape(QFrame::NoFrame);
    scrollArea_->setStyleSheet("QScrollArea { background-color: #f5f5f5; border: none; }");
    
    // Create content widget for scroll area
    scrollContent_ = new QWidget();
    scrollContent_->setStyleSheet("QWidget { background-color: #f5f5f5; }");
    contentLayout_ = new QVBoxLayout(scrollContent_);
    contentLayout_->setSpacing(0);
    contentLayout_->setContentsMargins(0, 0, 0, 20);
    
    // Header with title
    headerWidget_ = new QWidget(scrollContent_);
    headerWidget_->setStyleSheet("QWidget { background-color: white; }");
    headerWidget_->setFixedHeight(80);
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget_);
    headerLayout->setContentsMargins(20, 20, 20, 20);
    
    titleLabel_ = new QLabel("Home", headerWidget_);
    QFont titleFont("Segoe UI", 24, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    headerLayout->addWidget(titleLabel_);
    headerLayout->addStretch();
    
    contentLayout_->addWidget(headerWidget_);
    contentLayout_->addSpacing(15);
    
    // Deposit Accounts Section
    depositAccountsSection_ = new QWidget(scrollContent_);
    depositAccountsSection_->setStyleSheet("QWidget { background-color: transparent; }");
    depositAccountsLayout_ = new QVBoxLayout(depositAccountsSection_);
    depositAccountsLayout_->setSpacing(10);
    depositAccountsLayout_->setContentsMargins(15, 0, 15, 0);
    
    // Section header
    QWidget* depositHeader = createSectionHeader("DEPOSIT ACCOUNTS");
    depositAccountsLayout_->addWidget(depositHeader);
    
    // Account cards will be added dynamically in refreshAccountsDisplay()
    
    // Total label
    depositTotalLabel_ = new QLabel("Total  $0.00", depositAccountsSection_);
    QFont totalFont("Segoe UI", 14, QFont::Normal);
    depositTotalLabel_->setFont(totalFont);
    depositTotalLabel_->setStyleSheet(
        "QLabel { "
        "   color: #2c3e50; "
        "   padding: 15px 20px; "
        "   background-color: white; "
        "   border-radius: 8px;"
        "}"
    );
    depositTotalLabel_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    depositAccountsLayout_->addWidget(depositTotalLabel_);
    
    contentLayout_->addWidget(depositAccountsSection_);
    contentLayout_->addSpacing(20);
    
    // Credit Cards Section
    creditCardsSection_ = new QWidget(scrollContent_);
    creditCardsSection_->setStyleSheet("QWidget { background-color: transparent; }");
    creditCardsLayout_ = new QVBoxLayout(creditCardsSection_);
    creditCardsLayout_->setSpacing(10);
    creditCardsLayout_->setContentsMargins(15, 0, 15, 0);
    
    // Section header
    QWidget* creditHeader = createSectionHeader("CREDIT CARDS");
    creditCardsLayout_->addWidget(creditHeader);
    
    // Credit card cards will be added dynamically in refreshAccountsDisplay()
    
    contentLayout_->addWidget(creditCardsSection_);
    contentLayout_->addStretch();
    
    // Set scroll content
    scrollArea_->setWidget(scrollContent_);
    
    // Add scroll area to main layout
    mainLayout_->addWidget(scrollArea_);
}

QWidget* DashboardPage::createSectionHeader(const QString& title) {
    QWidget* header = new QWidget();
    header->setStyleSheet("QWidget { background-color: transparent; }");
    
    QHBoxLayout* layout = new QHBoxLayout(header);
    layout->setContentsMargins(5, 10, 5, 10);
    
    QLabel* label = new QLabel(title, header);
    QFont font("Segoe UI", 11, QFont::Bold);
    label->setFont(font);
    label->setStyleSheet("QLabel { color: #7f8c8d; }");
    layout->addWidget(label);
    layout->addStretch();
    
    return header;
}

QPushButton* DashboardPage::createAccountCard(const Account& account, bool isDeposit) {
    QPushButton* card = new QPushButton(scrollContent_);
    card->setCursor(Qt::PointingHandCursor);
    card->setFixedHeight(160);
    
    // Store account info in the button's property
    card->setProperty("accountNumber", QString::fromStdString(account.accountNumber()));
    
    // Card styling
    QString cardStyle = 
        "QPushButton {"
        "   background-color: white;"
        "   border: 1px solid #e0e0e0;"
        "   border-radius: 12px;"
        "   text-align: left;"
        "   padding: 0px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f8f9fa;"
        "   border: 1px solid #d0d0d0;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #e9ecef;"
        "}";
    
    card->setStyleSheet(cardStyle);
    
    // Create layout for card content
    QWidget* contentWidget = new QWidget(card);
    contentWidget->setStyleSheet("QWidget { background-color: transparent; }");
    
    QVBoxLayout* cardLayout = new QVBoxLayout(contentWidget);
    cardLayout->setContentsMargins(20, 15, 20, 15);
    cardLayout->setSpacing(8);
    
    // Account type and number
    QLabel* accountTypeLabel = new QLabel(accountTypeToString(account.accountType()), contentWidget);
    QFont typeFont("Segoe UI", 13, QFont::DemiBold);
    accountTypeLabel->setFont(typeFont);
    accountTypeLabel->setStyleSheet("QLabel { color: #2c3e50; }");
    cardLayout->addWidget(accountTypeLabel);
    
    QLabel* accountNumberLabel = new QLabel(QString::fromStdString(account.accountNumber()), contentWidget);
    QFont numberFont("Segoe UI", 11, QFont::Normal);
    accountNumberLabel->setFont(numberFont);
    accountNumberLabel->setStyleSheet("QLabel { color: #7f8c8d; }");
    cardLayout->addWidget(accountNumberLabel);
    
    cardLayout->addSpacing(10);
    
    // Balance
    QLabel* balanceLabel = new QLabel(formatCurrency(account.getBalance()), contentWidget);
    QFont balanceFont("Segoe UI", 26, QFont::Bold);
    balanceLabel->setFont(balanceFont);
    balanceLabel->setStyleSheet("QLabel { color: #2c3e50; }");
    cardLayout->addWidget(balanceLabel);
    
    cardLayout->addStretch();
    
    // Action buttons row (for deposit accounts)
    if (isDeposit) {
        QWidget* actionsWidget = new QWidget(contentWidget);
        actionsWidget->setStyleSheet("QWidget { background-color: transparent; }");
        QHBoxLayout* actionsLayout = new QHBoxLayout(actionsWidget);
        actionsLayout->setContentsMargins(0, 0, 0, 0);
        actionsLayout->setSpacing(10);
        
        // Debit badge
        QLabel* debitBadge = new QLabel("DEBIT", actionsWidget);
        debitBadge->setFixedSize(60, 24);
        debitBadge->setAlignment(Qt::AlignCenter);
        QFont badgeFont("Segoe UI", 9, QFont::Bold);
        debitBadge->setFont(badgeFont);
        debitBadge->setStyleSheet(
            "QLabel {"
            "   background-color: #00cc00;" // new green colour
            "   color: white;"
            "   border-radius: 4px;"
            "}"
        );
        actionsLayout->addWidget(debitBadge);
        
        actionsLayout->addStretch();
        
        cardLayout->addWidget(actionsWidget);
    }
    
    // Position content widget in button
    QVBoxLayout* buttonLayout = new QVBoxLayout(card);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addWidget(contentWidget);
    
    // Connect click handler
    connect(card, &QPushButton::clicked, this, &DashboardPage::onAccountCardClicked);
    
    return card;
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

QString DashboardPage::formatCurrency(double amount) const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << amount;
    return QString("$%1").arg(QString::fromStdString(ss.str()));
}

void DashboardPage::clearAccountCards() {
    // Remove all account cards from layouts
    for (auto* card : accountCards_) {
        card->deleteLater();
    }
    accountCards_.clear();
}

void DashboardPage::refreshAccountsDisplay() {
    if (!currentUser_) {
        return;
    }
    
    // Clear existing cards
    clearAccountCards();
    
    // Load accounts for current user
    AccountManager accountMgr(*currentUser_);
    accounts_ = accountMgr.loadUserAccounts();
    
    // Separate deposit and credit accounts
    double depositTotal = 0.0;
    double creditTotal = 0.0;
    
    // Add deposit account cards
    for (const Account& account : accounts_) {
        if (account.accountType() == AccountType::CHEQUING || 
            account.accountType() == AccountType::SAVINGS) {
            QPushButton* card = createAccountCard(account, true);
            depositAccountsLayout_->insertWidget(depositAccountsLayout_->count() - 1, card);
            accountCards_.push_back(card);
            depositTotal += account.getBalance();
        }
    }
    
    // Add credit card cards
    for (const Account& account : accounts_) {
        if (account.accountType() == AccountType::CREDIT || 
            account.accountType() == AccountType::LOAN) {
            QPushButton* card = createAccountCard(account, false);
            creditCardsLayout_->addWidget(card);
            accountCards_.push_back(card);
            creditTotal += account.getBalance();
        }
    }
    
    // Update totals
    depositTotalLabel_->setText(QString("Total   %1").arg(formatCurrency(depositTotal)));
    
    // Hide sections if no accounts
    depositAccountsSection_->setVisible(depositTotal > 0 || 
        std::any_of(accounts_.begin(), accounts_.end(), [](const Account& acc) {
            return acc.accountType() == AccountType::CHEQUING || 
                   acc.accountType() == AccountType::SAVINGS;
        }));
    
    creditCardsSection_->setVisible(
        std::any_of(accounts_.begin(), accounts_.end(), [](const Account& acc) {
            return acc.accountType() == AccountType::CREDIT || 
                   acc.accountType() == AccountType::LOAN;
        }));
    
    // Force layout updates to fix scroll area sizing
    scrollContent_->updateGeometry();
    scrollContent_->adjustSize();
    scrollArea_->updateGeometry();
}

void DashboardPage::onAccountCardClicked() {
    QPushButton* clickedCard = qobject_cast<QPushButton*>(sender());
    if (!clickedCard) {
        return;
    }
    
    QString accountNumber = clickedCard->property("accountNumber").toString();
    
    // Find the account
    for (const Account& account : accounts_) {
        if (QString::fromStdString(account.accountNumber()) == accountNumber) {
            if (onAccountClick_) {
                onAccountClick_(account);
            }
            break;
        }
    }
}

void DashboardPage::onShow() {
    // Update welcome message
    if (currentUser_) {
        // Refresh accounts display
        refreshAccountsDisplay();
    }
}