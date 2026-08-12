#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QSqlDatabase>

/**
 * @brief Manages the SQLite database connection.
 *
 * This class is responsible for:
 * - Opening the database connection.
 * - Creating the database schema.
 * - Providing a single database connection through the Singleton pattern.
 */
class DatabaseManager
{
public:

    /**
     * @brief Returns the unique DatabaseManager instance.
     *
     * @return Reference to the DatabaseManager instance.
     */
    static DatabaseManager& instance();

    /**
     * @brief Opens the SQLite database.
     *
     * @return true if the connection is opened successfully.
     * @return false otherwise.
     */
    bool openDatabase();

    /**
     * @brief Creates the required database tables.
     *
     * @return true if all tables are created successfully.
     * @return false otherwise.
     */
    bool createTables();

    /**
     * @brief Returns the current SQLite database connection.
     *
     * Provides access to the database connection managed by the
     * DatabaseManager singleton.
     *
     * @return The current QSqlDatabase connection.
     */
    QSqlDatabase getDatabase() const;

private:

    /**
     * @brief Private constructor.
     *
     * Prevents direct instantiation of the class.
     */
    DatabaseManager();

    /* SQLite database connection */
    QSqlDatabase database;
};

#endif // DATABASE_MANAGER_H