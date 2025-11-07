// Brandon Cotesta || 10/28/2025 | 3:00 PM
// User page for displaying user account information

#include "MoveMoneyPage.h"
#include "DatabaseManager.h"
#include "Authenticator.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMessageBox>
#include <iomanip>
#include <sstream>
#include <QDateTime>
#include <algorithm>

MoveMoneyPage::MoveMoneyPage()
    : Page("move_money"),
    stackedWidget_(nullptr),
    mainMenuWidget_(nullptr),
    containerWidget_(nullptr),
    currentUser_(nullptr),
    eTransferOverlay_(nullptr),
    transferFundsOverlay_(nullptr),
    miniStatementOverlay_(nullptr),
    eDepositOverlay_(nullptr),
    eTransferAccountSelect_(nullptr),
    eTransferRecipientInput_(nullptr),
    eTransferAmountInput_(nullptr),
    eTransferMessageInput_(nullptr),
    transferFromAccount_(nullptr),
    transferToAccount_(nullptr),
    transferAmountInput_(nullptr),
    titleLabel_(nullptr),
    miniStatementAccountSelect_(nullptr),
    miniStatementTable_(nullptr),
    generateStatementButton_(nullptr)
{
}

MoveMoneyPage::~MoveMoneyPage() {
    // Qt's parent-child system handles cleanup
}

void MoveMoneyPage::setUser(User* user) {
    currentUser_ = user;
}

void MoveMoneyPage::setTransactionCompleteCallback(std::function<void()> callback) {
    onTransactionComplete_ = callback;
}

void MoveMoneyPage::notifyTransactionComplete() {
    if (onTransactionComplete_) {
        onTransactionComplete_();
    }
}

