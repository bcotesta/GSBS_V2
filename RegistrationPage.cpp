// Brandon Cotesta || 10/28/2025 | 2:17 PM
// Registration page for new users

#include "RegistrationPage.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QEasingCurve>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

RegistrationPage::RegistrationPage()
    : Page("Register"),
      containerWidget_(nullptr),
      nameInput_(nullptr),
      emailInput_(nullptr),
      phoneInput_(nullptr),
      passwordInput_(nullptr),
      confirmPasswordInput_(nullptr),
      registerButton_(nullptr),
      cancelButton_(nullptr),
      backToLoginButton_(nullptr),
      statusLabel_(nullptr),
      titleLabel_(nullptr),
      showPasswordCheckbox_(nullptr),
      auth_(Authenticator::getInstance())
{
}

RegistrationPage::~RegistrationPage() {
    // Qt's parent-child system handles cleanup
}

void RegistrationPage::buildUI() {
    QWidget* centralWidget = getCentralWidget();
    
    // Set background color for the page
    centralWidget->setStyleSheet("QWidget { background-color: #f5f5f5; }");
    
    // Create a mobile-like centered container
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
    containerLayout->setSpacing(10);
    containerLayout->setContentsMargins(40, 30, 40, 30);
    
    // Title with modern styling
    titleLabel_ = new QLabel("Create Account", containerWidget_);
    QFont titleFont("Segoe UI", 26, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    titleLabel_->setMinimumHeight(40);
    containerLayout->addWidget(titleLabel_);
    
    containerLayout->addSpacing(3);
    
    // Subtitle
    QLabel* subtitleLabel = new QLabel("Sign up to get started", containerWidget_);
    QFont subtitleFont("Segoe UI", 11);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("QLabel { color: #7f8c8d; }");
    containerLayout->addWidget(subtitleLabel);
    
    containerLayout->addSpacing(12);
    
    // Name input
    QLabel* nameLabel = new QLabel("Full Name", containerWidget_);
    nameLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; font-size: 12px; }");
    containerLayout->addWidget(nameLabel);
    
    containerLayout->addSpacing(4);
    
    nameInput_ = new QLineEdit(containerWidget_);
    nameInput_->setPlaceholderText("Enter your full name");
    nameInput_->setMinimumHeight(45);
    nameInput_->setStyleSheet(
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
    containerLayout->addWidget(nameInput_);
    
    containerLayout->addSpacing(6);
    
    // Email input
    QLabel* emailLabel = new QLabel("Email", containerWidget_);
    emailLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; font-size: 12px; }");
    containerLayout->addWidget(emailLabel);
    
    containerLayout->addSpacing(4);
    
    emailInput_ = new QLineEdit(containerWidget_);
    emailInput_->setPlaceholderText("Enter your email");
    emailInput_->setMinimumHeight(45);
    emailInput_->setStyleSheet(
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
    containerLayout->addWidget(emailInput_);
    
    containerLayout->addSpacing(6);
    
    // Phone input
    QLabel* phoneLabel = new QLabel("Phone Number (Optional)", containerWidget_);
    phoneLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; font-size: 12px; }");
    containerLayout->addWidget(phoneLabel);
    
    containerLayout->addSpacing(4);
    
    phoneInput_ = new QLineEdit(containerWidget_);
    phoneInput_->setPlaceholderText("Enter your phone number");
    phoneInput_->setMinimumHeight(45);
    phoneInput_->setStyleSheet(
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
    containerLayout->addWidget(phoneInput_);
    
    containerLayout->addSpacing(6);
    
    // Password input
    QLabel* passwordLabel = new QLabel("Password", containerWidget_);
    passwordLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; font-size: 12px; }");
    containerLayout->addWidget(passwordLabel);
    
    containerLayout->addSpacing(4);
    
    passwordInput_ = new QLineEdit(containerWidget_);
    passwordInput_->setPlaceholderText("Enter your password");
    passwordInput_->setEchoMode(QLineEdit::Password);
    passwordInput_->setMinimumHeight(45);
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
    
    containerLayout->addSpacing(6);
    
    // Confirm Password input
    QLabel* confirmPasswordLabel = new QLabel("Confirm Password", containerWidget_);
    confirmPasswordLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; font-size: 12px; }");
    containerLayout->addWidget(confirmPasswordLabel);
    
    containerLayout->addSpacing(4);
    
    confirmPasswordInput_ = new QLineEdit(containerWidget_);
    confirmPasswordInput_->setPlaceholderText("Re-enter your password");
    confirmPasswordInput_->setEchoMode(QLineEdit::Password);
    confirmPasswordInput_->setMinimumHeight(45);
    confirmPasswordInput_->setStyleSheet(
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
    containerLayout->addWidget(confirmPasswordInput_);
    
    containerLayout->addSpacing(5);
    
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
        "} "
        "QCheckBox::indicator:hover {"
        "   border: 2px solid #3498db;"
        "} "
        "QCheckBox::indicator:checked {"
        "   background-color: #3498db;"
        "   border: 2px solid #3498db;"
        "}"
    );
    showPasswordCheckbox_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(showPasswordCheckbox_);
    
    containerLayout->addSpacing(6);
    
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
    
    containerLayout->addSpacing(4);
    
    // Register button
    registerButton_ = new QPushButton("Create Account", containerWidget_);
    registerButton_->setStyleSheet(
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
    registerButton_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(registerButton_);
    
    containerLayout->addSpacing(10);
    
    // Divider
    QFrame* divider = new QFrame(containerWidget_);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("QFrame { color: #e0e0e0; }");
    containerLayout->addWidget(divider);
    
    containerLayout->addSpacing(4);
    
    // Back to login button
    backToLoginButton_ = new QPushButton("Already have an account? Sign In", containerWidget_);
    backToLoginButton_->setStyleSheet(
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
    backToLoginButton_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(backToLoginButton_);
    
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
    QObject::connect(registerButton_, &QPushButton::clicked, [this]() { handleRegister(); });
    QObject::connect(backToLoginButton_, &QPushButton::clicked, [this]() { handleBackToLogin(); });
    QObject::connect(showPasswordCheckbox_, &QCheckBox::toggled, [this](bool checked) { handleShowPasswordToggle(checked); });
    
    // Enable Enter key to submit
    QObject::connect(nameInput_, &QLineEdit::returnPressed, [this]() { handleRegister(); });
    QObject::connect(emailInput_, &QLineEdit::returnPressed, [this]() { handleRegister(); });
    QObject::connect(phoneInput_, &QLineEdit::returnPressed, [this]() { handleRegister(); });
    QObject::connect(passwordInput_, &QLineEdit::returnPressed, [this]() { handleRegister(); });
    QObject::connect(confirmPasswordInput_, &QLineEdit::returnPressed, [this]() { handleRegister(); });
}

void RegistrationPage::onShow() {
    // Clear inputs when page is shown
    if (nameInput_) {
        nameInput_->clear();
        nameInput_->setFocus();
    }
    if (emailInput_) emailInput_->clear();
    if (phoneInput_) phoneInput_->clear();
    if (passwordInput_) passwordInput_->clear();
    if (confirmPasswordInput_) confirmPasswordInput_->clear();
    if (statusLabel_) {
        statusLabel_->clear();
        statusLabel_->hide();
    }
    if (showPasswordCheckbox_) showPasswordCheckbox_->setChecked(false);
}

void RegistrationPage::setRegistrationSuccessCallback(std::function<void(User*)> callback) {
    onRegistrationSuccess_ = callback;
}

void RegistrationPage::setBackToLoginCallback(std::function<void()> callback) {
    onBackToLoginClick_ = callback;
}

bool RegistrationPage::validateInputs() {
    QString name = nameInput_->text().trimmed();
    QString email = emailInput_->text().trimmed();
    QString phone = phoneInput_->text().trimmed();
    QString password = passwordInput_->text();
    QString confirmPassword = confirmPasswordInput_->text();
    
    // Check required fields
    if (name.isEmpty() || email.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        showError("Please fill in all required fields");
        return false;
    }
    
    // Validate email format
    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        showError("Please enter a valid email address");
        return false;
    }
    
    // Check password length
    if (password.length() < 6) {
        showError("Password must be at least 6 characters long");
        return false;
    }
    
    // Check passwords match
    if (password != confirmPassword) {
        showError("Passwords do not match");
        return false;
    }
    
    return true;
}

