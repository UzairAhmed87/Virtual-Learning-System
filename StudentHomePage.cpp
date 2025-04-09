#include "StudentHomePage.h"
#include <QApplication>
#include "TopBar.h"
#include "ProfilePage.h"
#include "DatabaseManager.h"
#include <QMessageBox>
StudentHomePage::StudentHomePage(const QString &userEmail,QWidget *parent) : QWidget(parent),email(userEmail),profilePage(nullptr),allcourses(nullptr) {
    setWindowTitle("Student Home Page");
    setMinimumSize(800, 500);
    setStyleSheet("background-color: #0d1b2a;");

    // Create the top bar
    topBar = new TopBar(this);
    stackWidget = new QStackedWidget(this);
    stackWidget->setContentsMargins(0,0,0,0);
    homePageWidget = new QWidget(this);
    homePageWidget->setContentsMargins(0,0,0,0);
    // Create the buttons grid
    createButtonsGrid();

    // Main Layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // Remove margins
    mainLayout->setSpacing(0); // Remove extra spacing
    mainLayout->addWidget(topBar);
    mainLayout->addWidget(buttonsWidget);
    homePageWidget->setLayout(mainLayout);
    stackWidget->addWidget(homePageWidget);
    QVBoxLayout *mainLayoutBox = new QVBoxLayout(this);
    mainLayoutBox->setContentsMargins(0,0,0,0);
    mainLayoutBox->setSpacing(0);
    mainLayoutBox->addWidget(stackWidget);

    connect(topBar,&TopBar::profileClicked,this,&StudentHomePage::openProfilePage);
     connect(topBar, &TopBar::homeButtonClicked, this, &StudentHomePage::gotoHomePage);
    connect(topBar, &TopBar::logoutClicked, this, [this]() {
        emit logoutRequested();
        qDebug()<<"Logout signal emitted";
    });
    setLayout(mainLayoutBox);
}



void StudentHomePage::createButtonsGrid() {
    buttonsWidget = new QWidget(this);
    buttonsLayout = new QGridLayout(buttonsWidget);
    buttonsLayout->setContentsMargins(40, 30, 40, 30);
    buttonsLayout->setHorizontalSpacing(25);
    buttonsLayout->setVerticalSpacing(25);

    QStringList buttonLabels = {
        "Enrolled Courses", "Lectures",
         "Assignments","Grades"
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
         connect(button, &QPushButton::clicked, this, [=]() { handleButtonClick(buttonLabels[i]); });
    }
    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection is not open!");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT unique_id FROM vls_schema.users WHERE email = :email");
    query.bindValue(":email",email);
    qDebug() << email;
    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to retrieve users: " + query.lastError().text());
        return;
    }
    if (query.next()) {  // Move to the first result
        uniqueId = query.value(0).toString();
        qDebug() << "Retrieved uniqueId:" << uniqueId;
    } else {
        qDebug() << "No user found with email:" << email;
    }  qDebug() << uniqueId;
}
void StudentHomePage::handleButtonClick(const QString &buttonText) {
    qDebug() << "Button Clicked:" << buttonText; // Debugging Output
    if (buttonText == "Lectures") {

    } else if (buttonText == "Enrolled Courses") {
        if(!allcourses)
        {
            allcourses = new CourseWidget(uniqueId,this,topBar);
            stackWidget->addWidget(allcourses);
            qDebug() << "Opening All Courses Page...";
            connect(allcourses, &CourseWidget::backButtonClicked, this, &StudentHomePage::gotoBackPage);
        }
        topBar->setParent(allcourses);
        stackWidget->setCurrentWidget(allcourses);
            qDebug() << "Opening Enrolled courses Page...";


        // Future: Open All Users page
    } else if (buttonText == "Grades") {


        // Future: Open Create Course form
    } else if (buttonText == "Assignments") {
        qDebug() << "Opening All assignments Page...";

    }}
void StudentHomePage::gotoBackPage() {
    topBar->setParent(homePageWidget);
    topBar->move(0, 0);
    topBar->show();
    mainLayout->insertWidget(0, topBar);  // Add this line to properly position the topBar

    stackWidget->setCurrentIndex(0);
}
void StudentHomePage::openProfilePage(){
    if(!profilePage){
        qDebug()<<"forming profile page";
        profilePage = new ProfilePage(email,this,topBar);
        stackWidget->addWidget(profilePage);
    }
    qDebug()<<"profile page already there";
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
        topBar->setParent(this);
        mainLayout->insertWidget(0, topBar);
        topBar->show();
    }
    topBar->setVisible(true);
    qDebug() << "TopBar Parent:" << topBar->parent();
    stackWidget->setCurrentWidget(homePageWidget);
    qDebug() << "✅ Successfully switched to home page!";
}

StudentHomePage::~StudentHomePage(){
    delete stackWidget;
};
