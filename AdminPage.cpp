
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

AdminHomePage::AdminHomePage(QWidget *parent) : QWidget(parent), registerUser(nullptr) {
    setWindowTitle("Admin Home Page");
    setMinimumSize(800, 500);
    setStyleSheet("background-color: #0d1b2a;");

    topBar = new TopBar(this);
    stackWidget = new QStackedWidget(this);
    stackWidget->setContentsMargins(0,0,0,0);
    // ✅ Buttons Grid
    QWidget *homePageWidget = new QWidget(this);
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
            "   transition: 0.3s; }"
            "QPushButton:hover { background-color: #2d3e50; border-color: #778da9; box-shadow: 0px 0px 10px rgba(119, 141, 169, 0.6); }"
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
        qDebug() << "Opening All Users Page...";
        // Future: Open All Users page
    } else if (buttonText == "Create Course") {
        qDebug() << "Opening Create Course Form...";
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

// ✅ Destructor (Prevent Memory Leak)
AdminHomePage::~AdminHomePage() {

        delete stackWidget;

}
