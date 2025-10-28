// Brandon Cotesta|| 10/28/2025 | 1:00 PM
// Example login page

#include "LoginPage.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

LoginPage::LoginPage()
    : Page("Login"),
      usernameInput_(nullptr),
      passwordInput_(nullptr),
      loginButton_(nullptr),
      registerButton_(nullptr),
      statusLabel_(nullptr)
{
}

LoginPage::~LoginPage() {
    // Qt's parent-child system handles cleanup
}

void LoginPage::buildUI() {
    QWidget* centralWidget = getCentralWidget();
    
    // Create a centered container
    QWidget* container = new QWidget(centralWidget);
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    
    // Title
    QLabel* titleLabel = new QLabel("GSBS Banking System", container);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    containerLayout->addWidget(titleLabel);
    
    containerLayout->addSpacing(20);
    
    // Username input
    QLabel* usernameLabel = new QLabel("Username (Email):", container);
    containerLayout->addWidget(usernameLabel);
    
    usernameInput_ = new QLineEdit(container);
    usernameInput_->setPlaceholderText("Enter your email");
    containerLayout->addWidget(usernameInput_);
    
    // Password input
    QLabel* passwordLabel = new QLabel("Password:", container);
    containerLayout->addWidget(passwordLabel);
    
    passwordInput_ = new QLineEdit(container);
    passwordInput_->setPlaceholderText("Enter your password");
    passwordInput_->setEchoMode(QLineEdit::Password);
    containerLayout->addWidget(passwordInput_);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    loginButton_ = new QPushButton("Login", container);
    registerButton_ = new QPushButton("Register", container);
    
    buttonLayout->addWidget(loginButton_);
    buttonLayout->addWidget(registerButton_);
    
    containerLayout->addLayout(buttonLayout);
    
    // Status label
    statusLabel_ = new QLabel("", container);
    statusLabel_->setAlignment(Qt::AlignCenter);
    containerLayout->addWidget(statusLabel_);
    
    // Add stretch to center content
    containerLayout->addStretch();
    
    // Add container to main layout
    mainLayout_->addStretch();
    mainLayout_->addWidget(container);
    mainLayout_->addStretch();
    
    // Connect signals
    QObject::connect(loginButton_, &QPushButton::clicked, [this]() { handleLogin(); });
    QObject::connect(registerButton_, &QPushButton::clicked, [this]() { handleRegister(); });
}

void LoginPage::onShow() {
    // Clear inputs when page is shown
    if (usernameInput_) usernameInput_->clear();
    if (passwordInput_) passwordInput_->clear();
    if (statusLabel_) statusLabel_->clear();
}

void LoginPage::setLoginSuccessCallback(std::function<void()> callback) {
    onLoginSuccess_ = callback;
}

void LoginPage::handleLogin() {
    QString username = usernameInput_->text();
    QString password = passwordInput_->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        statusLabel_->setText("Please enter username and password");
        statusLabel_->setStyleSheet("color: red;");
        return;
    }
    
    // Attempt login
    SessionManager session;
    // Note: You'll need to adapt SessionManager to work with QString
    // or convert: username.toStdString()
    
    // For now, simulate success
    statusLabel_->setText("Login successful!");
    statusLabel_->setStyleSheet("color: green;");
    
    if (onLoginSuccess_) {
        onLoginSuccess_();
    }
}

void LoginPage::handleRegister() {
    statusLabel_->setText("Registration not implemented yet");
    statusLabel_->setStyleSheet("color: orange;");
}