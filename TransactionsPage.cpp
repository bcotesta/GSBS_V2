// Brandon Cotesta || 10/28/2025 | 3:00 PM
// Transactions page for displaying user transactions

#include "TransactionsPage.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QScrollArea>
#include <QLineEdit>
#include <iomanip>
#include <sstream>

TransactionsPage::TransactionsPage()
    : Page("transactions"),

    containerWidget_(nullptr),
    titleLabel_(nullptr),
    currentUser_(nullptr),
    accounts_(nullptr),
    accountLabel_(nullptr),
    accountsDrop_(nullptr),
    transTable_(nullptr)
{

}

TransactionsPage::~TransactionsPage() {
    // Qt's parent-child system handles cleanup
}



void TransactionsPage::setUser(User* user) {
    currentUser_ = user;
}

void TransactionsPage::buildUI() {
    QWidget* centralWidget = getCentralWidget();

    // Set background color for the page
    centralWidget->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    // Create a centered container
    containerWidget_ = new QWidget(centralWidget);
    containerWidget_->setFixedWidth(400);
    containerWidget_->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   color: black;"
        "   border-radius: 12px;"
        "}"
    );

    QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget_);
    containerLayout->setSpacing(15);
    containerLayout->setContentsMargins(40, 35, 40, 35);

    // Title
    titleLabel_ = new QLabel("Transactions", containerWidget_);
    QFont titleFont("Segoe UI", 28, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    titleLabel_->setMinimumHeight(45);
    containerLayout->addWidget(titleLabel_);


    //Account dropdown label
    accountLabel_ = new QLabel("Select account:", containerWidget_);
    QFont accountFont("Segoe UI", 11, QFont::Bold);
    accountLabel_->setFont(accountFont);
    accountLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    containerLayout->addWidget(accountLabel_);


    //create combo box for the dropdown
    accountsDrop_ = new QComboBox(containerWidget_);


    //accountDropSet();

    accountsDrop_->setCursor(Qt::PointingHandCursor);
    containerLayout->addWidget(accountsDrop_);
    //connects button/drop down to a function.

    connect(accountsDrop_, &QComboBox::currentIndexChanged, this, &TransactionsPage::onAccountChanged);


    containerLayout->addSpacing(10);




    //create and add transaction table
    transTable_ = new QTableWidget(containerWidget_);
    QFont transTFont("Segoe UI", 11, QFont::Bold);
    transTable_->setFont(transTFont);
    transTable_->setStyleSheet(
        "QTableWidget {"
        "background - color:  #ffffff;"
        "alternate-background-color: #eaf7ea;"
        "gridline - color: #b6d8b6;"
        "selection-background-color: #3fa645;"
        "selection - color: white; "
        "font - size: 14px; "
        "}"
        "QHeaderView::section {"
        "background-color: #dff0df;"
        "padding: 6px;"
        "border: 1px solid #b6d8b6;"
        "font-weight: bold;"
        "font - size: 14px; "
        "}"
        "QTableView::item:hover {"
        "   background-color: #d0ebd0;"            
        "}"

    );
    containerLayout->addWidget(transTable_);

    transactiontablesetup();



    // Add container to main layout with centering
    mainLayout_->addStretch();

    // Horizontal centering
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(containerWidget_);
    hLayout->addStretch();
    mainLayout_->addLayout(hLayout);

    mainLayout_->addStretch();
}

void TransactionsPage::onShow() {
	cout << "TransactionsPage::onShow called" << endl;
    accountDropSet();
    //accountsDrop_->setCurrentIndex(-1);

}


//sets the drop down selections
//drop down is blank by default
void TransactionsPage::accountDropSet()
{
    accountsDrop_->clear();
    DatabaseManager& db = DatabaseManager::getInstance();
    AccountManager accM(*currentUser_);

    auto accResults = db.retrieveTable(accM.getAccountsTableName(), "");

    QString returnType;
    QString returnNum;
    for (const auto& ty : accResults)
    {
        returnType = QString::fromStdString(static_cast<string>(ty.at("accountType")));
        returnNum = QString::fromStdString(static_cast<string>(ty.at("accountNumber")));
        returnType = returnType + " " + returnNum;
        accountsDrop_->addItem(returnType, QVariant(returnNum));

    }

    accountsDrop_->setCurrentIndex(-1);
}


