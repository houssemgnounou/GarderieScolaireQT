#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection connection;
    if (!connection.createConnection()) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to the database.");
        return 1;
    }


    MainWindow w;
    w.show();

    int result = a.exec();

    connection.closeConnection();

    return result;
}
