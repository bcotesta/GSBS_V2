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
	//QWidget* containerWidget_;
	QScrollArea* scrollArea_;
	QWidget* scrollContent_;
	QVBoxLayout* contentLayout_;
	
	//Header section
	QWidget* headerWidget_;
	QLabel* titleLabel_;

	//Current user
	User* currentUser_;

	//Section
	QWidget* newAccountSection_;
	QVBoxLayout* newAccountLayout_;
	QLabel* newSavingsLabel;


	//helper method
	QWidget* createSectionHeader(const QString& title);

};

