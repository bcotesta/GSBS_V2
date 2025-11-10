#include "OTPPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <iostream>

OTPPage::OTPPage()
    : Page("Verify Identity"),
      containerWidget_(nullptr),
      titleLabel_(nullptr),
      instructionLabel_(nullptr),
      otpInput_(nullptr),
      verifyButton_(nullptr),
      resendButton_(nullptr),
      cancelButton_(nullptr),
      statusLabel_(nullptr),
      timerLabel_(nullptr),
      auth_(Authenticator::getInstance()),
      deliveryMethod_("phone"),
      remainingSeconds_(300) // 5 minutes
{
    countdownTimer_ = new QTimer(this);
    connect(countdownTimer_, &QTimer::timeout, this, &OTPPage::updateTimer);
}

OTPPage::~OTPPage() {
    if (countdownTimer_) {
        countdownTimer_->stop();
    }
}

void OTPPage::buildUI() {
    QWidget* centralWidget = getCentralWidget();
    centralWidget->setStyleSheet("QWidget { background-color: #f5f5f5; }");
    
    // Create centered container
    containerWidget_ = new QWidget(centralWidget);
    containerWidget_->setFixedWidth(420);
    containerWidget_->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border-radius: 12px;"
        "}"
    );
    
    QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget_);
    containerLayout->setSpacing(15);
    containerLayout->setContentsMargins(40, 35, 40, 35);
    
    // Title
    titleLabel_ = new QLabel("Two-Factor Authentication", containerWidget_);
    QFont titleFont("Segoe UI", 19, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    containerLayout->addWidget(titleLabel_);
    
    containerLayout->addSpacing(10);
    
    // Instruction label
    instructionLabel_ = new QLabel("We've sent a verification code to your phone", containerWidget_);
    instructionLabel_->setFont(QFont("Segoe UI", 11));
    instructionLabel_->setAlignment(Qt::AlignCenter);
    instructionLabel_->setWordWrap(true);
    instructionLabel_->setStyleSheet("QLabel { color: #7f8c8d; }");
    containerLayout->addWidget(instructionLabel_);
    
    containerLayout->addSpacing(20);
    
    // OTP Input
    QLabel* otpLabel = new QLabel("Verification Code", containerWidget_);
    otpLabel->setStyleSheet("QLabel { color: #2c3e50; font-weight: 600; font-size: 12px; }");
    containerLayout->addWidget(otpLabel);
    
    otpInput_ = new QLineEdit(containerWidget_);
    otpInput_->setPlaceholderText("Enter 6-digit code");
    otpInput_->setMaxLength(6);
    otpInput_->setAlignment(Qt::AlignCenter);
    otpInput_->setStyleSheet(
        "QLineEdit {"
        "   padding: 16px;"
        "   border: 2px solid #e0e0e0;"
        "   border-radius: 8px;"
        "   font-size: 24px;"
        "   font-weight: bold;"
        "   letter-spacing: 8px;"
		"   color: #2c3e50;"
        "   background-color: #f9f9f9;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #00cc00;"
        "   background-color: white;"
        "}"
    );
    containerLayout->addWidget(otpInput_);
    
    containerLayout->addSpacing(10);
    
    // Timer label
    timerLabel_ = new QLabel("Code expires in 5:00", containerWidget_);
    timerLabel_->setAlignment(Qt::AlignCenter);
    timerLabel_->setStyleSheet("QLabel { color: #95a5a6; font-size: 12px; }");
    containerLayout->addWidget(timerLabel_);
    
    containerLayout->addSpacing(10);
    
    // Status label
    statusLabel_ = new QLabel("", containerWidget_);
    statusLabel_->setAlignment(Qt::AlignCenter);
    statusLabel_->setWordWrap(true);
    statusLabel_->setStyleSheet("QLabel { padding: 8px; border-radius: 6px; font-size: 13px; }");
    statusLabel_->setMinimumHeight(0);
    statusLabel_->hide();
    containerLayout->addWidget(statusLabel_);
    
    containerLayout->addSpacing(10);
    
    // Verify button
    verifyButton_ = new QPushButton("Verify Code", containerWidget_);
    verifyButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #00cc00;"
        "   color: white;"
        "   border: none;"
        "   padding: 14px;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover { background-color: #029e02; }"
        "QPushButton:pressed { background-color: #027a02; }"
        "QPushButton:disabled {"
        "   background-color: #cccccc;"
        "   color: #666666;"
        "}"
    );
    verifyButton_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(verifyButton_);
    
    containerLayout->addSpacing(10);
    
    // Resend button
    resendButton_ = new QPushButton("Resend Code", containerWidget_);
    resendButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #3498db;"
        "   border: 2px solid #3498db;"
        "   padding: 12px;"
        "   border-radius: 8px;"
        "   font-size: 14px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover { background-color: #ecf6fc; }"
        "QPushButton:pressed { background-color: #d6eaf8; }"
    );
    resendButton_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(resendButton_);
    
    containerLayout->addSpacing(5);
    
    // Cancel button
    cancelButton_ = new QPushButton("Cancel", containerWidget_);
    cancelButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #95a5a6;"
        "   border: none;"
        "   padding: 10px;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover { color: #7f8c8d; text-decoration: underline; }"
    );
    cancelButton_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(cancelButton_);
    
    // Add container to main layout with centering
    mainLayout_->addStretch();
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(containerWidget_);
    hLayout->addStretch();
    mainLayout_->addLayout(hLayout);
    mainLayout_->addStretch();
    
    // Connect signals
    connect(verifyButton_, &QPushButton::clicked, this, &OTPPage::handleVerify);
    connect(resendButton_, &QPushButton::clicked, this, &OTPPage::handleResend);
    connect(cancelButton_, &QPushButton::clicked, this, &OTPPage::handleCancel);
    connect(otpInput_, &QLineEdit::returnPressed, this, &OTPPage::handleVerify);
}