QString MoveMoneyPage::accountTypeToString(AccountType type) const {
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

QString MoveMoneyPage::formatAccountDisplay(const Account& account) const {
    return QString("%1 - %2 - $%3")
        .arg(accountTypeToString(account.accountType()))
        .arg(QString::fromStdString(account.accountNumber()))
        .arg(account.getBalance(), 0, 'f', 2);
}

void MoveMoneyPage::loadUserAccounts() {
    if (!currentUser_) {
        cout << "[MoveMoneyPage] ERROR: No current user set" << endl;
        return;
    }
    
    cout << "[MoveMoneyPage] Loading accounts for user: " << currentUser_->email() << endl;
    
    // Use AccountManager to load accounts from database
    AccountManager accountMgr(*currentUser_);
    accounts_ = accountMgr.loadUserAccounts();
    
    cout << "[MoveMoneyPage] Loaded " << accounts_.size() << " accounts" << endl;
    
    // Clear and populate account dropdowns with enhanced format
    if (eTransferAccountSelect_) {
        eTransferAccountSelect_->clear();
        for (const auto& acc : accounts_) {
            eTransferAccountSelect_->addItem(formatAccountDisplay(acc));
        }
    }
    
    if (transferFromAccount_ && transferToAccount_) {
        transferFromAccount_->clear();
        transferToAccount_->clear();
        for (const auto& acc : accounts_) {
            QString displayText = formatAccountDisplay(acc);
            transferFromAccount_->addItem(displayText);
            transferToAccount_->addItem(displayText);
        }
    }
    
    if (miniStatementAccountSelect_) {
        miniStatementAccountSelect_->clear();
        for (const auto& acc : accounts_) {
            miniStatementAccountSelect_->addItem(formatAccountDisplay(acc));
        }
    }
}

void MoveMoneyPage::buildUI() {
    QWidget* centralWidget = getCentralWidget();

    // Set background color for the page
    centralWidget->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    // Create stacked widget to manage main menu and overlays
    stackedWidget_ = new QStackedWidget(centralWidget);
    
    // Add stacked widget to the existing mainLayout_ from Page base class
    mainLayout_->addWidget(stackedWidget_);

    // Create main menu widget
    mainMenuWidget_ = new QWidget();
    QVBoxLayout* mainMenuLayout = new QVBoxLayout(mainMenuWidget_);
    mainMenuLayout->setContentsMargins(0, 0, 0, 0);
    mainMenuLayout->setSpacing(0);

    // Create scroll area for main menu content
    QScrollArea* scrollArea = new QScrollArea(mainMenuWidget_);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: #f5f5f5; border: none; }");

    // Create scroll content widget
    QWidget* scrollContent = new QWidget();
    scrollContent->setStyleSheet("QWidget { background-color: #f5f5f5; }");
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setSpacing(0);
    scrollLayout->setContentsMargins(0, 0, 0, 20);

    // Header with white background
    QWidget* headerWidget = new QWidget(scrollContent);
    headerWidget->setStyleSheet("QWidget { background-color: white; }");
    headerWidget->setFixedHeight(80);

    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(20, 20, 20, 20);

    titleLabel_ = new QLabel("Move money", headerWidget);
    QFont titleFont("Segoe UI", 24, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    headerLayout->addWidget(titleLabel_);
    headerLayout->addStretch();

    scrollLayout->addWidget(headerWidget);
    scrollLayout->addSpacing(20);

    // Everyday Actions Section
    QWidget* sectionWidget = new QWidget(scrollContent);
    sectionWidget->setStyleSheet("QWidget { background-color: transparent; }");
    QVBoxLayout* sectionLayout = new QVBoxLayout(sectionWidget);
    sectionLayout->setContentsMargins(15, 0, 15, 0);
    sectionLayout->setSpacing(15);

    // Section title
    QLabel* sectionTitle = new QLabel("EVERYDAY ACTIONS", sectionWidget);
    QFont sectionFont("Segoe UI", 11, QFont::Bold);
    sectionTitle->setFont(sectionFont);
    sectionTitle->setStyleSheet("QLabel { color: #7f8c8d; padding: 10px 5px; }");
    sectionLayout->addWidget(sectionTitle);

    // Grid layout for action cards (2 columns)
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(15);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Create action cards
    QPushButton* eTransferCard = createActionCard(
        "eTransfer",
        "img/128x/data-transfer.png",
        sectionWidget
    );
    connect(eTransferCard, &QPushButton::clicked, this, &MoveMoneyPage::onETransferClicked);
    
    QPushButton* transferFundsCard = createActionCard(
        "Transfer funds",
        "img/128x/data-transfer.png",
        sectionWidget
    );
    connect(transferFundsCard, &QPushButton::clicked, this, &MoveMoneyPage::onTransferFundsClicked);
    
    QPushButton* miniStatementCard = createActionCard(
        "Mini Statement",
        "img/128x/transactionIcon.png",
        sectionWidget
    );
    connect(miniStatementCard, &QPushButton::clicked, this, &MoveMoneyPage::onMiniStatementClicked);
    
    QPushButton* eDepositCard = createActionCard(
        "eDeposit",
        "img/128x/homeIcon.png",
        sectionWidget
    );
    connect(eDepositCard, &QPushButton::clicked, this, &MoveMoneyPage::onEDepositClicked);

    // Add cards to grid (2x2)
    gridLayout->addWidget(eTransferCard, 0, 0);
    gridLayout->addWidget(transferFundsCard, 0, 1);
    gridLayout->addWidget(miniStatementCard, 1, 0);
    gridLayout->addWidget(eDepositCard, 1, 1);

    sectionLayout->addLayout(gridLayout);
    sectionLayout->addStretch();

    scrollLayout->addWidget(sectionWidget);
    scrollLayout->addStretch();

    scrollArea->setWidget(scrollContent);
    mainMenuLayout->addWidget(scrollArea);

    // Add main menu to stacked widget
    stackedWidget_->addWidget(mainMenuWidget_);

    // Create and add overlays
    eTransferOverlay_ = createETransferOverlay();
    transferFundsOverlay_ = createTransferFundsOverlay();
    miniStatementOverlay_ = createMiniStatementOverlay();
    eDepositOverlay_ = createEDepositOverlay();

    stackedWidget_->addWidget(eTransferOverlay_);
    stackedWidget_->addWidget(transferFundsOverlay_);
    stackedWidget_->addWidget(miniStatementOverlay_);
    stackedWidget_->addWidget(eDepositOverlay_);

    // Show main menu by default
    stackedWidget_->setCurrentIndex(MAIN_MENU);
}

QWidget* MoveMoneyPage::createEDepositOverlay() {
    QWidget* overlay = new QWidget(this);
    return overlay;
}

QPushButton* MoveMoneyPage::createActionCard(const QString& title, const QString& iconPath, QWidget* parent) {
    QPushButton* card = new QPushButton(parent);
    card->setCursor(Qt::PointingHandCursor);
    card->setFixedHeight(160);
    
    // Fix hover issue: set attribute to enable proper mouse tracking
    card->setAttribute(Qt::WA_Hover, true);

    // Card styling with green outline and proper hover effect
    QString cardStyle =
        "QPushButton {"
        "   background-color: #f0f0f0;"
        "   border: 1px solid #00cc00;"
        "   border-radius: 12px;"
        "   text-align: left;"
        "   padding: 0px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e8e8e8;"
        "   border: 2px solid #00cc00;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #d8d8d8;"
        "   border: 2px solid #008f39;"
        "}";
    card->setStyleSheet(cardStyle);

    // Create content widget
    QWidget* contentWidget = new QWidget(card);
    contentWidget->setStyleSheet("QWidget { background-color: transparent; border: none; }");
    contentWidget->setAttribute(Qt::WA_TransparentForMouseEvents);

    QVBoxLayout* cardLayout = new QVBoxLayout(contentWidget);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(0);

    // Icon circle container
    QWidget* iconCircle = new QWidget(contentWidget);
    iconCircle->setFixedSize(64, 64);
    iconCircle->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border: none;"
        "   border-radius: 32px;"
        "}"
    );
    iconCircle->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Icon label inside circle
    QLabel* iconLabel = new QLabel(iconCircle);
    QIcon icon(iconPath);
    QPixmap pixmap = icon.pixmap(32, 32);
    iconLabel->setPixmap(pixmap);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setGeometry(16, 16, 32, 32);
    iconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    cardLayout->addWidget(iconCircle);
    cardLayout->addSpacing(15);

    // Title label
    QLabel* titleLabel = new QLabel(title, contentWidget);
    QFont titleFont("Segoe UI", 14, QFont::Normal);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("QLabel { color: #2c3e50; border: none; }");
    titleLabel->setWordWrap(true);
    titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    cardLayout->addWidget(titleLabel);

    cardLayout->addStretch();

    // Position content widget in button
    QVBoxLayout* buttonLayout = new QVBoxLayout(card);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addWidget(contentWidget);

    return card;
}

QWidget* MoveMoneyPage::createOverlayHeader(const QString& title, QWidget* parent) {
    QWidget* header = new QWidget(parent);
    header->setStyleSheet("QWidget { background-color: white; }");
    header->setFixedHeight(80);

    QHBoxLayout* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(10, 20, 20, 20);

    // Back button
    QPushButton* backButton = new QPushButton(header);
    backButton->setText("<");  // Simple text back arrow
    backButton->setFixedSize(40, 40);
    backButton->setCursor(Qt::PointingHandCursor);
    QFont backFont("Segoe UI", 18, QFont::Bold);
    backButton->setFont(backFont);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   border: none;"
        "   border-radius: 20px;"
        "   color: #00cc00;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f0f0f0;"
        "}"
    );
    connect(backButton, &QPushButton::clicked, this, &MoveMoneyPage::hideOverlay);

    QLabel* titleLabel = new QLabel(title, header);
    QFont titleFont("Segoe UI", 24, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("QLabel { color: #2c3e50; }");

    headerLayout->addWidget(backButton);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    return header;
}

QWidget* MoveMoneyPage::createETransferOverlay() {
    QWidget* overlay = new QWidget();
    overlay->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    QVBoxLayout* layout = new QVBoxLayout(overlay);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Header
    QWidget* header = createOverlayHeader("eTransfer", overlay);
    layout->addWidget(header);

    // Content area with scroll
    QScrollArea* scrollArea = new QScrollArea(overlay);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: transparent; border: none; }");

    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 10, 20, 20);
    contentLayout->setSpacing(15);

    // Input styling
    QString inputStyle =
        "QLineEdit, QComboBox {"
        "   padding: 12px 15px;"
        "   border: 2px solid #e0e0e0;"
        "   border-radius: 8px;"
        "   font-size: 14px;"
        "   color: #000000;"
        "   background-color: white;"
        "   min-height: 48px;"
        "}"
        "QLineEdit:focus, QComboBox:focus {"
        "   border: 2px solid #00cc00;"
        "}";

    // From Account
    QLabel* accountLabel = new QLabel("From Account", content);
    accountLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; }");
    contentLayout->addWidget(accountLabel);

    eTransferAccountSelect_ = new QComboBox(content);
    eTransferAccountSelect_->setStyleSheet(inputStyle);
    contentLayout->addWidget(eTransferAccountSelect_);

    // Recipient email
    QLabel* recipientLabel = new QLabel("Recipient Email", content);
    recipientLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; }");
    contentLayout->addWidget(recipientLabel);

    eTransferRecipientInput_ = new QLineEdit(content);
    eTransferRecipientInput_->setPlaceholderText("Enter recipient's email");
    eTransferRecipientInput_->setStyleSheet(inputStyle);
    contentLayout->addWidget(eTransferRecipientInput_);

    // Amount
    QLabel* amountLabel = new QLabel("Amount", content);
    amountLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; }");
    contentLayout->addWidget(amountLabel);

    eTransferAmountInput_ = new QLineEdit(content);
    eTransferAmountInput_->setPlaceholderText("$0.00");
    eTransferAmountInput_->setStyleSheet(inputStyle);
    contentLayout->addWidget(eTransferAmountInput_);

    // Message
    QLabel* messageLabel = new QLabel("Message (Optional)", content);
    messageLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; }");
    contentLayout->addWidget(messageLabel);

    eTransferMessageInput_ = new QLineEdit(content);
    eTransferMessageInput_->setPlaceholderText("Add a message");
    eTransferMessageInput_->setStyleSheet(inputStyle);
    contentLayout->addWidget(eTransferMessageInput_);

    contentLayout->addStretch();

    // Send button
    QPushButton* sendButton = new QPushButton("Send eTransfer", content);
    sendButton->setMinimumHeight(50);
    sendButton->setCursor(Qt::PointingHandCursor);
    sendButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #00cc00;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #02a802;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #008f39;"
        "}"
    );
    connect(sendButton, &QPushButton::clicked, this, &MoveMoneyPage::handleETransferSend);
    contentLayout->addWidget(sendButton);

    scrollArea->setWidget(content);
    layout->addWidget(scrollArea);

    return overlay;
}

