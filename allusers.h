#ifndef ALLUSERS_H
#define ALLUSERS_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringList>
#include <QEvent>
#include <QPropertyAnimation>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include "TopBar.h"


class AllUsers : public QWidget {
    Q_OBJECT
public:
    AllUsers(QWidget *parent = nullptr);
    void applyHoverEffect(QPushButton *button);
    void populateTable(const QStringList &data);


private:
    QTableWidget *table;
    QPushButton *refreshBtn, *filterBtn;
    QLineEdit *searchBar;
    QLabel *heading;
    QWidget *hoverBar;
    QPushButton *studentBtn, *teacherBtn;
    QPushButton *deleteBtn;  // Refresh button
    QPushButton *updateBtn;

    QStringList students, teachers;
    QString currentCategory = "Students";
    TopBar *topBar;
    QTableWidget *tableWidget;
    QLabel *totalUsersLabel; // Label to display count
    QPushButton *backButton;

    void setupUI();
    void setupHoverEffect();
    void setupConnections();

    void switchCategory(const QString &category);
    void searchUser();
    void refreshTable();
    bool eventFilter(QObject *obj, QEvent *event);
    void updateUser(int row); // Function to update user details
    void deleteUser(int row); // Function to delete user
    void updateUserCount();
signals:
    void backButtonClicked();

};

#endif // ALLUSERS_H