//Detects when drop down selection is changed 
//and refreshes table to current selection
void TransactionsPage::onAccountChanged(int index) {

    accountName_ = accountsDrop_->itemText(index);
    accountID_ = accountsDrop_->itemData(index).toString();

    qDebug() << "Account changed to:" << accountName_ << "(" << accountID_ << ")";

    string tempnum = accountID_.toStdString();

    transactionTableRefresh(tempnum);


}



//transaction table setup
void TransactionsPage::transactiontablesetup()
{
    cout << "in transaction table setup" << endl;
    if (!currentUser_) {
        qDebug() << "Error: currentUser_ is null";
        return;
    }

    //Creates columns and adds header labels to each column
    transTable_->setColumnCount(5);
    transTable_->setHorizontalHeaderLabels({ "Transaction Type", "Amount", "Transaction Date", "Description", "Balance After" });

    string tempnum = accountID_.toStdString();

    //transactionTableRefresh(tempnum);

}


//clears the current info in the table then fills it out again
//with the account that is currently selected
void TransactionsPage::transactionTableRefresh(string acc)
{
    transTable_->clearContents();
    AccountManager transac(*currentUser_);
    DatabaseManager& db = DatabaseManager::getInstance();
    int j = 0;
    
    string tabwhere = "accountNumber = '" + acc + "'";
    auto vec =
        db.retrieveTable(transac.getTransactionsTableName(), tabwhere);
    transTable_->setRowCount(static_cast<int>(vec.size()));

    if (!vec.empty())
    {
        cout << "\n=== Loading Transaction Data ===" << endl;
        for (const auto& t : vec)
        {
            //Extract transaction info from result
            string transType = static_cast<string>(t.at("transactionType"));
            string transAmt = static_cast<string>(t.at("amount"));
            string transDate = static_cast<string>(t.at("transactionDate"));
            string transDesc = static_cast<string>(t.at("description"));
            string transBal = static_cast<string>(t.at("balanceAfter"));



            //Create item to be added to row 
            QTableWidgetItem* item0 = new QTableWidgetItem(QString::fromStdString(transType));
            //set item's flag to be editable
            item0->setFlags(item0->flags() | Qt::ItemIsEditable);
            //add item to correct row, column , then the item info
            transTable_->setItem(j, 0, item0);

            //repeat above for each column
            QTableWidgetItem* item1 = new QTableWidgetItem(QString::fromStdString(transAmt));
            item1->setFlags(item1->flags() | Qt::ItemIsEditable);
            transTable_->setItem(j, 1, item1);

            QTableWidgetItem* item2 = new QTableWidgetItem(QString::fromStdString(transDate));
            item2->setFlags(item2->flags() | Qt::ItemIsEditable);
            transTable_->setItem(j, 2, item2);

            QTableWidgetItem* item3 = new QTableWidgetItem(QString::fromStdString(transDesc));
            item3->setFlags(item3->flags() | Qt::ItemIsEditable);
            transTable_->setItem(j, 3, item3);

            QTableWidgetItem* item4 = new QTableWidgetItem(QString::fromStdString(transBal));
            item4->setFlags(item4->flags() | Qt::ItemIsEditable);
            transTable_->setItem(j, 4, item4);
            //increment for row count
            ++j;


            //Code to make each item in the table uneditable
            item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
            item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
            item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
            item3->setFlags(item3->flags() & ~Qt::ItemIsEditable);
            item4->setFlags(item4->flags() & ~Qt::ItemIsEditable);
        }

    }

    //table cleanup
    transTable_->resizeRowsToContents();
}