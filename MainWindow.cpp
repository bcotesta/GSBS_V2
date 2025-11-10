// Brandon Cotesta | 10/28/2025 | 4:00 PM
// Main application window

#include "MainWindow.h"
#include "LoginPage.h"
#include "RegistrationPage.h"
#include "OTPPage.h" 
#include "DatabaseManager.h"  // For 2FA database operations
#include <iostream>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(PageManager* pageManager, QWidget* parent)
    : QMainWindow(parent), 
      pageManager_(pageManager), 
      currentUser_(nullptr), 
      dashboardPage_(nullptr),
      pendingUser_(nullptr) 
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
    if (pendingUser_) {
        delete pendingUser_;
        pendingUser_ = nullptr;
    }
}

// Set the currently logged-in user
void MainWindow::setCurrentUser(User* user) {
    if (currentUser_) {
        delete currentUser_;
    }
    currentUser_ = user;
}

// This could be done better with a loop and a list of pages, but for now this is fine
// when you make your own pages just add them here
void MainWindow::setUserForAllPages(User* user) {
    if (dashboardPage_) {
        dashboardPage_->setUser(user);
    }
    if (settingsPage_) {
        settingsPage_->setUser(user);
    }
    if (transactionsPage_) {
        transactionsPage_->setUser(user);
    }
    if (userPage_) {
        userPage_->setUser(user);
    }
    if (moveMoneyPage_) {
		moveMoneyPage_->setUser(user);
    }
    if (openaccountPage_) {
        openaccountPage_->setUser(user);
    }
}