QWidget* MoveMoneyPage::createTransferFundsOverlay() {
    QWidget* overlay = new QWidget();
    overlay->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    QVBoxLayout* layout = new QVBoxLayout(overlay);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Header
    QWidget* header = createOverlayHeader("Transfer Funds", overlay);
    layout->addWidget(header);

    // Content area
    QScrollArea* scrollArea = new QScrollArea(overlay);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: transparent; border: none; }");

    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 10, 20, 20);
    contentLayout->setSpacing(15);

    QString inputStyle =
        "QLineEdit, QComboBox {"
        "   padding: 12px 15px;"
        "   border: 2px solid #e0e0e0;"
        "   border-radius: 8px;"
        "   font-size: 14px;"
        "   color: #000000;"
        "   background-color: white;"
        "   min-height: 48px;"
        "}"
        "QLineEdit:focus, QComboBox:focus {"
        "   border: 2px solid #00cc00;"
        "}";

    // From account
    QLabel* fromLabel = new QLabel("From Account", content);
    fromLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; }");
    contentLayout->addWidget(fromLabel);

    transferFromAccount_ = new QComboBox(content);
    transferFromAccount_->setStyleSheet(inputStyle);
    contentLayout->addWidget(transferFromAccount_);

    // To account
    QLabel* toLabel = new QLabel("To Account", content);
    toLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; }");
    contentLayout->addWidget(toLabel);

    transferToAccount_ = new QComboBox(content);
    transferToAccount_->setStyleSheet(inputStyle);
    contentLayout->addWidget(transferToAccount_);

    // Amount
    QLabel* amountLabel = new QLabel("Amount", content);
    amountLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; }");
    contentLayout->addWidget(amountLabel);

    transferAmountInput_ = new QLineEdit(content);
    transferAmountInput_->setPlaceholderText("$0.00");
    transferAmountInput_->setStyleSheet(inputStyle);
    contentLayout->addWidget(transferAmountInput_);

    contentLayout->addStretch();

    // Transfer button
    QPushButton* transferButton = new QPushButton("Transfer", content);
    transferButton->setMinimumHeight(50);
    transferButton->setCursor(Qt::PointingHandCursor);
    transferButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #00cc00;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #02a802;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #008f39;"
        "}"
    );
    connect(transferButton, &QPushButton::clicked, this, &MoveMoneyPage::handleTransfer);
    contentLayout->addWidget(transferButton);

    scrollArea->setWidget(content);
    layout->addWidget(scrollArea);

    return overlay;
}

