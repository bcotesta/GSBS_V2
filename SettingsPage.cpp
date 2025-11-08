// Brandon Cotesta || 10/28/2025 | 3:00 PM
// Sahara Rahimani || 11/04/2025 | 8:00 AM
// Dashboard page for displaying user and account information

#include "SettingsPage.h" 
#include "DatabaseManager.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QScrollArea>
#include <QtGui/QPixmap>
#include <QtGui/QIcon>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include <iomanip>
#include <sstream>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtPrintSupport/QPrinter>
#include <QtGui/QPainter>
#include <QtGui/QTextDocument>
#include <QDateTime>
#include <algorithm>

SettingsPage::SettingsPage()
    : Page("settings"),
    containerWidget_(nullptr),
    currentUser_(nullptr),
    stackedWidget_(nullptr),
    mainMenuWidget_(nullptr),
    titleLabel_(nullptr),
    ProfileOverlay_(nullptr),
    ProductsOverlay_(nullptr),
    AppOverlay_(nullptr),
    ContactOverlay_(nullptr),
    PrivacyOverlay_(nullptr),
    FAQOverlay_(nullptr),        // ADDED
    SignOutOverlay_(nullptr),
    productsButton_(nullptr),
    learnAppButton_(nullptr),
    contactButton_(nullptr),
    privacyButton_(nullptr),
    faqButton_(nullptr),         // ADDED
    signOutButton_(nullptr),
    pageManager_(nullptr),
    changeLabel(nullptr),
    fieldSelect_(nullptr),
    fieldInput_(nullptr)
{
}

SettingsPage::~SettingsPage() {}

void SettingsPage::setUser(User* user) {
    currentUser_ = user;
}

