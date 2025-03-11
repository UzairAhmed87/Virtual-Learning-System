#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "LoginPage.h"
#include "AdminHomePage.h"
#include "StudentHomePage.h"
#include "TeacherHomePage.h"
#include <QMessageBox>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLoginSuccess(QString role); // Slot to handle login success

private:
    QStackedWidget *stackWidget;
    LoginPage *loginPage;
    AdminHomePage *adminPage;
    StudentHomePage *studentPage;
    TeacherHomePage *teacherPage;
};

#endif // MAINWINDOW_H
