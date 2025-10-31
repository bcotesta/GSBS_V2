// Brandon Cotesta || 10/28/2025 | 3:00 PM
// Transactions page for displaying user transactions

#include "TransactionsPage.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QScrollArea>
#include <iomanip>
#include <sstream>

TransactionsPage::TransactionsPage()
    : Page("transactions"),

    containerWidget_(nullptr),
    currentUser_(nullptr),
    accounts_(nullptr),
    accountLabel_(nullptr),
    accountsDrop(nullptr)

{

}

TransactionsPage::~TransactionsPage() {
    // Qt's parent-child system handles cleanup
}

void TransactionsPage::setUser(User* user) {
    currentUser_ = user;
}

void TransactionsPage::buildUI() {
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
    titleLabel_ = new QLabel("Transactions", containerWidget_);
    QFont titleFont("Segoe UI", 28, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    titleLabel_->setMinimumHeight(45);
    containerLayout->addWidget(titleLabel_);

    
    //Account dropdown label
    accountLabel_ = new QLabel("Select account:", containerWidget_);
    QFont accountFont("Segoe UI", 11, QFont::Bold);
    accountLabel_->setFont(accountFont);
    accountLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    containerLayout->addWidget(accountLabel_);

    //create combo box for the dropdown
    accountsDrop = new QComboBox(containerWidget_);
    //the options to select from. the first string is the account name / what is displayed
    //and the second string is the id
    accountsDrop->addItem("Chequing", QVariant("Chequing"));
    accountsDrop->addItem("Savings", QVariant("Savings"));
    accountsDrop->addItem("Credit", QVariant("Credit"));
    accountsDrop->addItem("Loan", QVariant("Loan"));
    containerLayout->addWidget(accountsDrop);
    //connects button/drop down to a function.
    connect(accountsDrop, &QComboBox::currentIndexChanged, this, &TransactionsPage::onAccountChanged);
    



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

void TransactionsPage::onShow() {
	cout << "TransactionsPage::onShow called" << endl;
}


//Detects when drop down selection is changed
// no info currently displayed
//currently doesn't change the info displayed
void TransactionsPage::onAccountChanged(int index) {

    QString accountName = accountsDrop->itemText(index);
    QString accountId = accountsDrop->itemData(index).toString();

    qDebug() << "Account changed to:" << accountName << "(" << accountId << ")";

}