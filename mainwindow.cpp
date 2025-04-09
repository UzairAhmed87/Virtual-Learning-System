#include "mainwindow.h"
#include "LoginPage.h"
#include <QDebug>
#include "mesgboxutil.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setMinimumSize(800,500);
    setStyleSheet("background-color: #0d1b2a;");
    setContentsMargins(0,0,0,0);
    stackWidget = new QStackedWidget(this);
    setCentralWidget(stackWidget);

    loginPage = new LoginPage();
    wrapperWidget = new QWidget();
    QVBoxLayout *wrapperLayout = new QVBoxLayout(wrapperWidget);

    wrapperLayout->addStretch();  // Push content down
    wrapperLayout->addWidget(loginPage, 0, Qt::AlignHCenter);  // Center horizontally
    wrapperLayout->addStretch();  // Push content up

    wrapperWidget->setLayout(wrapperLayout);

    stackWidget->addWidget(wrapperWidget);
    connect(loginPage, &LoginPage::loginSuccessful, this, &MainWindow::handleLoginSuccess);

    stackWidget->setCurrentWidget(wrapperWidget); // Start with login page
    qDebug() << "LoginPage shown";
}

MainWindow::~MainWindow()
{
    // delete loginPage;
    // if (adminPage) delete adminPage;
    // if (studentPage) delete studentPage;
    // if (teacherPage) delete teacherPage;
    delete stackWidget;
}

void MainWindow::handleLoginSuccess(QString role, QString email)
{
    if (role == "Admin") {
        adminPage = new AdminHomePage(email);
        stackWidget->addWidget(adminPage);
        connect(adminPage, &AdminHomePage::logoutRequested, this, &MainWindow::handleLogout);
        stackWidget->setCurrentWidget(adminPage);
    }
    else if (role == "Student") {
        studentPage = new StudentHomePage(email);
        stackWidget->addWidget(studentPage);
        connect(studentPage, &StudentHomePage::logoutRequested, this, &MainWindow::handleLogout);
        stackWidget->setCurrentWidget(studentPage);
    }
    else if (role == "Teacher") {
        teacherPage = new TeacherHomePage(email);
        stackWidget->addWidget(teacherPage);
        connect(teacherPage, &TeacherHomePage::logoutRequested, this, &MainWindow::handleLogout);
        stackWidget->setCurrentWidget(teacherPage);
    }
    else {
        MessageBoxUtil::showCustomMessage(
            this,
            "No page assigned for this role.",
            "Access Denied",
            "OK"
            );
        stackWidget->setCurrentWidget(wrapperWidget);
    }
}

void MainWindow::handleLogout()
{
    // Switch back to login page
    stackWidget->setCurrentIndex(0); // Assuming login wrapper is the first widget
    qDebug()<<stackWidget->currentWidget();

}
