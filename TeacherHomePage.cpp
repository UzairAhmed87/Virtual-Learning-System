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
#include <QIcon>
#include <QGraphicsDropShadowEffect>
#include "TeacherHomePage.h"
#include "DatabaseManager.h"
#include "TopBar.h"
#include <QMessageBox>
#include "mesgboxutil.h"

TeacherHomePage::TeacherHomePage(const QString &userEmail, QWidget *parent)
    : QWidget(parent),
    email(userEmail),
    // Remove the initializers for the commented out member variables
    lecture(nullptr),
    pdf(nullptr),
    allcourses(nullptr),
    profilePage(nullptr),
    allassignments(nullptr),
    lecturesall(nullptr)
{
    // Store email for later user ID lookup - we'll retrieve the ID when needed
    email = userEmail;
    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        MessageBoxUtil::showCustomMessage(this, "Database connection is not open!", "Database Error", "OK");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT unique_id FROM vls_schema.users WHERE email = :email ");
    query.bindValue(":email", email);
    if (!query.exec()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Query execution failed: " + query.lastError().text(),
            "Database Error",
            "OK"
            );
        return;
    }
    if (query.next()) {
        uniqueId = query.value(0).toString();
}
    // We'll look up the uniqueId when needed instead of here

    setWindowTitle("Teacher Home Page");
    showFullScreen();
    setStyleSheet("background-color: #0d1b2a;");

    // Top Bar
    topBar = new TopBar(this);
    stackWidget = new QStackedWidget(this);
    stackWidget->setContentsMargins(0, 0, 0, 0);

    homePageWidget = new QWidget(this);
    homePageWidget->setContentsMargins(0, 0, 0, 0);

    setupUI();

    // Main Layout for stacked widget
    QVBoxLayout *mainLayoutBox = new QVBoxLayout(this);
    mainLayoutBox->setContentsMargins(0, 0, 0, 0);
    mainLayoutBox->setSpacing(0);
    mainLayoutBox->addWidget(stackWidget);

    connect(topBar, &TopBar::profileClicked, this, &TeacherHomePage::openProfilePage);
    connect(topBar, &TopBar::homeButtonClicked, this, &TeacherHomePage::gotoHomePage);
    connect(topBar, &TopBar::logoutClicked, this, [this]() {
        emit logoutRequested();
        qDebug() << "Logout signal emitted";
    });

    setLayout(mainLayoutBox);
}