void MainWindow::setupUI() {
    setWindowTitle("GSBS Banking System");
    
    // Mobile-like dimensions (portrait orientation)
    setFixedSize(450, 800);
    
    // Create central widget with vertical layout
    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create central stacked widget to hold pages
    stackedWidget_ = new QStackedWidget(mainWidget);
    mainLayout->addWidget(stackedWidget_, 1);

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
    Authenticator& auth = Authenticator::getInstance();
    
    // Create and add the login page
    LoginPage* loginPage = new LoginPage();
    
    // Create and add the registration page
    RegistrationPage* registrationPage = new RegistrationPage();
    
    // CREATE OTP PAGE
    otpPage_ = new OTPPage();
    
    // Create and add the content pages
    dashboardPage_ = new DashboardPage();
	settingsPage_ = new SettingsPage();
    transactionsPage_ = new TransactionsPage();
    userPage_ = new UserPage();
	moveMoneyPage_ = new MoveMoneyPage();
    openaccountPage_ = new OpenAccountPage();
    
    // Set login success callback - MODIFIED FOR 2FA
    loginPage->setLoginSuccessCallback([this, &auth](User* user) {
        std::string userID = user->userId() == 0 ? 
                            auth.getUserID() : 
                            std::to_string(user->userId());
        
        // Check if 2FA is enabled
        if (auth.isTwoFactorEnabled(userID)) {
            // Store user temporarily
            pendingUser_ = user;
            
            // Get 2FA method
            std::string method = auth.getTwoFactorMethod(userID);
            
            // Configure OTP page BEFORE navigating to it
            // This ensures the data is set before onShow() is called
            otpPage_->setUserID(userID);
            otpPage_->setDeliveryMethod(method);
            
            pageManager_->openPage("otp");
            updateStackedWidget();
            std::cout << "2FA required for user: " << user->email() << endl;
        } else {
            // No 2FA - prompt to enable it
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Enable Two-Factor Authentication");
            msgBox.setText("Two-factor authentication is not enabled on your account.");
            msgBox.setInformativeText("Would you like to enable it now for better security?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            msgBox.setIcon(QMessageBox::Question);
            
            int choice = msgBox.exec();
            
            if (choice == QMessageBox::Yes) {
                // Enable 2FA in database
                DatabaseManager& dbManager = DatabaseManager::getInstance();
                try {
                    std::string setClause = "twoFactorEnabled = '1', twoFactorMethod = 'phone'";
                    std::string whereClause = "userID = '" + userID + "'";
                    dbManager.updateTable("userinfo", setClause, whereClause);

                    QMessageBox::information(this, "Success",
                        "Two-factor authentication has been enabled successfully!");
                }
                catch (const std::exception& e) {
                    QMessageBox::warning(this, "Error",
                        "Failed to enable 2FA. You can enable it later in settings.");
                }
                
            }
            
            // Complete login
            this->setCurrentUser(user);
            dashboardPage_->setUser(user);
            std::cout << "User logged in: " << user->email() << std::endl;

            // Show navigation bar
            navBarWidget_->setVisible(true);
            homeButton_->setChecked(true);
            
            // Navigate to dashboard page
            pageManager_->openPage("dashboard");
            updateStackedWidget();
        }
    });
    
    // Set register callback to navigate to registration page
    loginPage->setRegisterCallback([this]() {
        pageManager_->openPage("register");
        updateStackedWidget();
    });
    
    // Set registration success callback - MODIFIED FOR 2FA
    registrationPage->setRegistrationSuccessCallback([this, &auth](User* user) {
        std::string userID = user->userId() == 0 ? 
                            auth.getUserID() : 
                            std::to_string(user->userId());
        
        // Always prompt to enable 2FA after registration
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Secure Your Account");
        msgBox.setText("Would you like to enable two-factor authentication?");
        msgBox.setInformativeText("This adds an extra layer of security to your account. Highly recommended!");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setIcon(QMessageBox::Question);
        
        int choice = msgBox.exec();
        
        if (choice == QMessageBox::Yes) {
            // Enable 2FA in database
            DatabaseManager& dbManager = DatabaseManager::getInstance();
            try {
                std::string setClause = "twoFactorEnabled = '1', twoFactorMethod = 'phone'";
                std::string whereClause = "userID = '" + userID + "'";
                dbManager.updateTable("userinfo", setClause, whereClause);

                QMessageBox::information(this, "Success",
                    "Two-factor authentication has been enabled successfully!");
            }
            catch (const std::exception& e) {
                QMessageBox::warning(this, "Error",
                    "Failed to enable 2FA. You can enable it later in settings.");
            }
        }
        
        // Complete registration
        this->setCurrentUser(user);
        dashboardPage_->setUser(user);
        std::cout << "User registered: " << user->email() << std::endl;

        // Show navigation bar
        navBarWidget_->setVisible(true);
        homeButton_->setChecked(true);
        
        // Navigate to dashboard page
        pageManager_->openPage("dashboard");
        updateStackedWidget();
    });
    
    // Set back to login callback
    registrationPage->setBackToLoginCallback([this]() {
        pageManager_->openPage("login");
        updateStackedWidget();
    });
    
    // SET OTP PAGE CALLBACKS
    otpPage_->setVerificationSuccessCallback([this]() {
        // OTP verified successfully - complete login
        if (pendingUser_) {
            this->setCurrentUser(pendingUser_);
            dashboardPage_->setUser(pendingUser_);
            std::cout << "User logged in with 2FA: " << pendingUser_->email() << std::endl;

            pendingUser_ = nullptr;  // Clear pending user
            
            // Show navigation bar
            navBarWidget_->setVisible(true);
            homeButton_->setChecked(true);
            
            // Navigate to dashboard
            pageManager_->openPage("dashboard");
            updateStackedWidget();
        }
    });
    
    otpPage_->setVerificationFailedCallback([this]() {
        // OTP verification failed - stay on OTP page
        std::cout << "OTP verification failed" << std::endl;
    });
    
    otpPage_->setCancelCallback([this]() {
        // User cancelled OTP - go back to login
        if (pendingUser_) {
            delete pendingUser_;
            pendingUser_ = nullptr;
        }
        
        pageManager_->openPage("login");
        updateStackedWidget();
    });
    
    // Add pages to page manager
    pageManager_->addPage("login", loginPage);
    pageManager_->addPage("register", registrationPage);
    pageManager_->addPage("otp", otpPage_);
    pageManager_->addPage("dashboard", dashboardPage_);
    pageManager_->addPage("settings", settingsPage_);
    pageManager_->addPage("transactions", transactionsPage_);
    pageManager_->addPage("user", userPage_);
	pageManager_->addPage("move_money", moveMoneyPage_);
    pageManager_->addPage("openaccount", openaccountPage_);

    // Add page widgets to stacked widget
    if (Page* page = pageManager_->getPage("login")) {
        stackedWidget_->addWidget(page->getWidget());
    }
    if (Page* page = pageManager_->getPage("register")) {
        stackedWidget_->addWidget(page->getWidget());
    }
    if (Page* page = pageManager_->getPage("otp")) {
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
    if (Page* page = pageManager_->getPage("move_money")) {
        stackedWidget_->addWidget(page->getWidget());
	}
    if (Page* page = pageManager_->getPage("openaccount")) {
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
    transactionsButton_->setIconSize(QSize(40, 40));

    moveMoneyButton_ = new QPushButton(navBarWidget_);
    moveMoneyButton_->setCheckable(true);
    moveMoneyButton_->setStyleSheet(buttonStyle);
    moveMoneyButton_->setIcon(QIcon("img/128x/data-transfer.png"));
    moveMoneyButton_->setIconSize(QSize(36, 36));

    settingsButton_ = new QPushButton(navBarWidget_);
    settingsButton_->setCheckable(true);
    settingsButton_->setStyleSheet(buttonStyle);
    settingsButton_->setIcon(QIcon("img/128x/settingsIcon.png"));
    settingsButton_->setIconSize(QSize(36, 36));

    openaccountButton_ = new QPushButton(navBarWidget_);
    openaccountButton_->setCheckable(true);
    openaccountButton_->setStyleSheet(buttonStyle);
    openaccountButton_->setIcon(QIcon("img/128x/newaccountIcon.png"));
    openaccountButton_->setIconSize(QSize(36, 36));

    // Add buttons to layout
    navLayout->addWidget(homeButton_);
    navLayout->addWidget(transactionsButton_);
    navLayout->addWidget(moveMoneyButton_);
    navLayout->addWidget(openaccountButton_);
    navLayout->addWidget(settingsButton_);

    // Connect button signals
    connect(homeButton_, &QPushButton::clicked, this, &MainWindow::onHomeButtonClicked);
    connect(transactionsButton_, &QPushButton::clicked, this, &MainWindow::onTransactionsButtonClicked);
    connect(moveMoneyButton_, &QPushButton::clicked, this, &MainWindow::onMoveMoneyButtonClicked);
    connect(openaccountButton_, &QPushButton::clicked, this, &MainWindow::onOpenAccountButtonClicked);
    connect(settingsButton_, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);

    // Initially hide nav bar (show only when logged in)
    navBarWidget_->setVisible(false);
}

void MainWindow::onHomeButtonClicked() {
    transactionsButton_->setChecked(false);
    moveMoneyButton_->setChecked(false);
    settingsButton_->setChecked(false);
    homeButton_->setChecked(true);

    pageManager_->openPage("dashboard");
    updateStackedWidget();
}

void MainWindow::onTransactionsButtonClicked() {
    homeButton_->setChecked(false);
    moveMoneyButton_->setChecked(false);
    openaccountButton_->setChecked(false);
    settingsButton_->setChecked(false);
    transactionsButton_->setChecked(true);

	transactionsPage_->setUser(currentUser_);

    pageManager_->openPage("transactions");
    updateStackedWidget();
}

void MainWindow::onMoveMoneyButtonClicked() {
    homeButton_->setChecked(false);
    transactionsButton_->setChecked(false);
    openaccountButton_->setChecked(false);
    settingsButton_->setChecked(false);
    moveMoneyButton_->setChecked(true);

    moveMoneyPage_->setUser(currentUser_);

    pageManager_->openPage("move_money");
    updateStackedWidget();
}

void MainWindow::onOpenAccountButtonClicked() {
    homeButton_->setChecked(false);
    transactionsButton_->setChecked(false);
    openaccountButton_->setChecked(true);
    settingsButton_->setChecked(false);
    moveMoneyButton_->setChecked(false);

    openaccountPage_->setUser(currentUser_);

    pageManager_->openPage("openaccount");
    updateStackedWidget();
}

void MainWindow::onSettingsButtonClicked() {
    homeButton_->setChecked(false);
    transactionsButton_->setChecked(false);
    moveMoneyButton_->setChecked(false);
    openaccountButton_->setChecked(false);
    settingsButton_->setChecked(true);

    pageManager_->openPage("settings");
    updateStackedWidget();
}

void MainWindow::updateNavBarVisibility() {
    navBarWidget_->setVisible(currentUser_ != nullptr);
}

void MainWindow::updateStackedWidget() {
    if (Page* currentPage = pageManager_->getCurrentPage()) {
        stackedWidget_->setCurrentWidget(currentPage->getWidget());
    }
}