QWidget* MoveMoneyPage::createMiniStatementOverlay() {
    QWidget* overlay = new QWidget();
    overlay->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    QVBoxLayout* layout = new QVBoxLayout(overlay);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Header
    QWidget* header = createOverlayHeader("Mini Statement", overlay);
    layout->addWidget(header);

    // Content
    QScrollArea* scrollArea = new QScrollArea(overlay);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: transparent; border: none; }");

    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 10, 20, 20);
    contentLayout->setSpacing(15);

    // Account selector
    QLabel* accountLabel = new QLabel("Select Account", content);
    accountLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; }");
    contentLayout->addWidget(accountLabel);

    miniStatementAccountSelect_ = new QComboBox(content);
    miniStatementAccountSelect_->setStyleSheet(
        "QComboBox {"
        "   padding: 12px 15px;"
        "   border: 2px solid #e0e0e0;"
        "   border-radius: 8px;"
        "   font-size: 14px;"
        "   color: #000000;"
        "   background-color: white;"
        "   min-height: 48px;"
        "}"
    );
    contentLayout->addWidget(miniStatementAccountSelect_);

    // Generate button
    generateStatementButton_ = new QPushButton("Generate Statement", content);
    generateStatementButton_->setMinimumHeight(50);
    generateStatementButton_->setCursor(Qt::PointingHandCursor);
    generateStatementButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #00cc00;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #02a802;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #008f39;"
        "}"
    );
    connect(generateStatementButton_, &QPushButton::clicked, this, &MoveMoneyPage::handleGenerateMiniStatement);
    contentLayout->addWidget(generateStatementButton_);

    // Recent transactions label
    QLabel* transactionsLabel = new QLabel("Recent Transactions", content);
    QFont labelFont("Segoe UI", 14, QFont::Bold);
    transactionsLabel->setFont(labelFont);
    transactionsLabel->setStyleSheet("QLabel { color: #2c3e50; margin-top: 10px; }");
    contentLayout->addWidget(transactionsLabel);

    // Transaction table
    miniStatementTable_ = new QTableWidget(content);
    miniStatementTable_->setStyleSheet(
        "QTableWidget {"
        "   background-color: white;"
        "   border: 1px solid #e0e0e0;"
        "   border-radius: 8px;"
        "   gridline-color: #f0f0f0;"
        "}"
        "QTableWidget::item {"
        "   padding: 8px;"
        "   border-bottom: 1px solid #f0f0f0;"
        "   color: #000000;"  // Black text for all items
        "}"
        "QTableWidget::item:alternate {"
        "   background-color: #e8f5e9;"  // Light green for alternate rows
        "   color: #000000;"  // Ensure black text on green rows
        "}"
        "QHeaderView::section {"
        "   background-color: #00cc00;"  // Green header background
        "   padding: 10px;"
        "   border: none;"
        "   border-bottom: 2px solid #00a000;"
        "   font-weight: bold;"
        "   color: #ffffff;"  // White text for header
        "}"
    );
    miniStatementTable_->setColumnCount(5);
    miniStatementTable_->setHorizontalHeaderLabels({"Type", "Amount", "Date", "Description", "Balance"});
    miniStatementTable_->horizontalHeader()->setStretchLastSection(false);
    miniStatementTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    miniStatementTable_->verticalHeader()->setVisible(false);
    miniStatementTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    miniStatementTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    miniStatementTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    miniStatementTable_->setAlternatingRowColors(true);
    miniStatementTable_->setMinimumHeight(300);
    contentLayout->addWidget(miniStatementTable_);

    contentLayout->addStretch();

    scrollArea->setWidget(content);
    layout->addWidget(scrollArea);

    return overlay;
}

