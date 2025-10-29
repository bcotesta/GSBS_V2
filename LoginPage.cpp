// Brandon Cotesta|| 10/28/2025 | 1:00 PM
// Example login page

#include "LoginPage.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QEasingCurve>

LoginPage::LoginPage()
    : Page("Login"),
      containerWidget_(nullptr),
      usernameInput_(nullptr),
      passwordInput_(nullptr),
      loginButton_(nullptr),
      registerButton_(nullptr),
      statusLabel_(nullptr),
      titleLabel_(nullptr),
      showPasswordCheckbox_(nullptr),
      auth_(Authenticator::getInstance())
{
}

LoginPage::~LoginPage() {
    // Qt's parent-child system handles cleanup
}

void LoginPage::buildUI() {
    QWidget* centralWidget = getCentralWidget();
    
    // Set background color for the page
    centralWidget->setStyleSheet("QWidget { background-color: #f5f5f5; }");
    
    // Create a mobile-like centered container with shadow effect
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
    
    // Title with modern styling
    titleLabel_ = new QLabel("GSBS Banking", containerWidget_);
    QFont titleFont("Segoe UI", 28, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    titleLabel_->setMinimumHeight(45);
    containerLayout->addWidget(titleLabel_);
    
    containerLayout->addSpacing(5);
    
    // Subtitle
    QLabel* subtitleLabel = new QLabel("Sign in to continue", containerWidget_);
    QFont subtitleFont("Segoe UI", 11);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("QLabel { color: #7f8c8d; }");
    containerLayout->addWidget(subtitleLabel);
    
    containerLayout->addSpacing(25);
    
    // Username/Email input with modern styling
    QLabel* usernameLabel = new QLabel("Email", containerWidget_);
    usernameLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; font-size: 12px; }");
    containerLayout->addWidget(usernameLabel);
    
    containerLayout->addSpacing(5);
    
    usernameInput_ = new QLineEdit(containerWidget_);
    usernameInput_->setPlaceholderText("Enter your email");
    usernameInput_->setStyleSheet(
        "QLineEdit {"
        "   padding: 12px 15px;"
        "   border: 2px solid #e0e0e0;"
        "   border-radius: 8px;"
        "   font-size: 14px;"
        "   background-color: #f9f9f9;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #3498db;"
        "   background-color: white;"
        "}"
    );
    containerLayout->addWidget(usernameInput_);
    
    containerLayout->addSpacing(10);
    
    // Password input with modern styling
    QLabel* passwordLabel = new QLabel("Password", containerWidget_);
    passwordLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; font-size: 12px; }");
    containerLayout->addWidget(passwordLabel);
    
    containerLayout->addSpacing(5);
    
    passwordInput_ = new QLineEdit(containerWidget_);
    passwordInput_->setPlaceholderText("Enter your password");
    passwordInput_->setEchoMode(QLineEdit::Password);
    passwordInput_->setStyleSheet(
        "QLineEdit {"
        "   padding: 12px 15px;"
        "   border: 2px solid #e0e0e0;"
        "   border-radius: 8px;"
        "   font-size: 14px;"
        "   background-color: #f9f9f9;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #3498db;"
        "   background-color: white;"
        "}"
    );
    containerLayout->addWidget(passwordInput_);
    
    containerLayout->addSpacing(8);
    
    // Show password checkbox
    showPasswordCheckbox_ = new QCheckBox("Show password", containerWidget_);
    showPasswordCheckbox_->setStyleSheet(
        "QCheckBox {"
        "   color: #7f8c8d;"
        "   font-size: 12px;"
        "   spacing: 8px;"
        "}"
        "QCheckBox::indicator {"
        "   width: 18px;"
        "   height: 18px;"
        "   border: 2px solid #d0d0d0;"
        "   border-radius: 4px;"
        "   background-color: white;"
        "}"
        "QCheckBox::indicator:hover {"
        "   border: 2px solid #3498db;"
        "}"
        "QCheckBox::indicator:checked {"
        "   background-color: #3498db;"
        "   border: 2px solid #3498db;"
        "   image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTIiIGhlaWdodD0iOSIgdmlld0JveD0iMCAwIDEyIDkiIGZpbGw9Im5vbmUiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+PHBhdGggZD0iTTEgNEw0LjUgNy41TDExIDEiIHN0cm9rZT0id2hpdGUiIHN0cm9rZS13aWR0aD0iMiIgc3Ryb2tlLWxpbmVjYXA9InJvdW5kIiBzdHJva2UtbGluZWpvaW49InJvdW5kIi8+PC9zdmc+);"
        "}"
    );
    showPasswordCheckbox_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(showPasswordCheckbox_);
    
    containerLayout->addSpacing(10);
    
    // Status label
    statusLabel_ = new QLabel("", containerWidget_);
    statusLabel_->setAlignment(Qt::AlignCenter);
    statusLabel_->setWordWrap(true);
    statusLabel_->setStyleSheet(
        "QLabel {"
        "   padding: 8px;"
        "   border-radius: 6px;"
        "   font-size: 13px;"
        "}"
    );
    statusLabel_->setMinimumHeight(0);
    statusLabel_->setMaximumHeight(50);
    statusLabel_->hide();
    containerLayout->addWidget(statusLabel_);
    
    containerLayout->addSpacing(5);
    
    // Login button with modern styling
    loginButton_ = new QPushButton("Sign In", containerWidget_);
    loginButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 14px;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #21618c;"
        "}"
    );
    loginButton_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(loginButton_);
    
    containerLayout->addSpacing(15);
    
    // Divider
    QFrame* divider = new QFrame(containerWidget_);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("QFrame { color: #e0e0e0; }");
    containerLayout->addWidget(divider);
    
    containerLayout->addSpacing(5);
    
    // Register button
    registerButton_ = new QPushButton("Create New Account", containerWidget_);
    registerButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #3498db;"
        "   border: 2px solid #3498db;"
        "   padding: 12px;"
        "   border-radius: 8px;"
        "   font-size: 14px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ecf6fc;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #d6eaf8;"
        "}"
    );
    registerButton_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(registerButton_);
    
    // Add container to main layout with centering
    mainLayout_->addStretch();
    
    // Horizontal centering
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(containerWidget_);
    hLayout->addStretch();
    mainLayout_->addLayout(hLayout);
    
    mainLayout_->addStretch();
    
    // Connect signals
    QObject::connect(loginButton_, &QPushButton::clicked, [this]() { handleLogin(); });
    QObject::connect(registerButton_, &QPushButton::clicked, [this]() { handleRegister(); });
    QObject::connect(showPasswordCheckbox_, &QCheckBox::toggled, [this](bool checked) { handleShowPasswordToggle(checked); });
    
    // Enable Enter key to submit
    QObject::connect(usernameInput_, &QLineEdit::returnPressed, [this]() { handleLogin(); });
    QObject::connect(passwordInput_, &QLineEdit::returnPressed, [this]() { handleLogin(); });
}