QWidget* SettingsPage::createOverlayHeader(const QString& title, QWidget* parent) {
    QWidget* header = new QWidget(parent);
    header->setStyleSheet("QWidget { background-color: white; }");
    header->setFixedHeight(80);

    QHBoxLayout* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(10, 20, 20, 20);

    QPushButton* backButton = new QPushButton(header);
    backButton->setText("<");
    backButton->setFixedSize(40, 40);
    backButton->setCursor(Qt::PointingHandCursor);
    QFont backFont("Segoe UI", 18, QFont::Bold);
    backButton->setFont(backFont);
    backButton->setStyleSheet(
        "QPushButton { background-color: transparent; border: none; border-radius: 20px; color: #00cc00; }"
        "QPushButton:hover { background-color: #f0f0f0; }"
    );
    connect(backButton, &QPushButton::clicked, this, &SettingsPage::hideOverlay);

    QLabel* titleLabel = new QLabel(title, header);
    QFont titleFont("Segoe UI", 24, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("QLabel { color: #2c3e50; }");

    headerLayout->addWidget(backButton);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    return header;
}

void SettingsPage::hideOverlay() {
    stackedWidget_->setCurrentIndex(MAIN_MENU);
}

QPushButton* SettingsPage::createActionCard(const QString& title, const QString& iconPath, QWidget* parent) {
    QPushButton* card = new QPushButton(parent);
    card->setCursor(Qt::PointingHandCursor);
    card->setFixedHeight(160);
    card->setAttribute(Qt::WA_Hover, true);

    QString cardStyle =
        "QPushButton { background-color: #f0f0f0; border: 1px solid #00cc00; border-radius: 12px; text-align: left; padding: 0px; }"
        "QPushButton:hover { background-color: #e8e8e8; border: 2px solid #00cc00; }"
        "QPushButton:pressed { background-color: #d8d8d8; border: 2px solid #008f39; }";
    card->setStyleSheet(cardStyle);

    QWidget* contentWidget = new QWidget(card);
    contentWidget->setStyleSheet("QWidget { background-color: transparent; border: none; }");
    contentWidget->setAttribute(Qt::WA_TransparentForMouseEvents);

    QVBoxLayout* cardLayout = new QVBoxLayout(contentWidget);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(0);

    QWidget* iconCircle = new QWidget(contentWidget);
    iconCircle->setFixedSize(64, 64);
    iconCircle->setStyleSheet("QWidget { background-color: white; border: none; border-radius: 32px; }");
    iconCircle->setAttribute(Qt::WA_TransparentForMouseEvents);

    QLabel* iconLabel = new QLabel(iconCircle);
    QIcon icon(iconPath);
    QPixmap pixmap = icon.pixmap(32, 32);
    iconLabel->setPixmap(pixmap);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setGeometry(16, 16, 32, 32);
    iconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    cardLayout->addWidget(iconCircle);
    cardLayout->addSpacing(15);

    QLabel* titleLabel = new QLabel(title, contentWidget);
    QFont titleFont("Segoe UI", 14, QFont::Normal);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("QLabel { color: #2c3e50; border: none; }");
    titleLabel->setWordWrap(true);
    titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    cardLayout->addWidget(titleLabel);
    cardLayout->addStretch();

    QVBoxLayout* buttonLayout = new QVBoxLayout(card);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addWidget(contentWidget);

    return card;
}

void SettingsPage::buildUI()
{
    QWidget* centralWidget = getCentralWidget();
    centralWidget->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    stackedWidget_ = new QStackedWidget(centralWidget);
    mainLayout_->addWidget(stackedWidget_);

    mainMenuWidget_ = new QWidget();
    QVBoxLayout* mainMenuLayout = new QVBoxLayout(mainMenuWidget_);

    containerWidget_ = new QWidget(mainMenuWidget_);
    containerWidget_->setFixedWidth(400);
    containerWidget_->setStyleSheet("QWidget { background-color: white; color: black; border-radius: 12px; }");

    QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget_);
    containerLayout->setSpacing(15);
    containerLayout->setContentsMargins(40, 35, 40, 35);

    QLabel* titleLabel = new QLabel("Settings", containerWidget_);
    QFont titleFont("Segoe UI", 28, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #2c3e50;");
    containerLayout->addWidget(titleLabel);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(containerWidget_);
    hLayout->addStretch();
    mainMenuLayout->addStretch();
    mainMenuLayout->addLayout(hLayout);
    mainMenuLayout->addStretch();

    QVBoxLayout* btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(12);

    auto createArrowButton = [&](const QString& text, const QString& iconPath) {
        QPushButton* btn = new QPushButton(containerWidget_);
        btn->setMinimumHeight(45);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QFont btnFont("Segoe UI", 14, QFont::DemiBold);

        btn->setStyleSheet(
            "QPushButton { background-color: white; border: 2px solid #00cc00; border-radius: 8px; padding: 0; }"
            "QPushButton:hover { background-color: #f4fff4; }"
            "QPushButton:pressed { background-color: #e8ffe8; }"
        );

        QWidget* content = new QWidget(btn);
        content->setStyleSheet("background: transparent;");
        QHBoxLayout* contentLayout = new QHBoxLayout(content);
        contentLayout->setContentsMargins(12, 0, 12, 0);
        contentLayout->setSpacing(8);

        if (!iconPath.isEmpty()) {
            QLabel* icon = new QLabel(content);
            QPixmap pix(iconPath);
            icon->setPixmap(pix.scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            icon->setFixedSize(28, 28);
            contentLayout->addWidget(icon);
        }

        QLabel* label = new QLabel(text, content);
        label->setFont(btnFont);
        label->setStyleSheet("color: #00cc00;");
        contentLayout->addWidget(label);

        QLabel* arrow = new QLabel(">", content);
        arrow->setFont(btnFont);
        arrow->setStyleSheet("color: #00cc00;");
        arrow->setFixedSize(22, 22);
        contentLayout->addWidget(arrow);

        QVBoxLayout* inner = new QVBoxLayout(btn);
        inner->setContentsMargins(4, 4, 4, 4);
        inner->addWidget(content);

        return btn;
        };

    auto createSignOutButton = [&]() {
        QPushButton* btn = new QPushButton("Sign Out", containerWidget_);
        btn->setMinimumHeight(45);
        btn->setStyleSheet(
            "QPushButton { background-color: #C73636; color: black; border-radius: 8px; }"
            "QPushButton:hover { background-color: #B12F2F; }"
            "QPushButton:pressed { background-color: #992222; }"
        );
        return btn;
        };

    QPushButton* profileBtn = createArrowButton("Profile", "img/128x/userIcon.png");
    QPushButton* productsBtn = createArrowButton("Products & Services", "img/128x/shopping-cart.png");
    QPushButton* appBtn = createArrowButton("Get to Know the App", "img/128x/star.png");
    QPushButton* contactBtn = createArrowButton("Contact Us", "img/128x/contact-mail.png");
    QPushButton* privacyBtn = createArrowButton("Privacy & Legal", "img/128x/information.png");
    QPushButton* faqBtn = createArrowButton("FAQ", "img/128x/question.png"); // ADDED
    QPushButton* signOutBtn = createSignOutButton();

    btnLayout->addWidget(profileBtn);
    btnLayout->addWidget(productsBtn);
    btnLayout->addWidget(appBtn);
    btnLayout->addWidget(contactBtn);
    btnLayout->addWidget(privacyBtn);
    btnLayout->addWidget(faqBtn); // ADDED
    btnLayout->addWidget(signOutBtn);
    containerLayout->addLayout(btnLayout);

    ProfileOverlay_ = createProfileOverlay();
    ProductsOverlay_ = createProductsOverlay();
    AppOverlay_ = createAppOverlay();
    ContactOverlay_ = createContactOverlay();
    PrivacyOverlay_ = createPrivacyOverlay();
    FAQOverlay_ = createFAQOverlay(); // ADDED
    SignOutOverlay_ = createSignOutPopUp();

    stackedWidget_->addWidget(mainMenuWidget_);
    stackedWidget_->addWidget(ProfileOverlay_);
    stackedWidget_->addWidget(ProductsOverlay_);
    stackedWidget_->addWidget(AppOverlay_);
    stackedWidget_->addWidget(ContactOverlay_);
    stackedWidget_->addWidget(PrivacyOverlay_);
    stackedWidget_->addWidget(FAQOverlay_); // ADDED
    stackedWidget_->addWidget(SignOutOverlay_);

    connect(profileBtn, &QPushButton::clicked, [this]() { stackedWidget_->setCurrentWidget(ProfileOverlay_); });
    connect(productsBtn, &QPushButton::clicked, [this]() { stackedWidget_->setCurrentWidget(ProductsOverlay_); });
    connect(appBtn, &QPushButton::clicked, [this]() { stackedWidget_->setCurrentWidget(AppOverlay_); });
    connect(contactBtn, &QPushButton::clicked, [this]() { stackedWidget_->setCurrentWidget(ContactOverlay_); });
    connect(privacyBtn, &QPushButton::clicked, [this]() { stackedWidget_->setCurrentWidget(PrivacyOverlay_); });
    connect(faqBtn, &QPushButton::clicked, [this]() { onFAQClicked(); }); // ADDED
    connect(signOutBtn, &QPushButton::clicked, [this]() { stackedWidget_->setCurrentWidget(SignOutOverlay_); });

    stackedWidget_->setCurrentWidget(mainMenuWidget_);
}

void SettingsPage::showOverlay(int overlayIndex) {
    stackedWidget_->setCurrentIndex(overlayIndex);
}

// --- PROFILE OVERLAY ---
QWidget* SettingsPage::createProfileOverlay() {
    QWidget* overlay = new QWidget();
    overlay->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    QVBoxLayout* layout = new QVBoxLayout(overlay);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget* header = createOverlayHeader("Profile", overlay);
    layout->addWidget(header);

    QScrollArea* scrollArea = new QScrollArea(overlay);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 0, 20, 16);
    contentLayout->setSpacing(10);

    QString inputStyle =
        "QLineEdit, QComboBox { padding: 10px 12px; border: 2px solid #e0e0e0; border-radius: 8px; font-size: 14px; color: #000000; background-color: white; min-height: 40px; }"
        "QLineEdit:focus, QComboBox:focus { border: 2px solid #00cc00; }";

    QLabel* changeLabel = new QLabel("Select field to change:", content);
    changeLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; font-size: 14px; }");
    contentLayout->addWidget(changeLabel);

    fieldSelect_ = new QComboBox(content);
    fieldSelect_->setStyleSheet(inputStyle);
    fieldSelect_->addItem("Name", QVariant("name"));
    fieldSelect_->addItem("Email", QVariant("email"));
    fieldSelect_->addItem("Phone Number", QVariant("phone"));
    fieldSelect_->addItem("Password", QVariant("password"));
    contentLayout->addWidget(fieldSelect_);

    fieldInput_ = new QLineEdit(content);
    fieldInput_->setPlaceholderText("Enter new value...");
    fieldInput_->setStyleSheet(inputStyle);
    contentLayout->addWidget(fieldInput_);

    QPushButton* changeButton = new QPushButton("Save Changes", content);
    changeButton->setMinimumHeight(44);
    changeButton->setCursor(Qt::PointingHandCursor);
    changeButton->setStyleSheet(
        "QPushButton { background-color: #00cc00; color: white; border: none; border-radius: 8px; font-size: 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #02a802; }"
        "QPushButton:pressed { background-color: #008f39; }"
    );
    connect(changeButton, &QPushButton::clicked, this, &SettingsPage::handleProfileSend);
    contentLayout->addWidget(changeButton);

    contentLayout->addStretch();

    scrollArea->setWidget(content);
    layout->addWidget(scrollArea);

    return overlay;
}
// --- FAQ OVERLAY (UPDATED WITH MORE QUESTIONS) ---
QWidget* SettingsPage::createFAQOverlay() {
    QWidget* overlay = new QWidget();
    overlay->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    QVBoxLayout* layout = new QVBoxLayout(overlay);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget* header = createOverlayHeader("FAQ", overlay);
    layout->addWidget(header);

    QScrollArea* scrollArea = new QScrollArea(overlay);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 10, 20, 20);
    contentLayout->setSpacing(18);

    auto addTitle = [&](const QString& text) {
        QLabel* label = new QLabel(text, content);
        label->setStyleSheet("font-size: 16px; font-weight: bold; color: black;");
        contentLayout->addWidget(label);
        };

    auto addText = [&](const QString& text) {
        QLabel* label = new QLabel(text, content);
        label->setWordWrap(true);
        label->setStyleSheet("font-size: 14px; color: black;");
        contentLayout->addWidget(label);
        };

    addTitle("How do I reset my password?");
    addText("Go to Settings > Profile and update your password securely.");

    addTitle("How do I transfer money?");
    addText("Use the Banking Operations page to send e-Transfers or move funds between your accounts.");

    addTitle("Where can I see recent transactions?");
    addText("Visit the Transactions page for a full list of your latest account activity.");

    addTitle("How do I deposit a cheque?");
    addText("Use the e-Deposit feature in Banking Operations and upload photos of your cheque.");

    addTitle("Is my information secure?");
    addText("Yes, your data is protected with industry-standard encryption and security systems.");

    addTitle("How do I open a new account?");
    addText("Navigate to Banking Operations and select 'Open New Account' to begin the process.");

    addTitle("Why was my e-Transfer delayed?");
    addText("Transfers may take a few minutes for verification. If it takes longer, contact support.");

    addTitle("Can I freeze my debit card if it's lost?");
    addText("Yes, go to Banking Operations and select 'Card Controls' to freeze or unfreeze your card.");

    addTitle("How do I download my mini-statement?");
    addText("On the Banking Operations page, select 'Mini-Statement' and choose download options.");

    addTitle("What should I do if I suspect fraud?");
    addText("Immediately freeze your card in the app and call customer support to report activity.");

    addTitle("Can I change my email or phone number?");
    addText("Yes, visit Settings > Profile and update your contact information.");

    addTitle("Why was my login attempt blocked?");
    addText("Multiple failed attempts will temporarily lock access for security. Try again later or use password recovery.");

    addTitle("Does the app support mobile wallets?");
    addText("Yes, our debit cards can be added to supported mobile wallet services for tap-to-pay.");

    addTitle("Can I set spending limits?");
    addText("Yes, visit Banking Operations and open Card Controls to adjust spending limits.");

    contentLayout->addStretch();
    scrollArea->setWidget(content);
    layout->addWidget(scrollArea);

    return overlay;
}


