#pragma once
#include "Page.h"
#include "User.h"
#include "Account.h"
#include "AccountManager.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <vector>
#include <functional>

class UserPage : public Page
{
public:
    UserPage();
    ~UserPage() override;

    // Lifecycle hooks
    void onShow() override;

    // Set the current user
    void setUser(User* user);

protected:
    void buildUI() override;

private:
    // UI Components
    QWidget* containerWidget_;
    QLabel* titleLabel_;

    // Current user
    User* currentUser_;
};