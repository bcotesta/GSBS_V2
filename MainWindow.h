#pragma once

#include "PageManager.h"
#include "User.h"
#include "DashboardPage.h"

#include <QMainWindow>
#include <QStackedWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(PageManager* pageManager, QWidget* parent = nullptr);
    ~MainWindow();
    
    void setCurrentUser(User* user);

private:
    PageManager* pageManager_;
    QStackedWidget* stackedWidget_;
    User* currentUser_;
    DashboardPage* dashboardPage_;
    
    void setupUI();
    void setupMenuBar();
    void setupPages();
    void updateStackedWidget();
};