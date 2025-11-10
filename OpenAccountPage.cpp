#include "OpenAccountPage.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <iomanip>
#include <sstream>


OpenAccountPage::OpenAccountPage()
    :Page("openaccount"),

    containerWidget_(nullptr),
    titleLabel_(nullptr),
    newAccountLabel_(nullptr),
    currentUser_(nullptr),
    newChequingButton_(nullptr),
    newSavingsButton_(nullptr),
    newCreditButton_(nullptr),
    newLoanButton_(nullptr)
{

}

OpenAccountPage::~OpenAccountPage()
{
    // Qt's parent-child system handles cleanup
}

void OpenAccountPage::setUser(User* user) {
    currentUser_ = user;
}

void OpenAccountPage::buildUI() {


    QWidget* centralWidget = getCentralWidget();

    // Set background color for the page
    centralWidget->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    // Create a centered container
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

    // Title
    titleLabel_ = new QLabel("Open Accounts", containerWidget_);
    QFont titleFont("Segoe UI", 28, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    titleLabel_->setMinimumHeight(45);
    containerLayout->addWidget(titleLabel_);

    //New Account button label
    newAccountLabel_ = new QLabel("Select new account:", containerWidget_);
    QFont newAccountFont("Segoe UI", 11, QFont::Bold);
    newAccountLabel_->setFont(newAccountFont);
    newAccountLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    containerLayout->addWidget(newAccountLabel_);


    //New savings account button
    newSavingsButton_ = new QPushButton("Create New Savings Account", containerWidget_);
    newSavingsButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #00cc00;"
        "   color: white;"
        "   border: none;"
        "   padding: 14px;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background-color: #029e02;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #029e02;"
        "}"
    );
    newSavingsButton_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(newSavingsButton_);

    containerLayout->addSpacing(15);


    //new chequing account button
    newChequingButton_ = new QPushButton("Create New Chequing Account", containerWidget_);
    newChequingButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #00cc00;"
        "   color: white;"
        "   border: none;"
        "   padding: 14px;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background-color: #029e02;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #029e02;"
        "}"
    );
    newChequingButton_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(newChequingButton_);

    containerLayout->addSpacing(15);






    //new credit account button
    newCreditButton_ = new QPushButton("Create New Credit Account", containerWidget_);
    newCreditButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #00cc00;"
        "   color: white;"
        "   border: none;"
        "   padding: 14px;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background-color: #029e02;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #029e02;"
        "}"
    );
    newCreditButton_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(newCreditButton_);

    containerLayout->addSpacing(15);





    //new loan account button
    newLoanButton_ = new QPushButton("Create New Loan Account", containerWidget_);
    newLoanButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #00cc00;"
        "   color: white;"
        "   border: none;"
        "   padding: 14px;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background-color: #029e02;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #029e02;"
        "}"
    );
    newLoanButton_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(newLoanButton_);

    containerLayout->addSpacing(15);




    //connect  buttons to functions 

    connect(newSavingsButton_, &QPushButton::clicked, [this]() { onNewSavingsPress(); });
    connect(newChequingButton_, &QPushButton::clicked, [this]() { onNewChequingPress(); });
    connect(newCreditButton_, &QPushButton::clicked, [this]() { onNewCreditPress(); });
    connect(newLoanButton_, &QPushButton::clicked, [this]() { onNewLoanPress(); });


    // Add container to main layout with centering
    mainLayout_->addStretch();

    // Horizontal centering
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(containerWidget_);
    hLayout->addStretch();
    mainLayout_->addLayout(hLayout);

    mainLayout_->addStretch();


}


void OpenAccountPage::onNewSavingsPress()
{
    bool yn = doubleCheck();
    if (yn == true)
    {
        AccountManager accM(*currentUser_);
        AccountType s = AccountType::SAVINGS;
        accM.createAccount(s);
    }
}

void OpenAccountPage::onNewChequingPress()
{
    bool yn = doubleCheck();
    if (yn == true)
    {
        AccountManager accM(*currentUser_);
        AccountType s = AccountType::CHEQUING;
        accM.createAccount(s);
    }
}

void OpenAccountPage::onNewCreditPress()
{
    
    if (doubleCheck() == true)
    {
        AccountManager accM(*currentUser_);
        AccountType s = AccountType::CREDIT;
        accM.createAccount(s);
    }
}

void OpenAccountPage::onNewLoanPress()
{

    if (doubleCheck() == true)
    {
        AccountManager accM(*currentUser_);
        AccountType s = AccountType::LOAN;
        accM.createAccount(s);
    }
}

bool OpenAccountPage::doubleCheck()
{
    //QApplication::activeWindow() will display the QMessageBox in the current active window
    QMessageBox msgBox(QApplication::activeWindow());
    msgBox.setWindowTitle("Create New Account");
    msgBox.setText("Are you sure you want to make a new account?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setIcon(QMessageBox::Question);


    int choice = msgBox.exec();

    if (choice == QMessageBox::Yes) {
        return true;
    }
    
    return false;
}



void OpenAccountPage::onShow() {
    cout << "OpenAccountPage::onShow called" << endl;


}