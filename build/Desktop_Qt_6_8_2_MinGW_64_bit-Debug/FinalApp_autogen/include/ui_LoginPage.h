/********************************************************************************
** Form generated from reading UI file 'LoginPage.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINPAGE_H
#define UI_LOGINPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginPage
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QLabel *logoLabel;
    QFrame *loginForm;
    QGridLayout *gridLayout_2;
    QLabel *emailHead;
    QPushButton *LoginBTN;
    QLabel *passHead;
    QComboBox *roleField;
    QLineEdit *emailField;
    QLabel *roleHead;
    QLineEdit *loginHead;
    QLineEdit *passField;

    void setupUi(QWidget *LoginPage)
    {
        if (LoginPage->objectName().isEmpty())
            LoginPage->setObjectName("LoginPage");
        LoginPage->resize(800, 500);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LoginPage->sizePolicy().hasHeightForWidth());
        LoginPage->setSizePolicy(sizePolicy);
        LoginPage->setMinimumSize(QSize(800, 500));
        LoginPage->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(LoginPage);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setGeometry(QRect(20, 80, 761, 336));
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        centralwidget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(100);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        logoLabel = new QLabel(centralwidget);
        logoLabel->setObjectName("logoLabel");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(logoLabel->sizePolicy().hasHeightForWidth());
        logoLabel->setSizePolicy(sizePolicy1);
        logoLabel->setMinimumSize(QSize(300, 300));
        logoLabel->setMaximumSize(QSize(600, 600));
        logoLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/images/BigLogo.png")));
        logoLabel->setScaledContents(true);
        logoLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(logoLabel);

        loginForm = new QFrame(centralwidget);
        loginForm->setObjectName("loginForm");
        loginForm->setEnabled(true);
        sizePolicy1.setHeightForWidth(loginForm->sizePolicy().hasHeightForWidth());
        loginForm->setSizePolicy(sizePolicy1);
        loginForm->setMinimumSize(QSize(300, 300));
        loginForm->setMaximumSize(QSize(600, 600));
        loginForm->setStyleSheet(QString::fromUtf8("background-color:#1b263b;border:2px;border-radius:20px;"));
        loginForm->setFrameShape(QFrame::Shape::StyledPanel);
        loginForm->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_2 = new QGridLayout(loginForm);
        gridLayout_2->setObjectName("gridLayout_2");
        emailHead = new QLabel(loginForm);
        emailHead->setObjectName("emailHead");
        emailHead->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(emailHead->sizePolicy().hasHeightForWidth());
        emailHead->setSizePolicy(sizePolicy2);
        QFont font;
        font.setPointSize(12);
        emailHead->setFont(font);
        emailHead->setAutoFillBackground(false);
        emailHead->setStyleSheet(QString::fromUtf8("color : #ffffff"));

        gridLayout_2->addWidget(emailHead, 1, 0, 1, 1);

        LoginBTN = new QPushButton(loginForm);
        LoginBTN->setObjectName("LoginBTN");
        sizePolicy1.setHeightForWidth(LoginBTN->sizePolicy().hasHeightForWidth());
        LoginBTN->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        LoginBTN->setFont(font1);
        LoginBTN->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        LoginBTN->setAutoFillBackground(false);
        LoginBTN->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    color: white;\n"
"    background-color: #1E90FF;\n"
"    padding: 8px;\n"
"    border: 2px solid #1E90FF;\n"
"    border-radius: 15px;n\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: transparent;\n"
"    border-color: #1E90FF;\n"
"    color: white;\n"
"}\n"
""));
        LoginBTN->setAutoDefault(false);

        gridLayout_2->addWidget(LoginBTN, 7, 0, 1, 1, Qt::AlignmentFlag::AlignHCenter);

        passHead = new QLabel(loginForm);
        passHead->setObjectName("passHead");
        sizePolicy2.setHeightForWidth(passHead->sizePolicy().hasHeightForWidth());
        passHead->setSizePolicy(sizePolicy2);
        passHead->setFont(font);
        passHead->setAutoFillBackground(false);
        passHead->setStyleSheet(QString::fromUtf8("color : #ffffff"));

        gridLayout_2->addWidget(passHead, 3, 0, 1, 1);

        roleField = new QComboBox(loginForm);
        roleField->addItem(QString());
        roleField->addItem(QString());
        roleField->addItem(QString());
        roleField->setObjectName("roleField");
        sizePolicy.setHeightForWidth(roleField->sizePolicy().hasHeightForWidth());
        roleField->setSizePolicy(sizePolicy);
        roleField->setStyleSheet(QString::fromUtf8("background-color:#ffffff;\n"
"padding-left:10px"));
        roleField->setEditable(true);

        gridLayout_2->addWidget(roleField, 6, 0, 1, 1);

        emailField = new QLineEdit(loginForm);
        emailField->setObjectName("emailField");
        sizePolicy.setHeightForWidth(emailField->sizePolicy().hasHeightForWidth());
        emailField->setSizePolicy(sizePolicy);
        emailField->setStyleSheet(QString::fromUtf8("background-color: #ffffff;\n"
"padding-left :10px "));

        gridLayout_2->addWidget(emailField, 2, 0, 1, 1);

        roleHead = new QLabel(loginForm);
        roleHead->setObjectName("roleHead");
        sizePolicy2.setHeightForWidth(roleHead->sizePolicy().hasHeightForWidth());
        roleHead->setSizePolicy(sizePolicy2);
        roleHead->setFont(font);
        roleHead->setAutoFillBackground(false);
        roleHead->setStyleSheet(QString::fromUtf8("color : #ffffff"));

        gridLayout_2->addWidget(roleHead, 5, 0, 1, 1);

        loginHead = new QLineEdit(loginForm);
        loginHead->setObjectName("loginHead");
        loginHead->setEnabled(true);
        sizePolicy2.setHeightForWidth(loginHead->sizePolicy().hasHeightForWidth());
        loginHead->setSizePolicy(sizePolicy2);
        QFont font2;
        font2.setPointSize(16);
        font2.setBold(true);
        loginHead->setFont(font2);
        loginHead->setStyleSheet(QString::fromUtf8("color : #ffffff;\n"
"width:60px\n"
""));
        loginHead->setAlignment(Qt::AlignmentFlag::AlignCenter);
        loginHead->setDragEnabled(false);
        loginHead->setReadOnly(true);

        gridLayout_2->addWidget(loginHead, 0, 0, 1, 1, Qt::AlignmentFlag::AlignHCenter);

        passField = new QLineEdit(loginForm);
        passField->setObjectName("passField");
        sizePolicy.setHeightForWidth(passField->sizePolicy().hasHeightForWidth());
        passField->setSizePolicy(sizePolicy);
        passField->setStyleSheet(QString::fromUtf8("background-color: #ffffff;\n"
"padding-left:5px;\n"
""));
        passField->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout_2->addWidget(passField, 4, 0, 1, 1);


        horizontalLayout->addWidget(loginForm);

        QWidget::setTabOrder(emailField, passField);
        QWidget::setTabOrder(passField, roleField);
        QWidget::setTabOrder(roleField, LoginBTN);

        retranslateUi(LoginPage);

        LoginBTN->setDefault(false);


        QMetaObject::connectSlotsByName(LoginPage);
    } // setupUi

    void retranslateUi(QWidget *LoginPage)
    {
        LoginPage->setWindowTitle(QCoreApplication::translate("LoginPage", "LoginPage", nullptr));
        logoLabel->setText(QString());
        emailHead->setText(QCoreApplication::translate("LoginPage", "Email:", nullptr));
        LoginBTN->setText(QCoreApplication::translate("LoginPage", "Log In", nullptr));
        passHead->setText(QCoreApplication::translate("LoginPage", "Password:", nullptr));
        roleField->setItemText(0, QCoreApplication::translate("LoginPage", "Admin", nullptr));
        roleField->setItemText(1, QCoreApplication::translate("LoginPage", "Teacher", nullptr));
        roleField->setItemText(2, QCoreApplication::translate("LoginPage", "Student", nullptr));

        roleHead->setText(QCoreApplication::translate("LoginPage", "Role:", nullptr));
        loginHead->setText(QCoreApplication::translate("LoginPage", "Login", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginPage: public Ui_LoginPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINPAGE_H
