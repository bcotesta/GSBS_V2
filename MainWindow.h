#pragma once

#include "PageManager.h"
#include "User.h"
#include "DashboardPage.h"
#include "SettingsPage.h"
#include "TransactionsPage.h"
#include "UserPage.h"
#include "OpenAccountPage.h"

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
    void onAccountButtonClicked();
    void onSettingsButtonClicked();
    void onOpenAccountButtonClicked();

private:
    PageManager* pageManager_;
    QStackedWidget* stackedWidget_;
    User* currentUser_;
    DashboardPage* dashboardPage_;
	SettingsPage* settingsPage_;
    TransactionsPage* transactionsPage_;
    UserPage* userPage_;
    OpenAccountPage* openaccountPage_;
    
    // Navigation bar widgets
    QWidget* navBarWidget_;
    QPushButton* homeButton_;
    QPushButton* transactionsButton_;
    QPushButton* accountButton_;
    QPushButton* settingsButton_;
    QPushButton* openaccountButton_;

    void setupUI();
	void setUserForAllPages(User* user);
    void setupMenuBar();
    void setupPages();
    void updateStackedWidget();
    void setupNavBar();
    void updateNavBarVisibility();
};