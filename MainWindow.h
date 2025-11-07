#pragma once

#include "PageManager.h"
#include "User.h"
#include "DashboardPage.h"
#include "SettingsPage.h"
#include "TransactionsPage.h"
#include "UserPage.h"
#include "OTPPage.h"
#include "MoveMoneyPage.h"

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(PageManager* pageManager, QWidget* parent = nullptr);
    ~MainWindow();
    
    void setCurrentUser(User* user);

private slots:
    void onHomeButtonClicked();
    void onTransactionsButtonClicked();
    void onMoveMoneyButtonClicked();
    void onSettingsButtonClicked();

private:
    PageManager* pageManager_;
    QStackedWidget* stackedWidget_;
    User* currentUser_;
    DashboardPage* dashboardPage_;
	SettingsPage* settingsPage_;
    TransactionsPage* transactionsPage_;
    UserPage* userPage_;
    OTPPage* otpPage_;
	MoveMoneyPage* moveMoneyPage_;

    // Temporary storage for user during OTP verification
    User* pendingUser_; 
    
    // Navigation bar widgets
    QWidget* navBarWidget_;
    QPushButton* homeButton_;
    QPushButton* transactionsButton_;
    QPushButton* moveMoneyButton_;
    QPushButton* settingsButton_;

    void setupUI();
	void setUserForAllPages(User* user);
    void setupMenuBar();
    void setupPages();
    void updateStackedWidget();
    void setupNavBar();
    void updateNavBarVisibility();
};