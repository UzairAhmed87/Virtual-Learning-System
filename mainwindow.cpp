#include "mainwindow.h"
#include "LoginPage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(800,500);
    setStyleSheet("background-color: #0d1b2a;");
    setContentsMargins(0,0,0,0);
    stackWidget = new QStackedWidget(this);
    setCentralWidget(stackWidget);

    loginPage = new LoginPage();
    QWidget *wrapperWidget = new QWidget();
    QVBoxLayout *wrapperLayout = new QVBoxLayout(wrapperWidget);

    wrapperLayout->addStretch();  // Push content down
    wrapperLayout->addWidget(loginPage, 0, Qt::AlignHCenter);  // Center horizontally
    wrapperLayout->addStretch();  // Push content up

    wrapperWidget->setLayout(wrapperLayout);

    stackWidget->addWidget(wrapperWidget);
    connect(loginPage, &LoginPage::loginSuccessful, this, &MainWindow::handleLoginSuccess);

    stackWidget->setCurrentWidget(loginPage); // Start with login page
}

MainWindow::~MainWindow()
{
    delete loginPage;
    delete adminPage;
    delete studentPage;
    delete teacherPage;
}

void MainWindow::handleLoginSuccess(QString role)
{
    if (role == "Admin") {
        adminPage = new AdminHomePage();
        stackWidget->addWidget(adminPage);
        stackWidget->setCurrentWidget(adminPage);
    }
    else if (role == "Student") {
        studentPage = new StudentHomePage();
        stackWidget->addWidget(studentPage);
        stackWidget->setCurrentWidget(studentPage);
    }
    else if (role == "Teacher") {
        teacherPage = new TeacherHomePage();
        stackWidget->addWidget(teacherPage);
        stackWidget->setCurrentWidget(teacherPage);
    }
    else {
        QMessageBox::warning(this, "Access Denied", "No page assigned for this role.");
        stackWidget->setCurrentWidget(loginPage);
    }
}
