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

    titleLabel_ = new QLabel("Open Accounts", headerWidget_);
    QFont titleFont("Segoe UI", 24, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    headerLayout->addWidget(titleLabel_);
    headerLayout->addStretch();

    contentLayout_->addWidget(headerWidget_);
    contentLayout_->addSpacing(15);


    QPushButton* card = new QPushButton(scrollContent_);
    card->setCursor(Qt::PointingHandCursor);
    card->setFixedHeight(160);

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
    QLabel* accountTypeLabel = new QLabel("Savings", contentWidget);
    QFont typeFont("Segoe UI", 13, QFont::DemiBold);
    accountTypeLabel->setFont(typeFont);
    accountTypeLabel->setStyleSheet("QLabel { color: #2c3e50; }");
    cardLayout->addWidget(accountTypeLabel);





    scrollArea_->setWidget(scrollContent_);

    mainLayout_-> addWidget(scrollArea_);

}

QWidget* OpenAccountPage::createSectionHeader(const QString& title) {
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


void OpenAccountPage::onShow() {
    cout << "OpenAccountPage::onShow called" << endl;


}