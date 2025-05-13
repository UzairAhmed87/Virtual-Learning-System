#include "adminhomepage.h"
#include <QApplication>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
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
#include "registerUser.h"
#include "TopBar.h"
#include "createcourse.h"
#include "allusers.h"
#include "ProfilePage.h"
#include "mainwindow.h"
#include "enrollStudent.h"
#include <QTimer>

// Updated constructor to match header file declaration
AdminHomePage::AdminHomePage(const QString &userEmail, QWidget *parent)
    : QWidget(parent),
    registerUser(nullptr),
    course(nullptr),
    users(nullptr),
    allcourses(nullptr),
    email(userEmail),
    enroll(nullptr),
    profilePage(nullptr) {

    setWindowTitle("Admin Home Page");
    setMinimumSize(800, 500);
    setStyleSheet("background-color: #0d1b2a;");

    // Create top bar and stacked widget
    topBar = new TopBar(this, email);
    stackWidget = new QStackedWidget(this);
    stackWidget->setContentsMargins(0, 0, 0, 0);

    // Create the home page widget
    homePageWidget = new QWidget(this);
    homePageWidget->setContentsMargins(0, 0, 0, 0);

    // Create buttons and UI elements
    createButtonsGrid();
    createWelcomeWidget();

    // Setup the main layout for home page
    mainLayout = new QVBoxLayout(homePageWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(topBar);
    mainLayout->addWidget(welcomeWidget);
    mainLayout->addWidget(buttonsWidget);

    // Add home page to stacked widget
    stackWidget->addWidget(homePageWidget);

    // Main layout for the entire widget
    QVBoxLayout *mainLayoutBox = new QVBoxLayout(this);
    mainLayoutBox->setContentsMargins(0, 0, 0, 0);
    mainLayoutBox->setSpacing(0);
    mainLayoutBox->addWidget(stackWidget);

    // Connect signals and slots
    connect(topBar, &TopBar::profileClicked, this, &AdminHomePage::openProfilePage);
    connect(topBar, &TopBar::homeButtonClicked, this, &AdminHomePage::gotoHomePage);
    connect(topBar, &TopBar::logoutClicked, this, [this]() {
        emit logoutRequested();
        qDebug() << "Logout signal emitted";
    });

    setLayout(mainLayoutBox);

    if (!topBar) {
        qDebug() << "❌ topBar is NULL!";
    } else {
        qDebug() << "✅ topBar initialized!";
    }
}

void AdminHomePage::createWelcomeWidget() {
    welcomeWidget = new QWidget(this);
    welcomeWidget->setStyleSheet("background: transparent;");
    QHBoxLayout *welcomeLayout = new QHBoxLayout(welcomeWidget);
    welcomeLayout->setContentsMargins(40, 25, 40, 25);

    // Rainbow gradient border container
    QWidget *borderContainer = new QWidget();
    borderContainer->setStyleSheet(
        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,"
        "stop:0 #FF0000, stop:0.16 #FF7F00, stop:0.33 #FFFF00,"
        "stop:0.5 #00FF00, stop:0.66 #0000FF, stop:0.83 #4B0082, stop:1 #9400D3);"
        "border-radius: 14px;"
        "padding: 3px;"
        );

    QWidget *welcomeContainer = new QWidget(borderContainer);
    welcomeContainer->setStyleSheet(
        "background-color: rgba(27, 38, 59, 0.95);"
        "border-radius: 12px;"
        );

    QHBoxLayout *containerLayout = new QHBoxLayout(welcomeContainer);
    containerLayout->setContentsMargins(25, 15, 25, 15);
    containerLayout->setSpacing(20);

    // Emoji without any border
    QLabel *emojiLabel = new QLabel("👨‍💼");  // Changed to admin icon
    emojiLabel->setStyleSheet(
        "font-size: 40px;"
        "background: transparent;"
        "padding: 0;"
        "margin: 0;"
        );
    emojiLabel->setAlignment(Qt::AlignCenter);

    // Welcome text with administrative title
    QLabel *welcomeLabel = new QLabel(
        "<div style='font-size: 24px; font-weight: bold; color: #e0e1dd;'>Administrator Control Panel</div>"
        "<div style='font-size: 18px; color: #a3b2c8; margin-top: 8px;'>System Management & Analytics</div>"
        );
    welcomeLabel->setStyleSheet("background: transparent;");
    welcomeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    containerLayout->addWidget(emojiLabel);
    containerLayout->addWidget(welcomeLabel);
    containerLayout->addStretch();

    QHBoxLayout *borderLayout = new QHBoxLayout(borderContainer);
    borderLayout->setContentsMargins(0, 0, 0, 0);
    borderLayout->addWidget(welcomeContainer);

    // Enhanced shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(25);
    shadow->setColor(QColor(0, 0, 0, 180));
    shadow->setOffset(5, 5);
    borderContainer->setGraphicsEffect(shadow);

    welcomeLayout->addWidget(borderContainer);
}

void AdminHomePage::createButtonsGrid() {
    buttonsWidget = new QWidget(this);
    buttonsLayout = new QGridLayout(buttonsWidget);
    buttonsLayout->setContentsMargins(60, 20, 60, 40);
    buttonsLayout->setHorizontalSpacing(30);
    buttonsLayout->setVerticalSpacing(30);

    // Button configuration: text, icon color, and hover color - updated for admin functionality
    struct ButtonConfig {
        QString text;
        QString iconColor;
        QString hoverColor;
        QString iconPath;
    };

    QVector<ButtonConfig> buttons = {
        {"Register User", "#4285F4", "#3367D6", ""},
        {"All Users", "#0F9D58", "#0B8043", ""},
        {"Create Course", "#9C27B0", "#7B1FA2", ""},
        {"All Courses", "#F4B400", "#F09300", ""},
        {"Enroll Student", "#F4B400", "#F09300", ""},
        {"Update Fee Status", "#DB4437", "#C53929", ""}  // Added the missing button
    };

    for (int i = 0; i < buttons.size(); i++) {
        QPushButton *button = new QPushButton(buttons[i].text);
        button->setFixedSize(350, 100);
        button->setCursor(Qt::PointingHandCursor);

        button->setStyleSheet(QString(
                                  "QPushButton {"
                                  "   background-color: #1b263b;"
                                  "   color: white;"
                                  "   font-size: 18px;"
                                  "   font-weight: bold;"
                                  "   border-radius: 10px;"
                                  "   padding: 15px 20px;"
                                  "   border: 2px solid %1;"
                                  "   text-align: left;"
                                  "}"
                                  "QPushButton:hover {"
                                  "   background-color: #2d3e50;"
                                  "   border-color: %2;"
                                  "}"
                                  "QPushButton:pressed {"
                                  "   background-color: #0f1925;"
                                  "}"
                                  "QPushButton:focus {"
                                  "   outline: none;"
                                  "}"
                                  ).arg(buttons[i].iconColor, buttons[i].hoverColor));

        QString iconText;
        if (buttons[i].text == "Register User") iconText = "👤";
        else if (buttons[i].text == "All Users") iconText = "👥";
        else if (buttons[i].text == "All Courses") iconText = "📚";
        else if (buttons[i].text == "Create Course") iconText = "📊";
        else if (buttons[i].text == "Enroll Student") iconText = "🧑‍🎓";
        else iconText = "💰";  // For Update Fee Status

        QHBoxLayout *buttonLayout = new QHBoxLayout(button);
        buttonLayout->setContentsMargins(15, 0, 15, 0);

        QLabel *emojiLabel = new QLabel(iconText);
        emojiLabel->setStyleSheet(QString("font-size: 24px; color: %1; background: transparent;").arg(buttons[i].iconColor));

        QLabel *textLabel = new QLabel(buttons[i].text);
        textLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold; background: transparent; border: none;");

        buttonLayout->addWidget(emojiLabel);
        buttonLayout->addWidget(textLabel);
        buttonLayout->addStretch();

        button->setText("");

        // Connect button clicks to handler
        connect(button, &QPushButton::clicked, this, [this, text=buttons[i].text]() {
            this->handleButtonClick(text);
        });

        buttonsLayout->addWidget(button, i / 2, i % 2);
    }
}

// Handle button clicks and page navigation
void AdminHomePage::handleButtonClick(const QString &buttonText) {
    qDebug() << "Button Clicked:" << buttonText; // Debugging Output

    if (buttonText == "Register User") {
        if (!registerUser) { // Ensure no memory leak
            registerUser = new RegisterUserForm(this, topBar);
            stackWidget->addWidget(registerUser);
            connect(registerUser, &RegisterUserForm::backButtonClicked, this, &AdminHomePage::gotoBackPage);
        }
        topBar->setParent(registerUser);
        qDebug() << "TopBar Parent:" << topBar->parent();
        stackWidget->setCurrentWidget(registerUser);

    } else if (buttonText == "All Users") {
        if (!users) { // Ensure no memory leak
            users = new AllUsers(this, topBar);
            stackWidget->addWidget(users);
            qDebug() << "Opening All Users Page...";
            connect(users, &AllUsers::backButtonClicked, this, &AdminHomePage::gotoBackPage);
        }
        topBar->setParent(users);
        qDebug() << "TopBar Parent:" << topBar->parent();
        stackWidget->setCurrentWidget(users);

    } else if (buttonText == "Create Course") {
        if (!course) { // Ensure no memory leak
            course = new createcourse(this, topBar);
            stackWidget->addWidget(course);
            qDebug() << "Opening Create Course Form...";
            connect(course, &createcourse::backButtonClicked, this, &AdminHomePage::gotoBackPage);
        }
        topBar->setParent(course);
        qDebug() << "TopBar Parent:" << topBar->parent();
        stackWidget->setCurrentWidget(course);

    } else if (buttonText == "All Courses") {
        if (!allcourses) {
            allcourses = new CourseWidget("", this, topBar);
            stackWidget->addWidget(allcourses);
            qDebug() << "Opening All Courses Page...";
            connect(allcourses, &CourseWidget::backButtonClicked, this, &AdminHomePage::gotoBackPage);
        }
        topBar->setParent(allcourses);
        qDebug() << "TopBar Parent:" << topBar->parent();
        stackWidget->setCurrentWidget(allcourses);

    } else if (buttonText == "Enroll Student") {
        if (!enroll) {
            enroll = new EnrollStudent(this, topBar);
            stackWidget->addWidget(enroll);
            qDebug() << "Opening Enrollment Page...";
            connect(enroll, &EnrollStudent::backButtonClicked, this, &AdminHomePage::gotoBackPage);
        }
        topBar->setParent(enroll);
        qDebug() << "TopBar Parent:" << topBar->parent();
        stackWidget->setCurrentWidget(enroll);

    } else if (buttonText == "Update Fee Status") {
        qDebug() << "Opening Fee Status Update Form...";
        // Future: Open Fee Status update form
    }
}

void AdminHomePage::gotoBackPage() {
    topBar->setParent(homePageWidget);
    topBar->move(0, 0);
    topBar->show();
    mainLayout->insertWidget(0, topBar);  // Add this line to properly position the topBar

    stackWidget->setCurrentIndex(0);
}

void AdminHomePage::openProfilePage() {
    if (!profilePage) {
        profilePage = new ProfilePage(email, this, topBar);
        stackWidget->addWidget(profilePage);
    }
    topBar->setParent(profilePage);
    stackWidget->setCurrentWidget(profilePage);
}

void AdminHomePage::gotoHomePage() {
    qDebug() << "🏠 Home Button Clicked! Going back to home page...";
    if (stackWidget->currentWidget() == homePageWidget) {
        qDebug() << "✅ Already on the home page. No need to switch.";
        return;
    }

    // FIX: Ensure proper reparenting of topBar
    if (topBar->parent() != homePageWidget) {
        topBar->setParent(homePageWidget);
        topBar->move(0, 0);
        topBar->show();
        mainLayout->insertWidget(0, topBar);
    }
    stackWidget->setCurrentWidget(homePageWidget);
}

// Destructor (Prevent Memory Leak)
AdminHomePage::~AdminHomePage() {
    delete stackWidget;
}
