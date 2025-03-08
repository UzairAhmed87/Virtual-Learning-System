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
#include <QPixmap>
#include "TopBar.h"

class RegisterUserForm : public QWidget {
    Q_OBJECT  // Required if you use signals/slots

public:
    explicit RegisterUserForm(QWidget *parent = nullptr);
private:
    QLineEdit *nameField;
    QLineEdit *emailField;
    QLineEdit *passwordField;
    QComboBox *roleDropdown;
    QPushButton *submitButton;
    TopBar *topBar;
};

#endif // REGISTERUSERFORM_H