void RegistrationPage::handleRegister() {
    if (!validateInputs()) {
        return;
    }
    
    QString name = nameInput_->text().trimmed();
    QString email = emailInput_->text().trimmed();
    QString phone = phoneInput_->text().trimmed();
    QString password = passwordInput_->text();
    
    // Disable button during processing
    registerButton_->setEnabled(false);
    registerButton_->setText("Creating account...");
    
    // Convert QString to std::string
    std::string nameStr = name.toStdString();
    std::string emailStr = email.toStdString();
    std::string phoneStr = phone.toStdString();
    std::string passwordStr = password.toStdString();
    
    // Attempt registration using Authenticator
    bool success = auth_.registerNewUser(nameStr, emailStr, phoneStr, passwordStr);
    
    if (success) {
        // Create User object from registered data
        User* user = new User(
            auth_.getUserID(),
            auth_.getUsername(),
            passwordStr
        );
        
        showSuccess("Account created successfully! Welcome.");
        
        // Call success callback with user object
        if (onRegistrationSuccess_) {
            onRegistrationSuccess_(user);
        }
    } else {
        showError("Registration failed. Email may already be in use.");
        registerButton_->setEnabled(true);
        registerButton_->setText("Create Account");
    }
}

void RegistrationPage::handleBackToLogin() {
    // Clear status
    if (statusLabel_) {
        statusLabel_->clear();
        statusLabel_->hide();
    }
    
    // Trigger back to login callback if set
    if (onBackToLoginClick_) {
        onBackToLoginClick_();
    }
}

void RegistrationPage::handleShowPasswordToggle(bool checked) {
    if (passwordInput_ && confirmPasswordInput_) {
        if (checked) {
            passwordInput_->setEchoMode(QLineEdit::Normal);
            confirmPasswordInput_->setEchoMode(QLineEdit::Normal);
        } else {
            passwordInput_->setEchoMode(QLineEdit::Password);
            confirmPasswordInput_->setEchoMode(QLineEdit::Password);
        }
    }
}

void RegistrationPage::showError(const QString& message) {
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

void RegistrationPage::showSuccess(const QString& message) {
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