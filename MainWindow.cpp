// Brandon Cotesta | 10/28/2025 | 4:00 PM
// Main application window

#include "MainWindow.h"
#include "LoginPage.h"
#include "RegistrationPage.h"

#include <iostream>

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QtSvg/QSvgRenderer>
#include <QtGui/QIcon>
#include <QtGui/QPainter>


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

// Set the currently logged-in user
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
    
    // Create central widget with vertical layout
    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);  // Set the layout on the widget
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create central stacked widget to hold pages
    stackedWidget_ = new QStackedWidget(mainWidget);
    mainLayout->addWidget(stackedWidget_, 1);  // Add stretch factor so pages take remaining space

    // Navigation bar setup
    setupNavBar();
    mainLayout->addWidget(navBarWidget_);

    setCentralWidget(mainWidget);
    
    // Set stylesheet for mobile app feel
    setStyleSheet(
        "QMainWindow {"
        "   background-color: #f5f5f5;"
        "}"
    );
}

// Setup the menu bar with File and Navigation menus
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
        
        // Hide navigation bar
        navBarWidget_->setVisible(false);
        
        pageManager_->openPage("login");
        updateStackedWidget();
    });
}

void MainWindow::setupPages() {
    // Create and add the login page
    LoginPage* loginPage = new LoginPage();
    
    // Create and add the registration page
    RegistrationPage* registrationPage = new RegistrationPage();
    
    // Create and add the content pages
    dashboardPage_ = new DashboardPage();
	settingsPage_ = new SettingsPage();
    transactionsPage_ = new TransactionsPage();
    userPage_ = new UserPage();
    
    // Set login success callback
    loginPage->setLoginSuccessCallback([this](User* user) {
        this->setCurrentUser(user);
        dashboardPage_->setUser(user);
        cout << "User logged in: " << user->email() << endl;
        
        // Show navigation bar
        navBarWidget_->setVisible(true);
        homeButton_->setChecked(true);
        
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
    pageManager_->addPage("settings", settingsPage_);
    pageManager_->addPage("transactions", transactionsPage_);
    pageManager_->addPage("user", userPage_);

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
    if (Page* page = pageManager_->getPage("settings")) {
        stackedWidget_->addWidget(page->getWidget());
	}
    if (Page* page = pageManager_->getPage("transactions")) {
        stackedWidget_->addWidget(page->getWidget());
    }
    if (Page* page = pageManager_->getPage("user")) {
        stackedWidget_->addWidget(page->getWidget());
    }
    
    // Open the login page initially
    pageManager_->openPage("login");
    updateStackedWidget();
}

void MainWindow::setupNavBar() {
    // Create navigation bar widget
    navBarWidget_ = new QWidget(this);
    navBarWidget_->setFixedHeight(70);
    navBarWidget_->setStyleSheet(
        "QWidget {"
        "   background-color: #ffffff;"
        "   border-top: 1px solid #e0e0e0;"
        "}"
    );

    // Create horizontal layout for nav buttons
    QHBoxLayout* navLayout = new QHBoxLayout(navBarWidget_);
    navLayout->setContentsMargins(0, 5, 0, 5);
    navLayout->setSpacing(0);

    // Button style
    QString buttonStyle =
        "QPushButton {"
        "   background-color: transparent;"
        "   border: none;"
        "   color: #757575;"
        "   font-size: 12px;"
        "   padding: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f0f0f0;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #e0e0e0;"
        "}"
        "QPushButton:checked {"
        "   color: #1976D2;"
        "   font-weight: bold;"
        "}";

    // Create navigation buttons
    homeButton_ = new QPushButton(navBarWidget_);
    homeButton_->setCheckable(true);
    homeButton_->setChecked(true);
    homeButton_->setStyleSheet(buttonStyle);
    homeButton_->setIcon(QIcon("img/128x/homeIcon.png"));
    homeButton_->setIconSize(QSize(36, 36));

    transactionsButton_ = new QPushButton(navBarWidget_);
    transactionsButton_->setCheckable(true);
    transactionsButton_->setStyleSheet(buttonStyle);
    transactionsButton_->setIcon(QIcon("img/128x/transactionIcon.png"));
    transactionsButton_->setIconSize(QSize(40, 40)); // this one is diffy just cause we want it to look a bit similar

    accountButton_ = new QPushButton(navBarWidget_);
    accountButton_->setCheckable(true);
    accountButton_->setStyleSheet(buttonStyle);
    accountButton_->setIcon(QIcon("img/128x/userIcon.png"));
    accountButton_->setIconSize(QSize(36, 36));

    settingsButton_ = new QPushButton(navBarWidget_);
    settingsButton_->setCheckable(true);
    settingsButton_->setStyleSheet(buttonStyle);
    settingsButton_->setIcon(QIcon("img/128x/settingsIcon.png"));
    settingsButton_->setIconSize(QSize(36, 36));

    // Add buttons to layout
    navLayout->addWidget(homeButton_);
    navLayout->addWidget(transactionsButton_);
    navLayout->addWidget(accountButton_);
    navLayout->addWidget(settingsButton_);

    // Connect button signals
    connect(homeButton_, &QPushButton::clicked, this, &MainWindow::onHomeButtonClicked);
    connect(transactionsButton_, &QPushButton::clicked, this, &MainWindow::onTransactionsButtonClicked);
    connect(accountButton_, &QPushButton::clicked, this, &MainWindow::onAccountButtonClicked);
    connect(settingsButton_, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);

    // Initially hide nav bar (show only when logged in)
    navBarWidget_->setVisible(false);
}

void MainWindow::onHomeButtonClicked() {
    // Uncheck other buttons
    transactionsButton_->setChecked(false);
    accountButton_->setChecked(false);
    settingsButton_->setChecked(false);
    homeButton_->setChecked(true);

    // Navigate to dashboard
    pageManager_->openPage("dashboard");
    updateStackedWidget();
}

void MainWindow::onTransactionsButtonClicked() {
    homeButton_->setChecked(false);
    accountButton_->setChecked(false);
    settingsButton_->setChecked(false);
    transactionsButton_->setChecked(true);

    // Navigate to transactions page
    pageManager_->openPage("transactions");
    updateStackedWidget();
}

void MainWindow::onAccountButtonClicked() {
    homeButton_->setChecked(false);
    transactionsButton_->setChecked(false);
    settingsButton_->setChecked(false);
    accountButton_->setChecked(true);

    // Navigate to user page
    pageManager_->openPage("user");
    updateStackedWidget();
}

void MainWindow::onSettingsButtonClicked() {
    homeButton_->setChecked(false);
    transactionsButton_->setChecked(false);
    accountButton_->setChecked(false);
    settingsButton_->setChecked(true);

    // Navigate to settings
    pageManager_->openPage("settings");
    updateStackedWidget();
}

void MainWindow::updateNavBarVisibility() {
    // Show nav bar only when user is logged in
    navBarWidget_->setVisible(currentUser_ != nullptr);
}
void MainWindow::updateStackedWidget() {
    if (Page* currentPage = pageManager_->getCurrentPage()) {
        stackedWidget_->setCurrentWidget(currentPage->getWidget());
    }
}