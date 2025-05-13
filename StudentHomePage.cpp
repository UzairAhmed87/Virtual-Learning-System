#include "StudentHomePage.h"
#include <QApplication>
#include "TopBar.h"
#include "ProfilePage.h"
#include "DatabaseManager.h"
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include "StudentAllCourses.h"

StudentHomePage::StudentHomePage(const QString &userEmail, QWidget *parent)
    : QWidget(parent), email(userEmail), profilePage(nullptr), allcourses(nullptr),
    allLecturesPage(nullptr), allAssignmentsPage(nullptr) {
    setWindowTitle("Student Home Page");
    setMinimumSize(900, 600);
    setStyleSheet("background-color: #0d1b2a;");

    // Initialize main widgets
    stackWidget = new QStackedWidget(this);
    homePageWidget = new QWidget();
    topBar = new TopBar(homePageWidget);

    // Create the welcome widget and buttons grid
    createWelcomeWidget();
    createButtonsGrid();
    setupMainLayout();

    // Stack setup
    stackWidget->addWidget(homePageWidget);
    QVBoxLayout *mainLayoutBox = new QVBoxLayout(this);
    mainLayoutBox->setContentsMargins(0, 0, 0, 0);
    mainLayoutBox->setSpacing(0);
    mainLayoutBox->addWidget(stackWidget);

    // Connections
    connect(topBar, &TopBar::profileClicked, this, &StudentHomePage::openProfilePage);
    connect(topBar, &TopBar::homeButtonClicked, this, &StudentHomePage::gotoHomePage);
    connect(topBar, &TopBar::logoutClicked, this, [this]() {
        emit logoutRequested();
        qDebug() << "Logout signal emitted";
    });

    // Database initialization
    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection is not open!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT unique_id FROM vls_schema.users WHERE email = :email");
    query.bindValue(":email", email);
    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to retrieve users: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        uniqueId = query.value(0).toString();
        qDebug() << "Retrieved uniqueId:" << uniqueId;
    } else {
        qDebug() << "No user found with email:" << email;
    }
}

