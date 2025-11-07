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
    
        

}



void OpenAccountPage::onShow() {
    cout << "OpenAccountPage::onShow called" << endl;


}