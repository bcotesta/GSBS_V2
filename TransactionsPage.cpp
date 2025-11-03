// Brandon Cotesta || 10/28/2025 | 3:00 PM
// Transactions page for displaying user transactions

#include "TransactionsPage.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QScrollArea>
#include <iomanip>
#include <sstream>

TransactionsPage::TransactionsPage()
    : Page("transactions"),

    containerWidget_(nullptr),
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
    SessionManager s = SessionManager();
    //setUser(s.getCurrentUser());
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
    //the options to select from. the first string is the account name / what is displayed
    //and the second string is the id
    accountsDrop_->addItem("Chequing", QVariant("Chequing"));
    accountsDrop_->addItem("Savings", QVariant("Savings"));
    accountsDrop_->addItem("Credit", QVariant("Credit"));
    accountsDrop_->addItem("Loan", QVariant("Loan"));
    containerLayout->addWidget(accountsDrop_);
    //connects button/drop down to a function.
    connect(accountsDrop_, &QComboBox::currentIndexChanged, this, &TransactionsPage::onAccountChanged);
    
    //create and add transaction table
    transTable_ = new QTableWidget(containerWidget_);
    QFont transTFont("Segoe UI", 11, QFont::Bold);
    transTable_->setFont(transTFont);
    transTable_->setStyleSheet("QTableWidget { background-color: #ecf0f1;}");
    containerLayout->addWidget(transTable_);

    //transactiontablesetup();


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

    transactiontablesetup();

}


//Detects when drop down selection is changed
// no info currently displayed
//currently doesn't change the info displayed
void TransactionsPage::onAccountChanged(int index) {

    QString accountName = accountsDrop_->itemText(index);
    QString accountId = accountsDrop_->itemData(index).toString();

    qDebug() << "Account changed to:" << accountName << "(" << accountId << ")";

}

//temp name
//transaction table
void TransactionsPage::transactiontablesetup()
{
    cout << "in transaction table setup";
    if (!currentUser_) {
         qDebug() << "Error: currentUser_ is null";
         return;
    }
        AccountManager transac(*currentUser_);
        transTable_->setColumnCount(5);
        transTable_->setHorizontalHeaderLabels({ "Transaction Type", "Amount", "Transaction Date", "Description", "Balance After" });

        DatabaseManager& db = DatabaseManager::getInstance();
        int j = 0;
        auto vec =
            db.retrieveTable(transac.getTransactionsTableName(), "");

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

                //QTableWidgetItem* newItem = new QTableWidgetItem();


                //transTable_ *newItem = new QTableWidgetItem(tr("%1").arg(rows + 1) * (column + 1)));
          
                QTableWidgetItem* item0 = new QTableWidgetItem(QString::fromStdString(transType));
                item0->setFlags(item0->flags() | Qt::ItemIsEditable);
                transTable_->setItem(j, 0, item0);
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
                ++j;

                /*
                    transTable_->setItem(j, 1, new QTableWidgetItem(QString::fromStdString(transAmt)));
                    transTable_->setItem(j, 2, new QTableWidgetItem(QString::fromStdString(transDate)));
                    transTable_->setItem(j, 3, new QTableWidgetItem(QString::fromStdString(transDesc)));
                    transTable_->setItem(j, 4, new QTableWidgetItem(QString::fromStdString(transBal)));
                    j++;
                    QTableWidgetItem* existingItem0 = transTable_->item(j, 0);
                    QTableWidgetItem* existingItem1 = transTable_->item(j, 1);
                    QTableWidgetItem* existingItem2 = transTable_->item(j, 2);
                    QTableWidgetItem* existingItem3 = transTable_->item(j, 3);
                    QTableWidgetItem* existingItem4 = transTable_->item(j, 4);
                    existingItem0->setFlags(existingItem0->flags() | Qt::ItemIsEditable);
                    existingItem1->setFlags(existingItem1->flags() | Qt::ItemIsEditable);
                    existingItem2->setFlags(existingItem2->flags() | Qt::ItemIsEditable);
                    existingItem3->setFlags(existingItem3->flags() | Qt::ItemIsEditable);
                    existingItem4->setFlags(existingItem4->flags() | Qt::ItemIsEditable);
                    existingItem0->setFlags(existingItem0->flags() & ~Qt::ItemIsEditable);
                    existingItem1->setFlags(existingItem1->flags() & ~Qt::ItemIsEditable);
                    existingItem2->setFlags(existingItem2->flags() & ~Qt::ItemIsEditable);
                    existingItem3->setFlags(existingItem3->flags() & ~Qt::ItemIsEditable);
                    existingItem4->setFlags(existingItem4->flags() & ~Qt::ItemIsEditable);
                    */
                item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
                item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
                item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
                item3->setFlags(item3->flags() & ~Qt::ItemIsEditable);
                item4->setFlags(item4->flags() & ~Qt::ItemIsEditable);
            }
           // item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        }
            
        //transTable_->setRowCount(static_cast<int>(vec.size()));

        //for loop to fill table with each row with the vector from retrieveTable()
        /*
        for (int i = 0; i < vec.size(); i++)
        {
            const auto& info = vec[i];
            transTable_->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(info.at("transactionType"))));
            transTable_->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.at("amount"))));
            transTable_->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(info.at("transactionDate"))));
            transTable_->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(info.at("description"))));
            transTable_->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(info.at("balanceAfter"))));
        }*/
   
    transTable_->resizeRowsToContents();
    transTable_->close();
    transTable_->show();
}