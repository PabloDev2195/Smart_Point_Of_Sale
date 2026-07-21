#include "database_manager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

/**
 * @brief Default constructor.
 *
 * The constructor is private because DatabaseManager
 * implements the Singleton design pattern.
 */
DatabaseManager::DatabaseManager()
{
}

/**
 * @brief Returns the unique instance of DatabaseManager.
 *
 * This method implements the Singleton pattern.
 * The first call creates the instance and subsequent
 * calls return the same object.
 *
 * @return Reference to the unique DatabaseManager instance.
 */
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

/**
 * @brief Opens the SQLite database connection.
 *
 * The database is stored in the application's data
 * directory obtained through QStandardPaths.
 * If the directory does not exist, it is created automatically.
 *
 * @return true if the database was opened successfully.
 * @return false if an error occurred.
 */
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

/**
 * @brief Creates the database tables.
 *
 * Currently creates the products table if it does not exist.
 *
 * @return true if all tables were created successfully.
 * @return false if any SQL statement failed.
 */
bool DatabaseManager::createTables()
{
    QSqlQuery query;
	bool bCreateTable = true;

    QString productsTable =
        "CREATE TABLE IF NOT EXISTS products ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
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