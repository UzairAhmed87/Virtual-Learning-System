#include "DatabaseManager.h"
#include <QDebug>
#include <QSqlError>

DatabaseManager::DatabaseManager() {
    // 🔥 Hardcoded database credentials (CHANGE THESE TO MATCH YOUR DB)
    QString host = "ep-ancient-morning-a48hzu0g-pooler.us-east-1.aws.neon.tech";
    QString dbName = "neondb";
    QString user = "neondb_owner";
    QString password = "npg_PqO8nbkQ6FrG";
    int port = 5432;  // PostgreSQL default port

    // Debugging output
    qDebug() << "🔹 DB_HOST:" << host;
    qDebug() << "🔹 DB_NAME:" << dbName;
    qDebug() << "🔹 DB_USER:" << user;
    qDebug() << "🔹 DB_PORT:" << port;

    // Use QPSQL driver for PostgreSQL
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);
    db.setConnectOptions("sslmode=require");

    if (!db.open()) {
        qDebug() << "❌ Database connection failed: " << db.lastError().text();
    } else {
        qDebug() << "✅ Database connected successfully!";
    }
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();  // Close database connection
    }
}

DatabaseManager& DatabaseManager::getInstance() {
    static DatabaseManager instance;
    return instance;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return db;
}
