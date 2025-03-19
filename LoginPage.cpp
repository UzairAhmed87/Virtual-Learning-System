#include "LoginPage.h"
#include "ui_LoginPage.h"
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "DatabaseManager.h"
#include "QCryptographicHash"

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

    QMessageBox mesgBox;
    mesgBox.setStyleSheet(
        "QMessageBox { background-color: #1b263b; color: white; font-size: 16px; }"
        "QLabel { color: white; }"
        "QPushButton { background-color: #778da9; color: white;border:2px solid #778da9; border-radius: 5px; padding: 8px; }"
        "QPushButton:hover { background-color: transparent; }"
        );

    if (email.isEmpty() || password.isEmpty()) {
        mesgBox.setWindowTitle("Login Error");
        mesgBox.setText("Please enter both email and password.⚠️ ");
        mesgBox.setIcon(QMessageBox::Warning);
        mesgBox.exec();
        return;
    }

    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        mesgBox.setWindowTitle("Database Error");
        mesgBox.setText("Database connection is not open!");
        mesgBox.setIcon(QMessageBox::Critical);
        mesgBox.exec();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id, first_name, last_name, email, phone, gender, role, department, course "
                  "FROM vls_schema.users "
                  "WHERE email = :email "
                  "AND password_hash = crypt(:password, password_hash) "
                  "AND LOWER(role) = LOWER(:role)");

    query.bindValue(":email", email);
    query.bindValue(":password", password);
    query.bindValue(":role", role);


    query.bindValue(":email", email);
    query.bindValue(":password", password); // Use hashed password
    query.bindValue(":role", role);

    if (!query.exec()) {
        mesgBox.setWindowTitle("Database Error");
        mesgBox.setText("Query execution failed: " + query.lastError().text());
        mesgBox.setIcon(QMessageBox::Critical);
        mesgBox.exec();
        return;
    }

    if (query.next()) {
        QString firstName = query.value(1).toString();
        mesgBox.setWindowTitle("Login Success");
        mesgBox.setText("✅ Welcome " + firstName + "!");
        mesgBox.setIcon(QMessageBox::Information);
        mesgBox.exec();
        emit loginSuccessful(role);
    } else {
        mesgBox.setWindowTitle("Login Failed");
        mesgBox.setText("Invalid email, password, or role.");
        mesgBox.setIcon(QMessageBox::Warning);
        mesgBox.exec();
    }
}
