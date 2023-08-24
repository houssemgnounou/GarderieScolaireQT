#include "connection.h"
#include <QSqlError>
#include <QMessageBox>

Connection::Connection()
{
}

bool Connection::createConnection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projet");
    db.setUserName("houssem");
    db.setPassword("houssem");

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Connection Error", "Failed to connect to database. Error detail: " + db.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, "Connection", "Connected to database.");
    return true;
}

void Connection::closeConnection()
{
    if (db.isOpen()) {
        db.close();
       // QMessageBox::information(nullptr, "Connection", "Database connection closed.");
    }
}
