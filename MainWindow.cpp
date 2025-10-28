// Brandon Cotesta | 10/28/2025 | 4:00 PM
// Main application window

#include "MainWindow.h"
#include "LoginPage.h"
#include "RegistrationPage.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <iostream>

using namespace std;

MainWindow::MainWindow(PageManager* pageManager, QWidget* parent)
    : QMainWindow(parent), pageManager_(pageManager), currentUser_(nullptr), dashboardPage_(nullptr)
{
    setupUI();
    setupPages();
    setupMenuBar();
}

MainWindow::~MainWindow() {
    if (currentUser_) {
        delete currentUser_;
        currentUser_ = nullptr;
    }
}

void MainWindow::setCurrentUser(User* user) {
    if (currentUser_) {
        delete currentUser_;
    }
    currentUser_ = user;
}

void MainWindow::setupUI() {
    setWindowTitle("GSBS Banking System");
    
    // Mobile-like dimensions (portrait orientation)
    setFixedSize(450, 800);
    
    // Create central stacked widget to hold pages
    stackedWidget_ = new QStackedWidget(this);
    setCentralWidget(stackedWidget_);
    
    // Set stylesheet for mobile app feel
    setStyleSheet(
        "QMainWindow {"
        "   background-color: #f5f5f5;"
        "}"
    );
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // File menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    
    QAction* logoutAction = new QAction("&Logout", this);
    QAction* exitAction = new QAction("E&xit", this);
    
    fileMenu->addAction(logoutAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    // Connect actions
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    connect(logoutAction, &QAction::triggered, [this]() {
        if (currentUser_) {
            delete currentUser_;
            currentUser_ = nullptr;
        }
        pageManager_->openPage("login");
        updateStackedWidget();
    });

    // Navigation menu
    QMenu* navMenu = menuBar->addMenu("&Navigation");
    
    QAction* backAction = new QAction("&Back", this);
    backAction->setShortcut(QKeySequence("Alt+Left"));
    navMenu->addAction(backAction);

    connect(backAction, &QAction::triggered, [this]() {
        if (pageManager_->canGoBack()) {
            pageManager_->goBack();
            updateStackedWidget();
        }
    });
}

void MainWindow::setupPages() {
    // Create and add the login page
    LoginPage* loginPage = new LoginPage();
    
    // Create and add the registration page
    RegistrationPage* registrationPage = new RegistrationPage();
    
    // Create and add the dashboard page
    dashboardPage_ = new DashboardPage();
    
    // Set login success callback
    loginPage->setLoginSuccessCallback([this](User* user) {
        this->setCurrentUser(user);
        dashboardPage_->setUser(user);
        cout << "User logged in: " << user->email() << endl;
        
        // Navigate to dashboard page
        pageManager_->openPage("dashboard");
        updateStackedWidget();
    });
    
    // Set register callback to navigate to registration page
    loginPage->setRegisterCallback([this]() {
        pageManager_->openPage("register");
        updateStackedWidget();
    });
    
    // Set registration success callback
    registrationPage->setRegistrationSuccessCallback([this](User* user) {
        this->setCurrentUser(user);
        dashboardPage_->setUser(user);
        cout << "User registered: " << user->email() << endl;
        
        // Navigate to dashboard page
        pageManager_->openPage("dashboard");
        updateStackedWidget();
    });
    
    // Set back to login callback
    registrationPage->setBackToLoginCallback([this]() {
        pageManager_->openPage("login");
        updateStackedWidget();
    });
    
    // Add pages to page manager
    pageManager_->addPage("login", loginPage);
    pageManager_->addPage("register", registrationPage);
    pageManager_->addPage("dashboard", dashboardPage_);

    // Add page widgets to stacked widget
    if (Page* page = pageManager_->getPage("login")) {
        stackedWidget_->addWidget(page->getWidget());
    }
    if (Page* page = pageManager_->getPage("register")) {
        stackedWidget_->addWidget(page->getWidget());
    }
    if (Page* page = pageManager_->getPage("dashboard")) {
        stackedWidget_->addWidget(page->getWidget());
    }
    
    // Open the login page initially
    pageManager_->openPage("login");
    updateStackedWidget();
}

void MainWindow::updateStackedWidget() {
    if (Page* currentPage = pageManager_->getCurrentPage()) {
        stackedWidget_->setCurrentWidget(currentPage->getWidget());
    }
}