QWidget* SettingsPage::createProductsOverlay() {
    QWidget* overlay = new QWidget();
    overlay->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    QVBoxLayout* layout = new QVBoxLayout(overlay);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QWidget* header = createOverlayHeader("Products and Services", overlay);
    layout->addWidget(header);

    QScrollArea* scrollArea = new QScrollArea(overlay);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("QScrollArea { background-color: transparent; border: none; }");

    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 10, 20, 20);
    contentLayout->setSpacing(18);

    auto createTitle = [&](const QString& text) {
        QLabel* label = new QLabel(text, content);
        label->setStyleSheet("QLabel { color: #000; font-size: 20px; font-weight: 700; }");
        return label;
        };

    auto createBody = [&](const QString& text) {
        QLabel* label = new QLabel(text, content);
        label->setWordWrap(true);
        label->setStyleSheet("QLabel { color: #2c3e50; font-size: 14px; }");
        return label;
        };

    // ---------------- CHEQUING ----------------
    contentLayout->addWidget(createTitle("Chequing Accounts"));

    contentLayout->addWidget(createBody(
        "Our chequing accounts are designed for everyday financial activity, offering convenient access "
        "to your money for shopping, bill payments, and transfers. With support for direct deposits, "
        "automatic payments, and debit card transactions, you can manage all your day-to-day banking with ease."
    ));
    contentLayout->addWidget(createBody(
        "These accounts offer unlimited digital transactions, mobile cheque deposits, and seamless integration "
        "with Interac e-Transfer services. Whether you're paying friends or managing recurring bills, "
        "our chequing system keeps your finances running smoothly."
    ));
    contentLayout->addWidget(createBody(
        "With built-in fraud protection and secure authentication, we prioritize keeping your money safe. "
        "Real-time alerts and in-app monitoring tools provide complete visibility and confidence in every transaction."
    ));
    contentLayout->addWidget(createBody(
        "We also provide customizable account options tailored for students, newcomers, and professionals. "
        "Enjoy simplified banking that adapts to your lifestyle, helping you manage everyday spending efficiently."
    ));

    // ---------------- SAVINGS ----------------
    contentLayout->addWidget(createTitle("Savings Accounts"));

    contentLayout->addWidget(createBody(
        "Our savings accounts help you build your financial future while keeping your funds easily accessible. "
        "Earn interest on every dollar saved and take advantage of automated savings features designed to help "
        "you stay consistent and disciplined with your financial goals."
    ));
    contentLayout->addWidget(createBody(
        "Whether you're saving for travel, an emergency fund, or long-term financial milestones, our savings "
        "options offer flexibility and security, backed by competitive rates and no hidden monthly fees."
    ));
    contentLayout->addWidget(createBody(
        "You can also set custom goals, track progress, and move money instantly between chequing and savings "
        "accounts, making it simple to manage your finances from anywhere."
    ));
    contentLayout->addWidget(createBody(
        "With round-up savings options, automatic transfers, and insights into your spending habits, "
        "our savings accounts support long-term financial growth in a manageable and convenient way."
    ));
    // ---------------- DEBIT CARDS ----------------
    contentLayout->addWidget(createTitle("Debit Cards"));

    contentLayout->addWidget(createBody(
        "Our debit cards provide secure, instant access to your funds for everyday purchases and ATM withdrawals. "
        "With convenient tap-to-pay and chip security, you can make transactions quickly while keeping your "
        "account protected through advanced encryption and fraud monitoring systems."
    ));
    contentLayout->addWidget(createBody(
        "Use your debit card for in-store purchases, online shopping, bill payments, and Interac transactions. "
        "Track spending in real time through the app and enjoy instant alerts for every transaction so you always "
        "know what is happening with your account."
    ));
    contentLayout->addWidget(createBody(
        "For added security, your card includes PIN protection, 24/7 fraud detection, and the ability to instantly "
        "lock or freeze your card from the app if it is lost or stolen. You maintain control of your card access at all times."
    ));
    contentLayout->addWidget(createBody(
        "We also offer contactless payment compatibility with major digital wallets, allowing you to pay with "
        "your phone or smartwatch wherever tap-to-pay is accepted. Our debit cards make everyday banking simple, "
        "safe, and aligned with your lifestyle."
    ));

    // ---------------- CREDIT CARDS ----------------
    contentLayout->addWidget(createTitle("Credit Cards"));

    contentLayout->addWidget(createBody(
        "Our credit card program offers convenience, protection, and the ability to build your credit responsibly. "
        "Whether you're making everyday purchases or shopping online, enjoy secure payments and real-time monitoring."
    ));
    contentLayout->addWidget(createBody(
        "Choose from cards designed for beginners, avid travelers, or users looking for cash-back rewards. "
        "We offer secure tap-to-pay features, zero-liability fraud protection, and reward programs based on spending categories."
    ));
    contentLayout->addWidget(createBody(
        "Manage your card through the mobile app, set spending limits, track purchases, and redeem rewards with ease. "
        "Detailed budget tools help you monitor habits and maintain a healthy credit profile."
    ));
    contentLayout->addWidget(createBody(
        "With flexible payment options, low-interest plans, and free credit-score monitoring, our credit cards "
        "empower you to build financial confidence while enjoying premium features and protections."
    ));

    // ---------------- LOANS ----------------
    contentLayout->addWidget(createTitle("Loans"));

    contentLayout->addWidget(createBody(
        "We provide personal and student loan solutions designed to support you through major life milestones. "
        "With transparent terms, predictable repayment schedules, and competitive rates, borrowing becomes more manageable."
    ));
    contentLayout->addWidget(createBody(
        "Our digital loan tools help you apply online, estimate payments, and review your borrowing options "
        "in minutes. No lengthy paperwork or branch visits required."
    ));
    contentLayout->addWidget(createBody(
        "Whether you're financing education, consolidating debt, or covering unexpected costs, our loan services "
        "prioritize flexibility and guidance to help you borrow responsibly."
    ));
    contentLayout->addWidget(createBody(
        "Stay in control with automatic payments, clear statements, and repayment reminders. At every step, "
        "we strive to make borrowing a transparent and supportive experience."
    ));

    contentLayout->addStretch();
    scrollArea->setWidget(content);
    layout->addWidget(scrollArea);

    return overlay;
}