void LoginPage::onShow() {
    // Clear inputs when page is shown
    if (usernameInput_) {
        usernameInput_->clear();
        usernameInput_->setFocus();
    }
    if (passwordInput_) passwordInput_->clear();
    if (statusLabel_) {
        statusLabel_->clear();
        statusLabel_->hide();
    }
    if (showPasswordCheckbox_) showPasswordCheckbox_->setChecked(false);
}

void LoginPage::setLoginSuccessCallback(std::function<void(User*)> callback) {
    onLoginSuccess_ = callback;
}

void LoginPage::setRegisterCallback(std::function<void()> callback) {
    onRegisterClick_ = callback;
}

void LoginPage::handleLogin() {
    QString username = usernameInput_->text().trimmed();
    QString password = passwordInput_->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        showError("Please enter both email and password");
        return;
    }
    
    // Disable button during processing
    loginButton_->setEnabled(false);
    loginButton_->setText("Signing in...");
    
    // Convert QString to std::string
    std::string usernameStr = username.toStdString();
    std::string passwordStr = password.toStdString();
    
    // Attempt login using Authenticator
    bool success = auth_.verifyCredentials(usernameStr, passwordStr);
    
    if (success) {
        // Create User object from authenticated data
        User* user = new User(
            auth_.getUserID(),
            auth_.getUsername(),
            passwordStr
        );
        
        showSuccess("Login successful! Welcome back.");
        
        // Disable button during processing
        loginButton_->setEnabled(true);
        loginButton_->setText("Sign in");

        // Call success callback with user object
        if (onLoginSuccess_) {
            onLoginSuccess_(user);
        }
    } else {
        showError("Invalid email or password. Please try again.");
        loginButton_->setEnabled(true);
        loginButton_->setText("Sign In");
    }
}

void LoginPage::handleRegister() {
    // Clear status
    if (statusLabel_) {
        statusLabel_->clear();
        statusLabel_->hide();
    }
    
    // Trigger register callback if set
    if (onRegisterClick_) {
        onRegisterClick_();
    }
}

void LoginPage::handleShowPasswordToggle(bool checked) {
    if (passwordInput_) {
        if (checked) {
            passwordInput_->setEchoMode(QLineEdit::Normal);
        } else {
            passwordInput_->setEchoMode(QLineEdit::Password);
        }
    }
}

void LoginPage::showError(const QString& message) {
    statusLabel_->setText(message);
    statusLabel_->setStyleSheet(
        "QLabel {"
        "   background-color: #ffe6e6;"
        "   color: #c0392b;"
        "   padding: 10px;"
        "   border-radius: 6px;"
        "   border-left: 4px solid #e74c3c;"
        "}"
    );
    statusLabel_->show();
}

void LoginPage::showSuccess(const QString& message) {
    statusLabel_->setText(message);
    statusLabel_->setStyleSheet(
        "QLabel {"
        "   background-color: #e6ffe6;"
        "   color: #27ae60;"
        "   padding: 10px;"
        "   border-radius: 6px;"
        "   border-left: 4px solid #2ecc71;"
        "}"
    );
    statusLabel_->show();
}