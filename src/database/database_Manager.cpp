#include "database_manager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

DatabaseManager::DatabaseManager()
{
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}


bool DatabaseManager::openDatabase()
{
    bool bOpenDatabase = true;

    database = QSqlDatabase::addDatabase("QSQLITE");

    QString dataPath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);

    QDir dir(dataPath);

    if(!dir.exists())
    {
        dir.mkpath(".");
    }

    database.setDatabaseName(dataPath + "/pos.db");

    if(!database.open())
    {
        qDebug() << "Database error:"
                 << database.lastError().text();

        bOpenDatabase = false;
    }

    qDebug() << "Database path:" << database.databaseName();

    return bOpenDatabase;
}


bool DatabaseManager::createTables()
{
    QSqlQuery query;
	bool bCreateTable = true;

    QString productsTable =
        "CREATE TABLE IF NOT EXISTS products ("
        "barcode TEXT UNIQUE,"
        "name TEXT NOT NULL,"
        "purchase_price REAL NOT NULL,"
        "sale_price REAL NOT NULL"
        ")";


    if(!query.exec(productsTable))
    {
        qDebug() << query.lastError().text();
        bCreateTable = false;
    }


    return bCreateTable;
}