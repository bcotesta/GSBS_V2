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

    scrollArea_(nullptr),
    scrollContent_(nullptr),
    contentLayout_(nullptr),
    headerWidget_(nullptr),
    titleLabel_(nullptr),
    currentUser_(nullptr)
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
    newAccountLabel_ = new QLabel("Select account:", containerWidget_);
    QFont newAccountFont("Segoe UI", 11, QFont::Bold);
    newAccountLabel_->setFont(newAccountFont);
    newAccountLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    containerLayout->addWidget(newAccountLabel_);




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



void OpenAccountPage::onShow() {
    cout << "OpenAccountPage::onShow called" << endl;


}