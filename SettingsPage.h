#pragma once
#include "Page.h"
#include "User.h"
#include "Account.h"
#include "PageManager.h"
#include "AccountManager.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <vector>
#include <functional>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTableWidget>

class SettingsPage : public Page
{
    Q_OBJECT
public:
    SettingsPage();
    ~SettingsPage() override;

    // Lifecycle hooks
    void onShow() override;

    // Set the current user
    void setUser(User* user);
    

protected:
    void buildUI() override;
 
private slots:
    // Card click handlers
    void onProfileClicked();
    void onProductsClicked();
    void onAppClicked();
    void onContactClicked();
    void onPrivacyClicked();
    void onSignOutClicked();
    void onFAQClicked();

    // Overlay navigation
    void showOverlay(int overlayIndex);
    void hideOverlay();

    // Action handlers
    void handleProfileSend();
    void handleSignOut();


private:
    // UI Components
    
    QPushButton* productsButton_;
    QPushButton* learnAppButton_;
    QPushButton* contactButton_;
    QPushButton* privacyButton_;
    QPushButton* signOutButton_;
    PageManager* pageManager_;
    QPushButton* faqButton_;
    // Helper function to create action cards
    QPushButton* createActionCard(const QString& title, const QString& iconPath, QWidget* parent);

    // Overlay creation functions
    QWidget* createProfileOverlay();
    QWidget* createProductsOverlay();
    QWidget* createAppOverlay();
    QWidget* createContactOverlay();
    QWidget* createPrivacyOverlay();
    QWidget* createSignOutPopUp();
    QWidget* createFAQOverlay();

    // Helper for creating overlay header with back button
    QWidget* createOverlayHeader(const QString& title, QWidget* parent);
    // Load user accounts using AccountManager
    void loadUserAccounts();

    // UI Components
    QStackedWidget* stackedWidget_;
    QWidget* mainMenuWidget_;
    QWidget* containerWidget_;
    QLabel* titleLabel_;

    // Overlay widgets
    QWidget* ProfileOverlay_;
    QWidget* ProductsOverlay_;
    QWidget* AppOverlay_;
    QWidget* ContactOverlay_;
    QWidget* PrivacyOverlay_;
    QWidget* SignOutOverlay_;
    QWidget* FAQOverlay_;

    // Current user
    User* currentUser_;
    std::vector<Account> accounts_;

    QLabel* changeLabel;
    QComboBox* fieldSelect_;
    QLineEdit* fieldInput_;
    // Overlay indices
    enum OverlayIndex {
		MAIN_MENU = 0,
        PROFILE = 1,
        PRODUCTS = 2,
        APP = 3,
        CONTACT = 4,
        PRIVACY = 5,
        FAQ=6,
        SIGNOUT=7
    };
};