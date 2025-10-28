// Brandon Cotesta | 10/27/2025 | 8:43 PM
// MOOD: Neutral. not excited to spend the night doing this
// but ready to get it done. Blue Jays are down 1-0 >:(.

#include "DatabaseManager.h"
#include "Authenticator.h"

#include <QtCore/QCoreApplication>

using namespace std;

// function to test DatabaseManager singleton connection
// gets all columns from userinfo table
static void DEBUG() {
    // Test authenticator
	Authenticator& auth = Authenticator::getInstance();
	string testUsername, testPassword;
	cout << "Enter username (email): ";
	getline(cin, testUsername);
	cout << "Enter password: ";
	getline(cin, testPassword);
	auth.setValidInfo(testUsername, testPassword);
}


// Main loop
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
	DEBUG();
    return app.exec();
}

