#include "LoginPage.h"
#include "ui_LoginPage.h"
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "DatabaseManager.h"
#include "QCryptographicHash"
#include "mesgboxutil.h"

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Expand to fit parent
    setMinimumSize(800, 500); // Adjust to your required size
    connect(ui->LoginBTN, &QPushButton::clicked, this, &LoginPage::handleLogin);
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::handleLogin() {
    QString email = ui->emailField->text();
    QString password = ui->passField->text();
    QString role = ui->roleField->currentText();

    if (email.isEmpty() || password.isEmpty()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Please enter both email and password.⚠️ ",
            "Login Error", // You can set any title you want
            "OK"           // Customize the button text
            );
        return;
    }

    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Database connection is not open! ",
            "Database Error", // You can set any title you want
            "OK"           // Customize the button text
            );

        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id, first_name, last_name, email, phone, gender, role, department "
                  "FROM vls_schema.users "
                  "WHERE email = :email "
                  "AND password_hash = crypt(:password, password_hash) "
                  "AND LOWER(role) = LOWER(:role)");

    query.bindValue(":email", email);
    query.bindValue(":password", password);
    query.bindValue(":role", role);
    if (!query.exec()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Query execution failed: "+ query.lastError().text(),
            "Database Error", // You can set any title you want
            "OK"           // Customize the button text
            );
        return;
    }

    if (query.next()) {
        QString firstName = query.value(1).toString();
        MessageBoxUtil::showCustomMessage(
            this,
            "✅ Welcome " + firstName + "!",
            "Login Successfull", // You can set any title you want
            "OK"           // Customize the button text
            );
        emit loginSuccessful(role,email);
    } else {
        MessageBoxUtil::showCustomMessage(
            this,
            "Invalid email, password, or role.",
            "Login Failed", // You can set any title you want
            "OK"           // Customize the button text
            );
    }
}
