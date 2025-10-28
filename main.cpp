// Brandon Cotesta | 10/27/2025 | 8:43 PM
// MOOD: Neutral. not excited to spend the night doing this
// but ready to get it done. Blue Jays are down 1-0 >:(.

#include "DatabaseManager.h"


#include <QtCore/QCoreApplication>

// function to test DatabaseManager singleton connection
static void DEBUG_connectDB() {
    DatabaseManager& dbManager = DatabaseManager::getInstance();
}


// Main loop
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
	DEBUG_connectDB();
    return app.exec();
}