void StudentHomePage::setupMainLayout() {
    mainLayout = new QVBoxLayout(homePageWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Add widgets to main layout with proper stretch factors
    mainLayout->addWidget(topBar);
    mainLayout->addWidget(welcomeWidget, 0);
    mainLayout->addWidget(buttonsWidget, 1);

    // Set size policies to ensure proper expansion
    welcomeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    buttonsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void StudentHomePage::createWelcomeWidget() {
    welcomeWidget = new QWidget(homePageWidget);
    welcomeWidget->setStyleSheet("background: transparent;");
    welcomeWidget->setFixedHeight(300);
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
    QLabel *emojiLabel = new QLabel("🚀");
    emojiLabel->setStyleSheet(
        "font-size: 40px;"
        "background: transparent;"
        "padding: 0;"
        "margin: 0;"
        );
    emojiLabel->setAlignment(Qt::AlignCenter);

    // Welcome text with new title
    QLabel *welcomeLabel = new QLabel(
        "<div style='font-size: 24px; font-weight: bold; color: #e0e1dd;'>Welcome to Evolved Education</div>"
        "<div style='font-size: 18px; color: #a3b2c8; margin-top: 8px;'>Ready to learn?</div>"
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

void StudentHomePage::createButtonsGrid() {
    buttonsWidget = new QWidget(homePageWidget);
    buttonsLayout = new QGridLayout(buttonsWidget);
    buttonsLayout->setContentsMargins(60, 20, 60, 40);
    buttonsLayout->setHorizontalSpacing(30);
    buttonsLayout->setVerticalSpacing(30);

    // Button configuration: text, icon color, and hover color
    struct ButtonConfig {
        QString text;
        QString iconColor;
        QString hoverColor;
    };

    QVector<ButtonConfig> buttons = {
        {"Grades", "#4CAF50", "#388E3C"},
        {"Lectures", "#2196F3", "#1976D2"},
        {"Enrolled Courses", "#9C27B0", "#7B1FA2"},
        {"Assignments", "#FF9800", "#F57C00"}
    };

    for (int i = 0; i < buttons.size(); i++) {
        QPushButton *button = new QPushButton(buttons[i].text, buttonsWidget);
        button->setFixedSize(450, 120);
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
                                  ).arg(buttons[i].iconColor, buttons[i].hoverColor));

        QString iconText;
        if (buttons[i].text == "Grades") iconText = "📊";
        else if (buttons[i].text == "Lectures") iconText = "🎓";
        else if (buttons[i].text == "Enrolled Courses") iconText = "📚";
        else iconText = "📝";

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

        connect(button, &QPushButton::clicked, this, [=]() { handleButtonClick(buttons[i].text); });
        buttonsLayout->addWidget(button, i / 2, i % 2);
    }
}

void StudentHomePage::handleButtonClick(const QString &buttonText) {
    qDebug() << "Button Clicked:" << buttonText;
    if (buttonText == "Lectures") {
        if(!allLecturesPage) {
            allLecturesPage = new UploadPage(this,topBar,uniqueId);
            stackWidget->addWidget(allLecturesPage);
            qDebug() << "Opening All Lectures Page...";
             connect(allLecturesPage, &UploadPage::backButtonClicked, this, &StudentHomePage::gotoBackPage);
        }
        topBar->setParent(allLecturesPage);
        stackWidget->setCurrentWidget(allLecturesPage);
    } else if (buttonText == "Enrolled Courses") {
        if(!allcourses) {
            allcourses = new StudentsAllCourses(uniqueId, this, topBar);
            stackWidget->addWidget(allcourses);
            qDebug() << "Opening All Courses Page...";
            connect(allcourses, &StudentsAllCourses::backButtonClicked, this, &StudentHomePage::gotoBackPage);
        }
        topBar->setParent(allcourses);
        stackWidget->setCurrentWidget(allcourses);
    } else if (buttonText == "Grades") {
        // Implement grades page similarly
    } else if (buttonText == "Assignments") {
        if(!allAssignmentsPage) {
            allAssignmentsPage = new AllAssignments(this,topBar);
            stackWidget->addWidget(allAssignmentsPage);
            qDebug() << "Opening All Assignments Page...";
             connect(allAssignmentsPage, &AllAssignments::backButtonClicked, this, &StudentHomePage::gotoBackPage);
        }
        topBar->setParent(allAssignmentsPage);
        stackWidget->setCurrentWidget(allAssignmentsPage);
    }
}

void StudentHomePage::gotoBackPage() {
    topBar->setParent(homePageWidget);
    topBar->move(0, 0);
    topBar->show();
    mainLayout->insertWidget(0, topBar);
    stackWidget->setCurrentIndex(0);
}

void StudentHomePage::openProfilePage() {
    if(!profilePage) {
        qDebug() << "forming profile page";
        profilePage = new ProfilePage(email, this, topBar);
        stackWidget->addWidget(profilePage);
    }
    qDebug() << "profile page already there";
    topBar->setParent(profilePage);
    stackWidget->setCurrentWidget(profilePage);
}

void StudentHomePage::gotoHomePage() {
    qDebug() << "🏠 Home Button Clicked! Going back to home page...";

    if (stackWidget->currentWidget() == homePageWidget) {
        qDebug() << "✅ Already on the home page. No need to switch.";
        return;
    }
    if (topBar->parent() != this) {
        topBar->setParent(homePageWidget);
        mainLayout->insertWidget(0, topBar);
        topBar->show();
    }
    topBar->setVisible(true);
    qDebug() << "TopBar Parent:" << topBar->parent();
    stackWidget->setCurrentWidget(homePageWidget);
    qDebug() << "✅ Successfully switched to home page!";
}

StudentHomePage::~StudentHomePage() {
    delete stackWidget;
}
