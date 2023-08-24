#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>

class Connection
{
public:
    Connection();
    bool createConnection();
    void closeConnection();
private:
    QSqlDatabase db;
};

#endif // CONNECTION_H
