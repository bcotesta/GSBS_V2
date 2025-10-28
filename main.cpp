// Brandon Cotesta | 10/27/2025 | 8:43 PM
// MOOD: Neutral. not excited to spend the night doing this
// but ready to get it done. Blue Jays are down 1-0 >:(.

#include "DatabaseManager.h"


#include <QtCore/QCoreApplication>

// function to test DatabaseManager singleton connection
// gets all columns from userinfo table
static void DEBUG_connectDB() {
    DatabaseManager& dbManager = DatabaseManager::getInstance();

	// grab multiple columns from userinfo table for a specific userID
    std::string selectCols = "userID, name, email, phone, password";
	auto results = dbManager.retrieveTable("userinfo", "");
    for (const auto& row : results) {
        std::cout << "User Record:" << std::endl;
        for (const auto& [colName, value] : row) {
            std::cout << "  " << colName << ": " << static_cast<std::string>(value) << std::endl;
        }
	}
}


// Main loop
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
	DEBUG_connectDB();
    return app.exec();
}

