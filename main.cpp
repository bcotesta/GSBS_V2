// Brandon Cotesta | 10/27/2025 | 8:43 PM
// MOOD: Neutral. not excited to spend the night doing this
// but ready to get it done. Blue Jays are down 1-0 >:(.

#include "SessionManager.h"
#include "AccountManager.h"
#include "UIManager.h"
#include "PageManager.h"
#include "BankingOperationsManager.h"
#include "LoginPage.h"
#include "RegistrationPage.h"
#include "User.h"

#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include <iostream>

using namespace std;

// Forward declarations
class MainWindow;

// ==================== QT WINDOW SETUP ====================

class MainWindow : public QMainWindow {
public:
    MainWindow(PageManager* pageManager, QWidget* parent = nullptr)
        : QMainWindow(parent), pageManager_(pageManager), currentUser_(nullptr)
    {
        setupUI();
        setupPages();
        setupMenuBar();
    }

    ~MainWindow() {
        if (currentUser_) {
            delete currentUser_;
            currentUser_ = nullptr;
        }
    }

    void setCurrentUser(User* user) {
        if (currentUser_) {
            delete currentUser_;
        }
        currentUser_ = user;
    }

private:
    PageManager* pageManager_;
    QStackedWidget* stackedWidget_;
    User* currentUser_;

    void setupUI() {
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

    void setupMenuBar() {
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

    void setupPages() {
        // Create and add the login page
        LoginPage* loginPage = new LoginPage();
        
        // Create and add the registration page
        RegistrationPage* registrationPage = new RegistrationPage();
        
        // Set login success callback
        loginPage->setLoginSuccessCallback([this](User* user) {
            this->setCurrentUser(user);
            cout << "User logged in: " << user->email() << endl;
            
            // TODO: Navigate to dashboard page
            // For now, just show a message
            cout << "Login successful! Dashboard page not yet implemented." << endl;
        });
        
        // Set register callback to navigate to registration page
        loginPage->setRegisterCallback([this]() {
            pageManager_->openPage("register");
            updateStackedWidget();
        });
        
        // Set registration success callback
        registrationPage->setRegistrationSuccessCallback([this](User* user) {
            this->setCurrentUser(user);
            cout << "User registered: " << user->email() << endl;
            
            // TODO: Navigate to dashboard page
            // For now, just show a message
            cout << "Registration successful! Dashboard page not yet implemented." << endl;
        });
        
        // Set back to login callback
        registrationPage->setBackToLoginCallback([this]() {
            pageManager_->openPage("login");
            updateStackedWidget();
        });
        
        // Add pages to page manager
        pageManager_->addPage("login", loginPage);
        pageManager_->addPage("register", registrationPage);

        // Add page widgets to stacked widget
        if (Page* page = pageManager_->getPage("login")) {
            stackedWidget_->addWidget(page->getWidget());
        }
        if (Page* page = pageManager_->getPage("register")) {
            stackedWidget_->addWidget(page->getWidget());
        }

        // TODO: Add more pages here (e.g., DashboardPage, AccountsPage, etc.)
        
        // Open the login page initially
        pageManager_->openPage("login");
        updateStackedWidget();
    }
    
    void updateStackedWidget() {
        if (Page* currentPage = pageManager_->getCurrentPage()) {
            stackedWidget_->setCurrentWidget(currentPage->getWidget());
        }
    }
};

// ==================== QT WINDOW INITIALIZATION ====================

static void initializeQtWindow(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Set application style
    app.setStyle("Fusion");
    
    // Set application-wide font
    QFont appFont("Segoe UI", 10);
    app.setFont(appFont);

    // Create page manager - MUST outlive mainWindow
    PageManager pageManager;

    // Create and show main window - use scope or pointer to control lifetime
    {
        MainWindow mainWindow(&pageManager);
        mainWindow.show();

        // Run application event loop
        app.exec();
    } // mainWindow destructs here BEFORE pageManager
}

// ==================== CONSOLE-BASED BANKING APP (Legacy) ====================

static void displayMainMenu() {
    cout << "\n========================================" << endl;
    cout << "           BANKING MAIN MENU" << endl;
    cout << "========================================" << endl;
    cout << "1. View All Accounts" << endl;
    cout << "2. Deposit Funds" << endl;
    cout << "3. Withdraw Funds" << endl;
    cout << "4. Transfer Funds" << endl;
    cout << "5. Create New Account" << endl;
    cout << "6. View Account Summary" << endl;
    cout << "7. Logout" << endl;
    cout << "========================================" << endl;
    cout << "Enter choice: ";
}

static void runBankingApp() {
    SessionManager session;
    
    // Offer login or registration
    cout << "\n=== Welcome to GSBS Banking System ===" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register New Account" << endl;
    cout << "Enter choice (1-2): ";
    
    int choice;
    cin >> choice;
    cin.ignore(); // Clear newline from buffer
    
    bool success = false;
    
    if (choice == 1) {
        // Existing login
        success = session.login();
        if (!success) {
            UIManager::showError("Login failed. Exiting...");
            return;
        }
    }
    else if (choice == 2) {
        // New registration
        success = session.registerUser();
        if (!success) {
            UIManager::showError("Registration failed. Exiting...");
            return;
        }
    }
    else {
        UIManager::showError("Invalid choice. Exiting...");
        return;
    }
    
    User* user = session.getCurrentUser();
    UIManager::displayUserInfo(*user);
    
    // Step 2: Initialize account manager
    AccountManager accountMgr(*user);
    
    // Step 3: Load existing accounts
    vector<Account> accounts = accountMgr.loadUserAccounts();
    
    // Step 4: Initialize banking operations manager
    BankingOperationsManager bankingOps(accountMgr, accounts);
    
    // Main menu loop
    bool running = true;
    while (running) {
        displayMainMenu();
        
        int menuChoice;
        cin >> menuChoice;
        cin.ignore(); // Clear newline from buffer
        
        switch (menuChoice) {
            case 1: // View All Accounts
                bankingOps.handleViewAccounts();
                break;
                
            case 2: // Deposit
                bankingOps.handleDeposit();
                break;
                
            case 3: // Withdraw
                bankingOps.handleWithdraw();
                break;
                
            case 4: // Transfer
                bankingOps.handleTransfer();
                break;
                
            case 5: // Create Account
                bankingOps.handleCreateAccount();
                break;
                
            case 6: // View Summary
                bankingOps.handleViewSummary();
                break;
                
            case 7: // Logout
                UIManager::showInfo("Logging out...");
                session.logout();
                running = false;
                break;
                
            default:
                UIManager::showError("Invalid choice. Please try again.");
                break;
        }
    }
    
    UIManager::showSuccess("Thank you for using GSBS Banking System!");
}

// ==================== MAIN ENTRY POINT ====================

int main(int argc, char *argv[]) {
    // Choose between Qt GUI or console mode
    cout << "=== GSBS Banking System ===" << endl;
    cout << "Select mode:" << endl;
    cout << "1. GUI Mode (Mobile-like Interface)" << endl;
    cout << "2. Console Mode (Legacy)" << endl;
    cout << "Enter choice (1-2): ";
    
    int modeChoice;
    cin >> modeChoice;
    cin.ignore();
    
    if (modeChoice == 1) {
        // Launch Qt GUI
        initializeQtWindow(argc, argv);
    } else {
        // Launch console app
        QApplication app(argc, argv); // Still needed for Qt initialization
        UIManager::displayWelcome();
        runBankingApp();
    }
    
    return 0;
}