QWidget* SettingsPage::createAppOverlay() {
    QWidget* overlay = new QWidget();
    overlay->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    QVBoxLayout* layout = new QVBoxLayout(overlay);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Header
    QWidget* header = createOverlayHeader("Get to Know the App", overlay);
    layout->addWidget(header);

    // Scroll area
    QScrollArea* scrollArea = new QScrollArea(overlay);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("QScrollArea { background-color: transparent; border: none; }");

    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 10, 20, 20);
    contentLayout->setSpacing(15);

    auto title = [&](QString s) {
        QLabel* l = new QLabel(s);
        l->setStyleSheet("font-size:18px; font-weight:700; color:black;");
        return l;
        };

    auto text = [&](QString s) {
        QLabel* l = new QLabel(s);
        l->setWordWrap(true);
        l->setStyleSheet("font-size:14px; color:#333;");
        return l;
        };

    // ? DASHBOARD
    contentLayout->addWidget(title("Dashboard"));
    contentLayout->addWidget(text(
        "The dashboard provides a quick, real-time overview of your financial profile. "
        "You can instantly see your balances across different accounts including chequing, savings, credit card balances, and loan amounts."
    ));
    contentLayout->addWidget(text(
        "This section makes it easy to monitor your financial health at a glance, view recent account information, "
        "and quickly access essential features throughout the app. It is designed to give you everything you need "
        "as soon as you log in."
    ));

    // ? TRANSACTIONS PAGE
    contentLayout->addWidget(title("Transactions"));
    contentLayout->addWidget(text(
        "The Transactions page provides a chronological list of your recent activity, including deposits, withdrawals, "
        "transfers, and account charges. Each entry includes clear labeling to help you quickly understand your financial activity."
    ));
    contentLayout->addWidget(text(
        "This page helps you verify transactions, track spending habits, and ensure all activity matches your expectations. "
        "Whether you're reviewing purchases or confirming deposits, this section keeps your financial history organized and accessible."
    ));

    // ? BANKING OPERATIONS PAGE
    contentLayout->addWidget(title("Banking Operations"));
    contentLayout->addWidget(text(
        "The Banking Operations page includes all transactional tools you need to manage your accounts. "
        "Here, you can send e-Transfers, move money between your accounts (such as chequing to savings), "
        "make withdrawals, and even open new accounts."
    ));
    contentLayout->addWidget(text(
        "This page also offers e-Deposit features for cheque deposits, and a mini-statement generation tool "
        "to review or download a quick summary of your recent financial activity. All actions are intuitive "
        "and secure, ensuring smooth financial management on the go."
    ));

    // ? SETTINGS PAGE
    contentLayout->addWidget(title("Settings"));
    contentLayout->addWidget(text(
        "In the Settings page, you can personalize your banking experience by updating your profile information "
        "(name, email, phone number, and password). It also offers access to detailed guides explaining our banking products "
        "and services in simple terms."
    ));
    contentLayout->addWidget(text(
        "From this page, you can also reach customer support, read privacy and legal information, and securely sign out of the app. "
        "Settings is designed to give you control, transparency, and convenience in managing your banking preferences."
    ));

    contentLayout->addStretch();

    scrollArea->setWidget(content);
    layout->addWidget(scrollArea);

    return overlay;
}


