#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AdminHomePage.h"
#include "DatabaseManager.h"  // Include database manager
#include "StudentHomePage.h"
#include "TeacherHomePage.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openHomePage();

private:
    Ui::MainWindow *ui;
    AdminHomePage *adminPage;
    StudentHomePage *studentPage;
    TeacherHomePage *teacherPage;
};
#endif // MAINWINDOW_H
