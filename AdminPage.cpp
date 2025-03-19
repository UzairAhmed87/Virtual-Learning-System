
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QPixmap>
#include <QSpacerItem>
#include <QDebug>
#include <QStackedWidget>
#include "AdminHomePage.h"
#include "registerUser.h"
#include "TopBar.h"
#include "createcourse.h"
#include "allusers.h"
#include "ProfilePage.h"
#include  "mainwindow.h"
#include <QTimer>
AdminHomePage::AdminHomePage(QWidget *parent) : QWidget(parent), registerUser(nullptr) ,course(nullptr),users(nullptr){
    setWindowTitle("Admin Home Page");
    setMinimumSize(800, 500);
    setStyleSheet("background-color: #0d1b2a;");

    topBar = new TopBar(this);
    stackWidget = new QStackedWidget(this);
    stackWidget->setContentsMargins(0,0,0,0);
    // ✅ Buttons Grid

   homePageWidget = new QWidget(this);
    homePageWidget->setContentsMargins(0,0,0,0);
    QVBoxLayout *mainLayout = new QVBoxLayout(homePageWidget);
    QWidget *buttonsWidget =  new QWidget();
    QGridLayout *buttonsLayout = new QGridLayout();
    buttonsLayout->setContentsMargins(40, 30, 40, 30);
    buttonsLayout->setHorizontalSpacing(25);
    buttonsLayout->setVerticalSpacing(25);

    QStringList buttonLabels = {
        "Register User", "All Users",
        "Create Course", "All Courses",
        "Enroll Student", "Update Fee Status"
    };

    for (int i = 0; i < buttonLabels.size(); i++) {
        QPushButton *button = new QPushButton(buttonLabels[i]);
        button->setFixedHeight(70);
        button->setCursor(Qt::PointingHandCursor);
        button->setStyleSheet(
            "QPushButton {"
            "   background-color: #1b263b; color: white; font-size: 18px; font-weight: bold;"
            "   border-radius: 10px; padding: 12px; border: 2px solid #415a77;"
            "}"
            "QPushButton:hover { background-color: #2d3e50; border-color: #778da9; }"
            "QPushButton:pressed { background-color: #0f1925; }"
            );

        buttonsLayout->addWidget(button, i / 2, i % 2);
        buttonsWidget->setLayout(buttonsLayout);
        // ✅ Connect button click to a generic slot function
        connect(button, &QPushButton::clicked, this, [=]() { handleButtonClick(buttonLabels[i]); });
    }

    // ✅ Main Layout

    mainLayout->setContentsMargins(0, 0, 0, 0); // **Remove Margins**
    mainLayout->setSpacing(0); // **Remove Extra Spacing**
    mainLayout->addWidget(topBar);
    mainLayout->addWidget(buttonsWidget);
    homePageWidget->setLayout(mainLayout);

    stackWidget->addWidget(homePageWidget);
    QVBoxLayout *mainLayoutBox = new QVBoxLayout(this);
    mainLayoutBox->setContentsMargins(0,0,0,0);
    mainLayoutBox->setSpacing(0);
    mainLayoutBox->addWidget(stackWidget);
    if (!topBar) {
        qDebug() << "❌ topBar is NULL!";
    } else {
        qDebug() << "✅ topBar initialized!";
    }

    connect(topBar,&TopBar::profileClicked,this,&AdminHomePage::openProfilePage);
    bool connected = connect(topBar, &TopBar::homeButtonClicked, this, &AdminHomePage::gotoHomePage, Qt::UniqueConnection);


    if (connected) {
        qDebug() << "✅ Signal connected successfully!";
    } else {
        qDebug() << "❌ Signal connection failed!";
    }


    setLayout(mainLayoutBox);

}

// ✅ Button Handling Function
void AdminHomePage::handleButtonClick(const QString &buttonText) {
    qDebug() << "Button Clicked:" << buttonText; // Debugging Output
    if (buttonText == "Register User") {
        if (!registerUser) { // Ensure no memory leak
            registerUser = new RegisterUserForm();
            stackWidget->addWidget(registerUser);
            connect(registerUser, &RegisterUserForm::backButtonClicked, this, &AdminHomePage::gotoBackPage);
        }
        stackWidget->setCurrentWidget(registerUser);
    } else if (buttonText == "All Users") {
        if (!users) { // Ensure no memory leak
            users = new AllUsers();
            stackWidget->addWidget(users);
             qDebug() << "Opening All Users Page...";
            connect(users, &AllUsers::backButtonClicked, this, &AdminHomePage::gotoBackPage);
        }
        stackWidget->setCurrentWidget(users);

        // Future: Open All Users page
    } else if (buttonText == "Create Course") {
        if (!course) { // Ensure no memory leak
            course = new createcourse();
            stackWidget->addWidget(course);
             qDebug() << "Opening Create Course Form...";
            connect(course, &createcourse::backButtonClicked, this, &AdminHomePage::gotoBackPage);
        }
        stackWidget->setCurrentWidget(course);

        // Future: Open Create Course form
    } else if (buttonText == "All Courses") {
        qDebug() << "Opening All Courses Page...";
        // Future: Open All Courses page
    } else if (buttonText == "Enroll Student") {
        qDebug() << "Opening Enrollment Page...";
        // Future: Open Enrollment page
    } else if (buttonText == "Update Fee Status") {
        qDebug() << "Opening Fee Status Update Form...";
        // Future: Open Fee Status update form
    }
}
void AdminHomePage::gotoBackPage() {
    stackWidget->setCurrentIndex(0);
}
void AdminHomePage::openProfilePage(){
    profilePage = new ProfilePage(this);
    stackWidget->addWidget(profilePage);
    stackWidget->setCurrentWidget(profilePage);
}
void AdminHomePage::gotoHomePage() {
    qDebug() << "🏠 Home Button Clicked! Going back to home page...";

    // Ensure connection remains active
    connect(topBar, &TopBar::homeButtonClicked, this, &AdminHomePage::gotoHomePage, Qt::UniqueConnection);

    if (stackWidget->currentWidget() == homePageWidget) {
        qDebug() << "✅ Already on the home page. No need to switch.";
        return;
    }

    stackWidget->setCurrentWidget(homePageWidget);
    stackWidget->update();
    stackWidget->repaint();
    qDebug() << "✅ Successfully switched to home page!";
}


// ✅ Destructor (Prevent Memory Leak)
AdminHomePage::~AdminHomePage() {
    delete registerUser;
    delete course;
    delete users;
    delete profilePage;
    delete stackWidget;
}
