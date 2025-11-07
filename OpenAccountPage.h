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
class OpenAccountPage : public Page
{
	Q_OBJECT

public:
	OpenAccountPage();
	~OpenAccountPage() override;

	void onShow() override;

	void setUser(User* user);

protected:
	void buildUI() override;

private slots:

private:
	//UI Components
	QWidget* containerWidget_;

	//Labels
	QLabel* titleLabel_;
	QLabel* newAccountLabel_;

	//Buttons
	QPushButton* newChequingButton_;
	QPushButton* newSavingsButton_;
	QPushButton* newCreditButton_;
	QPushButton* newLoanButton_;

	//Current user
	User* currentUser_;

};