void MoveMoneyPage::handleETransferSend() {
    cout << "[MoveMoneyPage] eTransfer send initiated" << endl;
    
    if (!currentUser_ || accounts_.empty()) {
        QMessageBox::warning(nullptr, "Error", "No accounts available.");
        return;
    }
    
    // Get selected account index
    int accountIndex = eTransferAccountSelect_->currentIndex();
    if (accountIndex < 0 || accountIndex >= static_cast<int>(accounts_.size())) {
        QMessageBox::warning(nullptr, "Error", "Please select an account.");
        return;
    }
    
    // Validate recipient email
    QString recipient = eTransferRecipientInput_->text().trimmed();
    if (recipient.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Please enter recipient email.");
        return;
    }
    
    // Parse and validate amount
    QString amountText = eTransferAmountInput_->text().trimmed();
    amountText.replace("$", "");  // Remove $ if present
    bool ok;
    double amount = amountText.toDouble(&ok);
    
    if (!ok || amount <= 0) {
        QMessageBox::warning(nullptr, "Error", "Please enter a valid amount.");
        return;
    }
    
    // Get the account reference
    Account& account = accounts_[accountIndex];
    
    cout << "[MoveMoneyPage] Account selected: " << account.accountNumber() 
         << " | Current balance: $" << account.getBalance() << endl;
    
    // Check sufficient funds
    if (amount > account.getBalance()) {
        QMessageBox::warning(nullptr, "Insufficient Funds", 
            QString("Insufficient funds. Current balance: $%1").arg(account.getBalance(), 0, 'f', 2));
        return;
    }
    
    // Create AccountManager with current user context
    AccountManager accountMgr(*currentUser_);
    
    cout << "[MoveMoneyPage] Initiating withdrawal of $" << amount 
         << " from account " << account.accountNumber() << endl;
    
    // Perform withdrawal (eTransfer acts as withdrawal)
    // AccountManager::withdraw() handles:
    // - Balance update on Account object
    // - Database sync via syncAccountToDatabase()
    // - Transaction recording via recordTransaction()
    if (accountMgr.withdraw(account, amount)) {
        cout << "[MoveMoneyPage] Withdrawal successful! New balance: $" 
             << account.getBalance() << endl;
        
        // Update the local accounts vector (account reference is already updated)
        accounts_[accountIndex] = account;
        
        // Reload accounts to refresh UI with updated balances
        loadUserAccounts();
        
        // Clear inputs
        eTransferRecipientInput_->clear();
        eTransferAmountInput_->clear();
        eTransferMessageInput_->clear();
        
        // Show success message
        QString message = eTransferMessageInput_->text().trimmed();
        QMessageBox::information(nullptr, "Success", 
            QString("eTransfer of $%1 sent to %2 successfully!\nNew balance: $%3")
            .arg(amount, 0, 'f', 2)
            .arg(recipient)
            .arg(account.getBalance(), 0, 'f', 2));
        
        // Notify that transaction was completed (refresh dashboard)
        notifyTransactionComplete();
        
        // Return to main menu
        hideOverlay();
    } else {
        cout << "[MoveMoneyPage] ERROR: Withdrawal failed" << endl;
        QMessageBox::critical(nullptr, "Error", "eTransfer failed. Please try again.");
    }
}

