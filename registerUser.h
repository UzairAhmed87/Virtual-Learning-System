#ifndef REGISTERUSERFORM_H
#define REGISTERUSERFORM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QPixmap>
#include "TopBar.h"
#include <QCryptographicHash>
#include <QString>

class RegisterUserForm : public QWidget {
    Q_OBJECT  // Required if you use signals/slots

public:
    explicit RegisterUserForm(QWidget *parent = nullptr,QWidget *topBar = nullptr);
    QString generateUniqueID(QString role,QString department);
    void reset();
    ~RegisterUserForm();
private:
    QLineEdit *firstNameField;
    QLineEdit *lastNameField;
    QLineEdit *phoneField;
    QComboBox *genderDropdown;
    QComboBox *userTypeDropdown;
    QComboBox *departmentDropdown;
    QLineEdit *emailField;
    QLineEdit *passwordField;
    QPushButton *registerButton;
    QPushButton *resetButton;
    // TopBar *topBar;
signals:
    void backButtonClicked();
// private slots:
    // void gotoHome();
};

#endif // REGISTERUSERFORM_H
