#pragma once

#include "PageManager.h"
#include "User.h"
#include "DashboardPage.h"

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

private:
    PageManager* pageManager_;
    QStackedWidget* stackedWidget_;
    User* currentUser_;
    DashboardPage* dashboardPage_;
    
    // Navigation bar widgets
    QWidget* navBarWidget_;
    QPushButton* homeButton_;
    QPushButton* transactionsButton_;
    QPushButton* accountButton_;
    QPushButton* settingsButton_;

    void setupUI();
    void setupMenuBar();
    void setupPages();
    void updateStackedWidget();
    void setupNavBar();
    void updateNavBarVisibility();
};