QWidget* SettingsPage::createContactOverlay() {
    QWidget* overlay = new QWidget();
    overlay->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    QVBoxLayout* layout = new QVBoxLayout(overlay);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QWidget* header = createOverlayHeader("Contact Us", overlay);
    layout->addWidget(header);

    QScrollArea* scrollArea = new QScrollArea(overlay);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: transparent; border: none; }");

    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 15, 20, 20);
    contentLayout->setSpacing(15);

    auto addTitle = [&](const QString& text) {
        QLabel* label = new QLabel(text, content);
        label->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: black; }");
        contentLayout->addWidget(label);
        };

    auto addText = [&](const QString& text) {
        QLabel* label = new QLabel(text, content);
        label->setWordWrap(true);
        label->setStyleSheet("QLabel { font-size: 14px; color: black; }");
        contentLayout->addWidget(label);
        };

    addTitle("We Are Here To Help");
    addText(
        "Our support team at GSBS Sudbury Banking is committed to ensuring your banking "
        "experience is safe, easy, and reliable. Whether you need help with your account, "
        "transactions, or navigating the app, we are ready to assist."
    );

    addTitle("Customer Support Phone");
    addText("Sudbury Office: 705-555-0198\nToll-Free (Canada): 1-888-555-4722");

    addTitle("Email Support");
    addText("support@gsbsbank.ca");

    addTitle("Office Location");
    addText(
        "GSBS Digital Banking\n"
        "935 Ramsey Lake Road\n"
        "Sudbury, ON\n"
        "P3E 2C6"
    );

    addTitle("Office Hours");
    addText(
        "Monday to Friday: 9:00 AM to 5:00 PM\n"
        "Saturday: 10:00 AM to 2:00 PM\n"
        "Sunday: Closed"
    );

    addTitle("Notes");
    addText(
        "For your security, please do not share your password or verification codes with anyone. "
        "GSBS staff will never request confidential login details."
    );

    scrollArea->setWidget(content);
    layout->addWidget(scrollArea);

    return overlay;
}