void TeacherHomePage::setupUI() {

    // ===== Rainbow Welcome Widget =====
    QWidget *welcomeWidget = new QWidget(this);
    welcomeWidget->setStyleSheet("background: transparent;");
    QHBoxLayout *welcomeLayout = new QHBoxLayout(welcomeWidget);
    welcomeLayout->setContentsMargins(40, 0, 40, 0);

    // Rainbow border container
    QWidget *borderContainer = new QWidget();
    borderContainer->setFixedHeight(200);
    borderContainer->setStyleSheet(
        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #FF0000, stop:0.16 #FF7F00, stop:0.33 #FFFF00, "
        "stop:0.5 #00FF00, stop:0.66 #0000FF, stop:0.83 #4B0082, stop:1 #9400D3);"
        "border-radius: 14px; padding: 3px;"
        );

    QWidget *welcomeCard = new QWidget(borderContainer);
    welcomeCard->setStyleSheet(
        "background-color: rgba(27, 38, 59, 0.95);"
        "border-radius: 12px;"
        );

    QHBoxLayout *bannerLayout = new QHBoxLayout(welcomeCard);
    bannerLayout->setContentsMargins(25, 20, 25, 20);
    bannerLayout->setSpacing(20);

    QLabel *emojiLabel = new QLabel("🚀");
    emojiLabel->setStyleSheet("font-size: 40px; background: transparent;");
    emojiLabel->setAlignment(Qt::AlignCenter);

    QLabel *welcomeText = new QLabel(
        "<div style='font-size: 24px; font-weight: bold; color: #e0e1dd;'>Welcome to Evolved Education</div>"
        "<div style='font-size: 18px; color: #a3b2c8; margin-top: 8px;'>Ready to upload, Teacher?</div>"
        );
    welcomeText->setStyleSheet("background: transparent;");
    welcomeText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    bannerLayout->addWidget(emojiLabel);
    bannerLayout->addWidget(welcomeText);
    bannerLayout->addStretch();

    QHBoxLayout *borderLayout = new QHBoxLayout(borderContainer);
    borderLayout->setContentsMargins(0, 0, 0, 0);
    borderLayout->addWidget(welcomeCard);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(25);
    shadow->setColor(QColor(0, 0, 0, 180));
    shadow->setOffset(5, 5);
    borderContainer->setGraphicsEffect(shadow);

    welcomeLayout->addWidget(borderContainer);

    // ===== Main Buttons =====
    QStringList buttonLabels = {
        "📤 Upload Lecture", "📎 Upload Assignment",
        "🎞️ All Lectures", "📂 All Assignments",
        "📝 View Submitted Assignments"
    };

    QStringList colors = {
        "#00ff88", "#009dff", "#ffb300", "#bc4ed8", "#ff5050"
    };

    QGridLayout *buttonsLayout = new QGridLayout();
    buttonsLayout->setHorizontalSpacing(30);
    buttonsLayout->setVerticalSpacing(30);
    buttonsLayout->setContentsMargins(60, 20, 60, 40);

    // Create the first 4 buttons in a 2x2 grid
    for (int i = 0; i < buttonLabels.size() - 1; i++) {
        QPushButton *btn = new QPushButton(buttonLabels[i]);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedHeight(100);
        btn->setFixedWidth(450);
        btn->setStyleSheet(QString(
                               "QPushButton {"
                               "background-color: #1b263b;"
                               "color: white;"
                               "font-size: 18px;"
                               "font-weight: bold;"
                               "border: 2px solid %1;"
                               "border-radius: 12px;"
                               "padding: 12px;"
                               "}"
                               "QPushButton:hover { background-color: #27364a; }"
                               ).arg(colors[i]));

        connect(btn, &QPushButton::clicked, this, [=]() { handleButtonClick(buttonLabels[i]); });
        buttonsLayout->addWidget(btn, i / 2, i % 2);
    }

    // Create the last button centered below
    QPushButton *lastButton = new QPushButton(buttonLabels.last());
    lastButton->setCursor(Qt::PointingHandCursor);
    lastButton->setFixedHeight(100);
    lastButton->setFixedWidth(450);
    lastButton->setStyleSheet(
        "QPushButton {"
        "background-color: #1b263b;"
        "color: white;"
        "font-size: 18px;"
        "font-weight: bold;"
        "border: 2px solid #ff5050;"
        "border-radius: 12px;"
        "padding: 12px;"
        "}"
        "QPushButton:hover { background-color: #27364a; }"
        );
    connect(lastButton, &QPushButton::clicked, this, [=]() { handleButtonClick(buttonLabels.last()); });

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    centerLayout->addWidget(lastButton);
    centerLayout->addStretch();

    buttonsLayout->addLayout(centerLayout, 2, 0, 1, 2);

    // Main Layout
    mainLayout = new QVBoxLayout(homePageWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(topBar);               // Top bar (no margin)
    mainLayout->addSpacing(15);                  // Gap for back button
    // mainLayout->addWidget(backButton, 0, Qt::AlignLeft);
    mainLayout->addSpacing(20);                  // Gap before welcome
    mainLayout->addWidget(welcomeWidget);
    mainLayout->addSpacing(30);                  // Gap before buttons
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();                    // Push everything up

    stackWidget->addWidget(homePageWidget);
}

void TeacherHomePage::handleButtonClick(const QString &buttonText) {
    qDebug() << "Button Clicked:" << buttonText; // Debugging Output

    if (buttonText == "📤 Upload Lecture") {
        qDebug() << "Creating Upload Lecture page";

        try {
            if (!lecture) {
                lecture = new UploadLecture(uniqueId,this,topBar);
                stackWidget->addWidget(lecture);
                connect(lecture,&UploadLecture::backButtonClicked,this,&TeacherHomePage::gotoBackPage);
            }
            topBar->setParent(lecture);
            stackWidget->setCurrentWidget(lecture);
        } catch (const std::exception &e) {
            qDebug() << "Exception creating Upload Lecture page:" << e.what();
            QMessageBox::warning(this, "Error", "Failed to open Upload Lecture page: " + QString(e.what()));
        }
    }
    else if (buttonText == "🎞️ All Lectures") {
        qDebug() << "Opening All Courses Page";

        try {
            if (!lecturesall) {
                lecturesall = new UploadPage(this, topBar,uniqueId);  // Using email instead of uniqueId
                stackWidget->addWidget(lecturesall);
                connect(lecturesall, &UploadPage::backButtonClicked, this, &TeacherHomePage::gotoBackPage);
            }
            topBar->setParent(lecturesall);
            stackWidget->setCurrentWidget(lecturesall);
        } catch (const std::exception &e) {
            qDebug() << "Exception creating Courses page:" << e.what();
            QMessageBox::warning(this, "Error", "Failed to open Courses page: " + QString(e.what()));
        }
    }
    else if ( buttonText == "📂 All Assignments") {
        qDebug() << "Opening All Courses Page";

        try {
            if (!allassignments) {
                allassignments = new AllAssignments(this, topBar,uniqueId);  // Using email instead of uniqueId
                stackWidget->addWidget(allassignments);
                connect(allassignments, &AllAssignments::backButtonClicked, this, &TeacherHomePage::gotoBackPage);
            }
            topBar->setParent(allassignments);
            stackWidget->setCurrentWidget(allassignments);
        } catch (const std::exception &e) {
            qDebug() << "Exception creating Courses page:" << e.what();
            QMessageBox::warning(this, "Error", "Failed to open Courses page: " + QString(e.what()));
        }
    }
    else if (buttonText == "📎 Upload Assignment") {
        qDebug() << "Opening Upload PDF Page";

        try {
            if (!pdf) {
                pdf = new UploadPdf(uniqueId, this, topBar);  // Using email instead of uniqueId
                stackWidget->addWidget(pdf);
                connect(pdf, &UploadPdf::backButtonClicked, this, &TeacherHomePage::gotoBackPage);
            }
            topBar->setParent(pdf);
            stackWidget->setCurrentWidget(pdf);
        } catch (const std::exception &e) {
            qDebug() << "Exception creating Upload PDF page:" << e.what();
            QMessageBox::warning(this, "Error", "Failed to open Upload PDF page: " + QString(e.what()));
        }
    }
    else if (buttonText == "📝 View Submitted Assignments") {
        qDebug() << "Opening All assignments Page...";
        QMessageBox::information(this, "Coming Soon", "This feature is under development.");
    }
}

void TeacherHomePage::gotoBackPage() {
    topBar->setParent(homePageWidget);
    topBar->move(0, 0);
    topBar->show();
    mainLayout->insertWidget(0, topBar);  // Add this line to properly position the topBar

    stackWidget->setCurrentIndex(0);
}

void TeacherHomePage::openProfilePage() {
    try {
        if (!profilePage) {
            profilePage = new ProfilePage(email, this, topBar);
            stackWidget->addWidget(profilePage);
        }
        topBar->setParent(profilePage);
        stackWidget->setCurrentWidget(profilePage);
    } catch (const std::exception &e) {
        qDebug() << "Exception creating Profile page:" << e.what();
        QMessageBox::warning(this, "Error", "Failed to open Profile page: " + QString(e.what()));
    }
}

void TeacherHomePage::gotoHomePage() {
    qDebug() << "🏠 Home Button Clicked! Going back to home page...";

    if (stackWidget->currentWidget() == homePageWidget) {
        qDebug() << "✅ Already on the home page. No need to switch.";
        return;
    }

    if (topBar->parent() != this) {
        topBar->setParent(this);
        mainLayout->insertWidget(0, topBar);
        topBar->show();
    }

    topBar->setVisible(true);
    qDebug() << "TopBar Parent:" << topBar->parent();
    stackWidget->setCurrentWidget(homePageWidget);

    qDebug() << "✅ Successfully switched to home page!";
}

TeacherHomePage::~TeacherHomePage() {
    delete stackWidget;
}
