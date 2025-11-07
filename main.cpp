// Brandon Cotesta | 10/27/2025 | 8:43 PM
// MOOD: Neutral. not excited to spend the night doing this
// but ready to get it done. Blue Jays are down 1-0 >:(.

// VERY IMPORTANT NOTE - ALL PAGE STUFF MUST BE DONE IN MAINWINDOW.CPP
// THAT IS WHERE YOU ADD THE RESPECTIVE PAGES TO BE USED IN THE APP

#include "SessionManager.h"
#include "AccountManager.h"
#include "UIManager.h"
#include "PageManager.h"
#include "BankingOperationsManager.h"
#include "MainWindow.h"
#include "User.h"
#include "TwilioService.h"

#include <QApplication>
#include <QFont>
#include <vector>
#include <iostream>

using namespace std;

// ==================== TWILIO INITIALIZATION ====================

static void initializeTwilio() {
    TwilioService& twilioSvc = TwilioService::getInstance();
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    
    // Pull credentials from database
    std::string accountSid = dbManager.getSystemConfig("TWILIO_ACCOUNT_SID");
    std::string authToken = dbManager.getSystemConfig("TWILIO_AUTH_TOKEN");
    std::string fromNumber = dbManager.getSystemConfig("TWILIO_FROM_NUMBER");
    
    // Verify credentials were retrieved
    if (accountSid.empty() || authToken.empty() || fromNumber.empty()) {
        std::cout << "[MAIN] WARNING: Twilio credentials not found in database" << std::endl;
        std::cout << "[MAIN] Please insert credentials into system_config table" << std::endl;
        return;
    }
    
    twilioSvc.configure(accountSid, authToken, fromNumber);
    std::cout << "[MAIN] Twilio SMS service initialized from database" << std::endl;
}

// ==================== QT WINDOW INITIALIZATION ====================

static void initializeQtWindow(PageManager& pageManager, QApplication& app) {
    // Set application style
    app.setStyle("Fusion");
    
    // Set application-wide font
    QFont appFont("Segoe UI", 10);
    app.setFont(appFont);

    // Create and show main window
    MainWindow mainWindow(&pageManager);
    mainWindow.show();

    // Run application event loop
    app.exec();
}

// ==================== MAIN ENTRY POINT ====================

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // initialize Twilio service (uses Qt Network)
    initializeTwilio();
    
    // Set application style
    app.setStyle("Fusion");

    // Set application-wide font
    QFont appFont("Segoe UI", 10);
    app.setFont(appFont);

    // Create page manager - MUST outlive mainWindow
    PageManager pageManager;

    // Create and show main window
    MainWindow mainWindow(&pageManager);
    mainWindow.show();

    // Run application event loop
    return app.exec();
    
}

