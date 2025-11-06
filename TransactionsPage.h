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
#include <QTableWidget>
#include <QComboBox>
#include <vector>
#include <functional>
#include "Transaction.h"
#include "SessionManager.h"



class TransactionsPage : public Page
{
    //needed for connect statements
    Q_OBJECT

public:
    TransactionsPage();
    ~TransactionsPage() override;
    

    // Lifecycle hooks
    void onShow() override;

    // Set the current user
    void setUser(User* user);

    //table functions
    void transactiontablesetup();
    void transactionTableRefresh(string acc);

    //account type check
    void accountDropSet();


protected:
    void buildUI() override;

private slots:
    //when drop down is changed this is what detects it
    void onAccountChanged(int index);

private:
    // UI Components
    QWidget* containerWidget_;
    QLabel* titleLabel_;
    QLabel* accountLabel_;
    // Current user
    User* currentUser_;

    // dropdown for account selection
    QComboBox *accountsDrop_;
    QStringList *accounts_;
    QString accountName_;
    QString accountID_;



    //table
    QTableWidget *transTable_;


};