void MoveMoneyPage::handleTransfer() {
    cout << "[MoveMoneyPage] Transfer initiated" << endl;
    
    if (!currentUser_ || accounts_.size() < 2) {
        QMessageBox::warning(nullptr, "Error", "You need at least 2 accounts to transfer funds.");
        return;
    }
    
    // Get selected account indices
    int fromIndex = transferFromAccount_->currentIndex();
    int toIndex = transferToAccount_->currentIndex();
    
    if (fromIndex < 0 || fromIndex >= static_cast<int>(accounts_.size()) ||
        toIndex < 0 || toIndex >= static_cast<int>(accounts_.size())) {
        QMessageBox::warning(nullptr, "Error", "Please select valid accounts.");
        return;
    }
    
    if (fromIndex == toIndex) {
        QMessageBox::warning(nullptr, "Error", "Cannot transfer to the same account.");
        return;
    }
    
    // Parse and validate amount
    QString amountText = transferAmountInput_->text().trimmed();
    amountText.replace("$", "");
    bool ok;
    double amount = amountText.toDouble(&ok);
    
    if (!ok || amount <= 0) {
        QMessageBox::warning(nullptr, "Error", "Please enter a valid amount.");
        return;
    }
    
    // Get the account references
    Account& fromAccount = accounts_[fromIndex];
    Account& toAccount = accounts_[toIndex];
    
    cout << "[MoveMoneyPage] Transfer from " << fromAccount.accountNumber() 
         << " ($" << fromAccount.getBalance() << ") to " 
         << toAccount.accountNumber() << " ($" << toAccount.getBalance() << ")" << endl;
    
    // Check sufficient funds
    if (amount > fromAccount.getBalance()) {
        QMessageBox::warning(nullptr, "Insufficient Funds", 
            QString("Insufficient funds in source account. Current balance: $%1")
            .arg(fromAccount.getBalance(), 0, 'f', 2));
        return;
    }
    
    // Create AccountManager with current user context
    AccountManager accountMgr(*currentUser_);
    
    cout << "[MoveMoneyPage] Initiating transfer of $" << amount << endl;
    
    // Perform transfer
    // AccountManager::transfer() handles:
    // - Withdrawal from source account
    // - Deposit to destination account
    // - Database sync for both accounts
    // - Transaction recording for both accounts
    if (accountMgr.transfer(fromAccount, toAccount, amount)) {
        cout << "[MoveMoneyPage] Transfer successful!" << endl;
        cout << "  From account new balance: $" << fromAccount.getBalance() << endl;
        cout << "  To account new balance: $" << toAccount.getBalance() << endl;
        
        // Update the local accounts vector (references are already updated)
        accounts_[fromIndex] = fromAccount;
        accounts_[toIndex] = toAccount;
        
        // Reload accounts to refresh UI with updated balances
        loadUserAccounts();
        
        // Clear input
        transferAmountInput_->clear();
        
        // Show success message
        QMessageBox::information(nullptr, "Success", 
            QString("Transfer of $%1 completed successfully!\n\nFrom: %2 (New balance: $%3)\nTo: %4 (New balance: $%5)")
            .arg(amount, 0, 'f', 2)
            .arg(QString::fromStdString(fromAccount.accountNumber()))
            .arg(fromAccount.getBalance(), 0, 'f', 2)
            .arg(QString::fromStdString(toAccount.accountNumber()))
            .arg(toAccount.getBalance(), 0, 'f', 2));
        
        // Notify that transaction was completed (refresh dashboard)
        notifyTransactionComplete();
        
        // Return to main menu
        hideOverlay();
    } else {
        cout << "[MoveMoneyPage] ERROR: Transfer failed" << endl;
        QMessageBox::critical(nullptr, "Error", "Transfer failed. Please try again.");
    }
}

