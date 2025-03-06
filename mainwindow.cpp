#include "mainwindow.h"
#include "ui_mainwindow.h"  // Include the generated UI header
#include "AdminHomePage.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "DatabaseManager.h"
#include "StudentHomePage.h"
#include "TeacherHomePage.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->setStyleSheet(
        "QMessageBox { background-color: #1b263b; color: white; font-size: 14px; }"
        "QMessageBox QLabel { color: white; font-size: 16px; }"
        "QMessageBox QPushButton { background-color: #778da9; color: #1b263b;border:2px solid #778da9 border-radius: 5px; padding: 5px; }"
        "QMessageBox QPushButton:hover { background-color: transparent; color:white}"
        );
     connect(ui->LoginBTN, &QPushButton::clicked, this, &MainWindow::openHomePage);

}

MainWindow::~MainWindow()
{ 
    delete ui;
    delete adminPage;
    delete studentPage;
    delete teacherPage;
}



void MainWindow::openHomePage() {
    QString email = ui->emailField->text();
    QString password = ui->passField->text();
    QString role = ui->roleField->currentText(); // Get role from combo box

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Error", "Please enter both email and password.");
        return;
    }

    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection is not open!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE email = :email AND password = :password AND role = :role");
    query.bindValue(":email", email);
    query.bindValue(":password", password);
    query.bindValue(":role", role);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Query execution failed: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        QMessageBox::information(this, "Login Success", "Welcome " + role + "!");

        this->hide(); // Hide the login window

        if (role == "Admin") {
            adminPage = new AdminHomePage();
            adminPage->showMaximized();
        }
        else if (role == "Student") {
            studentPage = new StudentHomePage();
            studentPage->showMaximized();
        }
        else if(role == "Teacher")
        {
            teacherPage = new TeacherHomePage();
            teacherPage->showMaximized();
        }
        else {
            QMessageBox::warning(this, "Access Denied", "No page assigned for this role.");
            this->show();
        }

    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid email, password, or role.");
    }
}
