#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QSqlDatabase>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool openDatabase();
    bool createTables();

private:
    DatabaseManager();
    QSqlDatabase database;
};

#endif // DATABASE_MANAGER_H