void MoveMoneyPage::showOverlay(int overlayIndex) {
    stackedWidget_->setCurrentIndex(overlayIndex);
}

void MoveMoneyPage::hideOverlay() {
    stackedWidget_->setCurrentIndex(MAIN_MENU);
}

void MoveMoneyPage::onETransferClicked() {
    loadUserAccounts();  // Refresh accounts before showing overlay
    showOverlay(ETRANSFER);
}

void MoveMoneyPage::onTransferFundsClicked() {
    loadUserAccounts();  // Refresh accounts before showing overlay
    showOverlay(TRANSFER_FUNDS);
}

void MoveMoneyPage::onMiniStatementClicked() {
    loadUserAccounts();  // Refresh accounts before showing overlay
    miniStatementTable_->setRowCount(0);  // Clear previous data
    showOverlay(MINI_STATEMENT);
}

void MoveMoneyPage::onEDepositClicked() {
    showOverlay(EDEPOSIT);
}

void MoveMoneyPage::onShow() {
    cout << "MoveMoneyPage::onShow called" << endl;
    
    // Load user accounts when page is shown
    loadUserAccounts();
    
    // Always return to main menu when page is shown
    if (stackedWidget_) {
        stackedWidget_->setCurrentIndex(MAIN_MENU);
    }
    
    // Force layout update to fix initial load issue
    QWidget* centralWidget = getCentralWidget();
    if (centralWidget) {
        centralWidget->updateGeometry();
        centralWidget->update();
    }
}