QWidget* SettingsPage::createPrivacyOverlay() {
    QWidget* overlay = new QWidget();
    overlay->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    QVBoxLayout* layout = new QVBoxLayout(overlay);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Header
    QWidget* header = createOverlayHeader("Privacy and Legal", overlay);
    layout->addWidget(header);

    // Scrollable content
    QScrollArea* scrollArea = new QScrollArea(overlay);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: transparent; border: none; }");

    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 15, 20, 20);
    contentLayout->setSpacing(15);

    auto addTitle = [&](const QString& text) {
        QLabel* label = new QLabel(text, content);
        label->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: black; }");
        contentLayout->addWidget(label);
        };

    auto addText = [&](const QString& text) {
        QLabel* label = new QLabel(text, content);
        label->setWordWrap(true);
        label->setStyleSheet("QLabel { font-size: 14px; color: black; }");
        contentLayout->addWidget(label);
        };

    addTitle("Our Commitment to Your Privacy");
    addText(
        "At GSBS Sudbury Banking, protecting your personal information is one of our highest "
        "priorities. We follow Canadian banking privacy standards to ensure your information "
        "remains confidential, secure, and used only for appropriate banking purposes."
    );
    addText(
        "We never sell your personal information. Access is restricted to authorized staff "
        "who require this information to support your banking services and ensure secure "
        "account management."
    );

    addTitle("How We Use Your Information");
    addText(
        "We collect and use your information to help provide the banking services you expect, "
        "including account access, security verification, fraud prevention, and customer support. "
        "We may also use your information to improve our digital banking services and notify you "
        "about important changes or updates."
    );

    addTitle("Data Security");
    addText(
        "We use secure technology and industry-standard encryption to help protect your data. "
        "We may ask for identity confirmation before discussing account details to protect against "
        "unauthorized access. To help safeguard your account, never share your login details, "
        "passwords, or verification codes with anyone."
    );

    addTitle("User Responsibilities");
    addText(
        "As a user, you are responsible for maintaining the confidentiality of your login "
        "credentials and making sure your device is secure when accessing mobile banking services. "
        "If you believe your account has been compromised, contact us immediately."
    );

    addTitle("Legal Notice");
    addText(
        "Use of the GSBS banking application is subject to Canadian law. All transactions and services "
        "are governed by local banking regulations. By using this app, you agree to follow security "
        "policies, acceptable use guidelines, and any updated terms that may be provided."
    );

    addTitle("Questions About Privacy");
    addText(
        "For more information about privacy policies, user rights, or data handling, contact our "
        "support team. We will be happy to assist you with any questions or concerns."
    );

    scrollArea->setWidget(content);
    layout->addWidget(scrollArea);

    return overlay;
}