void OTPPage::onShow() {
    Page::onShow();
    
    if (otpInput_) {
        otpInput_->clear();
        otpInput_->setFocus();
    }
    if (statusLabel_) {
        statusLabel_->clear();
        statusLabel_->hide();
    }
    
    // Reset timer
    remainingSeconds_ = 300; // Reset to 5 minutes
    
    // Defer sendOTP() and countdown to allow layout to settle
    // This ensures the UI is fully built and positioned before starting
    QTimer::singleShot(0, this, [this]() {
        sendOTP();
        startCountdown();
    });
}

void OTPPage::setUserID(const std::string& userID) {
    userID_ = userID;
}

void OTPPage::setDeliveryMethod(const std::string& method) {
    deliveryMethod_ = method;
    
    if (instructionLabel_) {
        QString text = "We've sent a verification code to your " + 
                      QString::fromStdString(method);
        instructionLabel_->setText(text);
    }
}

void OTPPage::setVerificationSuccessCallback(std::function<void()> callback) {
    onVerificationSuccess_ = callback;
}

void OTPPage::setVerificationFailedCallback(std::function<void()> callback) {
    onVerificationFailed_ = callback;
}

void OTPPage::setCancelCallback(std::function<void()> callback) {
    onCancel_ = callback;
}

void OTPPage::handleVerify() {
    QString code = otpInput_->text().trimmed();
    
    if (code.length() != 6) {
        showError("Please enter a 6-digit verification code");
        return;
    }
    
    verifyButton_->setEnabled(false);
    verifyButton_->setText("Verifying...");
    
    // Verify OTP
    bool verified = auth_.verifyOTP(userID_, code.toStdString());
    
    if (verified) {
        countdownTimer_->stop();
        showSuccess("Verification successful!");
        // reset button in case of logout
        verifyButton_->setEnabled(true);
        verifyButton_->setText("Verify Code");
        // Call success callback
        if (onVerificationSuccess_) {
            QTimer::singleShot(1000, [this]() {
                onVerificationSuccess_();
            });
        }
    } else {
        showError("Invalid or expired code. Please try again.");
        verifyButton_->setEnabled(true);
        verifyButton_->setText("Verify Code");
        otpInput_->clear();
        otpInput_->setFocus();
    }
}

void OTPPage::handleResend() {
    resendButton_->setEnabled(false);
    resendButton_->setText("Sending...");
    
    sendOTP();
    
    // Reset timer
    remainingSeconds_ = 300;
    startCountdown();
    
    showInfo("A new verification code has been sent");
    
    QTimer::singleShot(30000, [this]() { // Re-enable after 30 seconds
        if (resendButton_) {
            resendButton_->setEnabled(true);
            resendButton_->setText("Resend Code");
        }
    });
}

void OTPPage::handleCancel() {
    countdownTimer_->stop();
    
    if (onCancel_) {
        onCancel_();
    }
}

void OTPPage::sendOTP() {
    bool sent = auth_.sendOTP(userID_, deliveryMethod_);
    
    if (!sent) {
        showError("Failed to send verification code. Please try again.");
    } else {
        std::cout << "[OTP] Verification code sent to user " << userID_ << std::endl;
    }
}

void OTPPage::startCountdown() {
    countdownTimer_->start(1000); // Update every second
}

void OTPPage::updateTimer() {
    remainingSeconds_--;
    
    if (remainingSeconds_ <= 0) {
        countdownTimer_->stop();
        timerLabel_->setText("Code expired");
        timerLabel_->setStyleSheet("QLabel { color: #e74c3c; font-size: 12px; font-weight: bold; }");
        verifyButton_->setEnabled(false);
        showError("Verification code has expired. Please request a new code.");
        return;
    }
    
    int minutes = remainingSeconds_ / 60;
    int seconds = remainingSeconds_ % 60;
    QString timeStr = QString("Code expires in %1:%2")
                      .arg(minutes)
                      .arg(seconds, 2, 10, QChar('0'));
    
    timerLabel_->setText(timeStr);
    
    // Change color when time is running out
    if (remainingSeconds_ <= 60) {
        timerLabel_->setStyleSheet("QLabel { color: #e67e22; font-size: 12px; font-weight: bold; }");
    }
}

void OTPPage::showError(const QString& message) {
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

void OTPPage::showSuccess(const QString& message) {
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

void OTPPage::showInfo(const QString& message) {
    statusLabel_->setText(message);
    statusLabel_->setStyleSheet(
        "QLabel {"
        "   background-color: #e6f3ff;"
        "   color: #2980b9;"
        "   padding: 10px;"
        "   border-radius: 6px;"
        "   border-left: 4px solid #3498db;"
        "}"
    );
    statusLabel_->show();
}