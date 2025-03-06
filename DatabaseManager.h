#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>

class DatabaseManager {
public:
    static DatabaseManager& getInstance();  // Singleton pattern
    QSqlDatabase getDatabase();

private:
    DatabaseManager();   // Private constructor
    ~DatabaseManager();  // Private destructor
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