QWidget* SettingsPage::createSignOutPopUp() {
    QWidget* overlay = new QWidget();
    overlay->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    QVBoxLayout* layout = new QVBoxLayout(overlay);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Header
    QWidget* header = createOverlayHeader("Sign Out", overlay);
    layout->addWidget(header);

    // Content area
    QWidget* content = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 40, 20, 20);
    contentLayout->setSpacing(20);

    // Message label
    QLabel* confirmLabel = new QLabel("Are you sure you want to sign out?", content);
    confirmLabel->setStyleSheet("font-size: 16px; font-weight: 600; color: black;");
    confirmLabel->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(confirmLabel);

    // Buttons container
    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->setSpacing(15);

    // Cancel button (white with green border)
    QPushButton* cancelBtn = new QPushButton("Cancel", content);
    cancelBtn->setMinimumHeight(45);
    cancelBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: white;"
        "   color: #00cc00;"
        "   border: 2px solid #00cc00;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #f4fff4; }"
        "QPushButton:pressed { background-color: #e8ffe8; }"
    );

    // Confirm button (red)
    QPushButton* signOutBtn = new QPushButton("Sign Out", content);
    signOutBtn->setMinimumHeight(45);
    signOutBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #C73636;"
        "   color: white;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #B12F2F; }"
        "QPushButton:pressed { background-color: #992222; }"
    );

    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(signOutBtn);
    contentLayout->addLayout(btnRow);

    // Add content to layout
    layout->addWidget(content);

    // Button logic
    connect(cancelBtn, &QPushButton::clicked, [this]() {
        stackedWidget_->setCurrentIndex(MAIN_MENU);
        });

    connect(signOutBtn, &QPushButton::clicked, [this]() {
        handleSignOut();   // Call your logout logic
        });

    return overlay;
}


void SettingsPage::handleProfileSend() {

}

void SettingsPage::handleSignOut() {

}


void SettingsPage::onProfileClicked() {
    showOverlay(PROFILE);
}
void SettingsPage::onProductsClicked() {
    showOverlay(PRODUCTS);
}
void SettingsPage::onAppClicked() {
    showOverlay(APP);
}
void SettingsPage::onContactClicked() {
    showOverlay(CONTACT);
}
void SettingsPage::onPrivacyClicked() {
    showOverlay(PRIVACY);
}
void SettingsPage::onSignOutClicked() {
    showOverlay(SIGNOUT);
}
void SettingsPage::onFAQClicked() {
    showOverlay(FAQ);
}

void SettingsPage::onShow() {
    qDebug() << "SettingsPage::onShow called";
}