// Add the mini statement generation handler
void MoveMoneyPage::handleGenerateMiniStatement() {
    cout << "[MoveMoneyPage] Generating mini statement" << endl;
    
    if (!currentUser_ || accounts_.empty()) {
        QMessageBox::warning(nullptr, "Error", "No accounts available.");
        return;
    }
    
    // Get selected account index
    int accountIndex = miniStatementAccountSelect_->currentIndex();
    if (accountIndex < 0 || accountIndex >= static_cast<int>(accounts_.size())) {
        QMessageBox::warning(nullptr, "Error", "Please select an account.");
        return;
    }
    
    const Account& account = accounts_[accountIndex];
    
    cout << "[MoveMoneyPage] Generating statement for account: " << account.accountNumber() << endl;
    
    // Get transactions from database
    DatabaseManager& db = DatabaseManager::getInstance();
    AccountManager accountMgr(*currentUser_);
    
    // Get the transactions table name for this user
    std::string transTableName = accountMgr.getTransactionsTableName();
    
    // Calculate date 4 weeks ago
    QDateTime currentDate = QDateTime::currentDateTime();
    QDateTime fourWeeksAgo = currentDate.addDays(-28);  // 4 weeks = 28 days
    QString fourWeeksAgoStr = fourWeeksAgo.toString("yyyy-MM-dd HH:mm:ss");
    
    cout << "[MoveMoneyPage] Filtering transactions from: " << fourWeeksAgoStr.toStdString() << endl;
    
    // Retrieve transactions for this specific account within the last 4 weeks
    std::string whereClause = "accountNumber = '" + account.accountNumber() + 
                              "' AND transactionDate >= '" + fourWeeksAgoStr.toStdString() + "'";
    auto transactions = db.retrieveTable(transTableName, whereClause);
    
    cout << "[MoveMoneyPage] Found " << transactions.size() << " transactions in last 4 weeks" << endl;
    
    // Clear existing table data
    miniStatementTable_->setRowCount(0);
    
    if (transactions.empty()) {
        QMessageBox::information(nullptr, "No Transactions", 
            "No transactions found for this account in the last 4 weeks.");
        return;
    }
    
    // Sort transactions by date (newest first)
    std::sort(transactions.begin(), transactions.end(), 
        [](const std::map<std::string, sql::SQLString>& a, const std::map<std::string, sql::SQLString>& b) {
            std::string dateA = static_cast<std::string>(a.at("transactionDate"));
            std::string dateB = static_cast<std::string>(b.at("transactionDate"));
            return dateA > dateB;  // Descending order (newest first)
        });
    
    // Populate table with transaction data
    miniStatementTable_->setRowCount(static_cast<int>(transactions.size()));
    
    int row = 0;
    for (const auto& trans : transactions) {
        // Transaction type
        std::string transType = static_cast<std::string>(trans.at("transactionType"));
        miniStatementTable_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(transType)));
        
        // Amount
        std::string amount = static_cast<std::string>(trans.at("amount"));
        miniStatementTable_->setItem(row, 1, new QTableWidgetItem("$" + QString::fromStdString(amount)));
        
        // Date - format for better readability
        std::string dateStr = static_cast<std::string>(trans.at("transactionDate"));
        QDateTime transDate = QDateTime::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd HH:mm:ss");
        QString formattedDate = transDate.toString("MMM dd, yyyy hh:mm AP");
        miniStatementTable_->setItem(row, 2, new QTableWidgetItem(formattedDate));
        
        // Description
        std::string description = static_cast<std::string>(trans.at("description"));
        miniStatementTable_->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(description)));
        
        // Balance after transaction
        std::string balanceAfter = static_cast<std::string>(trans.at("balanceAfter"));
        miniStatementTable_->setItem(row, 4, new QTableWidgetItem("$" + QString::fromStdString(balanceAfter)));
        
        row++;
    }
    
    // Resize columns to content
    miniStatementTable_->resizeColumnsToContents();
}