#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QPixmap>
#include <QSpacerItem>
#include "TeacherHomePage.h"
#include "DatabaseManager.h"
#include "TopBar.h"
#include <QMessageBox>

TeacherHomePage::TeacherHomePage(const QString &userEmail,QWidget *parent ) : QWidget(parent),email(userEmail),lecture(nullptr),pdf(nullptr),allcourses(nullptr),profilePage(nullptr) {
        setWindowTitle("Teacher Home Page");
        setMinimumSize(800, 500);
        setStyleSheet("background-color: #0d1b2a;");

        // Top Bar
        topBar = new TopBar(this);
        stackWidget = new QStackedWidget(this);
        stackWidget->setContentsMargins(0,0,0,0);

        homePageWidget = new QWidget(this);
        homePageWidget->setContentsMargins(0,0,0,0);
        // Buttons Grid
        QWidget *buttonsWidget = new QWidget(this);
        QGridLayout *buttonsLayout = new QGridLayout(buttonsWidget);
        buttonsLayout->setContentsMargins(40, 30, 40, 30);
        buttonsLayout->setHorizontalSpacing(25);
        buttonsLayout->setVerticalSpacing(25);

        QStringList buttonLabels = {
            "Upload Lecture", "Upload Assignment",
            "All Lectures", "All Assignments",
            "View Submitted Assignments"
        };

        for (int i = 0; i < buttonLabels.size() - 1; i++) {  // Loop for first 4 buttons
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
            QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
            if (!db.isOpen()) {
                QMessageBox::critical(this, "Database Error", "Database connection is not open!");
                return;
            }
            QSqlQuery query;
            query.prepare("SELECT unique_id FROM vls_schema.users WHERE email = :email");
            query.bindValue(":email",email);
            if (!query.exec()) {
                QMessageBox::critical(this, "Database Error", "Failed to retrieve users: " + query.lastError().text());
                return;
            }
            if (query.next()) {  // Move to the first result
                uniqueId = query.value(0).toString();
            } else {
                qDebug() << "No user found with email:" << email;
            }
        }

        // Center the last button with less width
        QPushButton *lastButton = new QPushButton(buttonLabels.last());
        lastButton->setFixedHeight(70);
        lastButton->setMinimumWidth(300);
        lastButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
          // Reduce width for better alignment
        lastButton->setCursor(Qt::PointingHandCursor);
        lastButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #1b263b; color: white; font-size: 18px; font-weight: bold;"
            "   border-radius: 10px; padding: 12px; border: 2px solid #415a77;"
            "}"
            "QPushButton:hover { background-color: #2d3e50; border-color: #778da9; }"
            "QPushButton:pressed { background-color: #0f1925; }"
            );

        // Use a horizontal layout to center it properly
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addStretch();  // Push it to center
        hLayout->addWidget(lastButton);
        hLayout->addStretch();  // Push it to center

        // Add this horizontal layout to the grid at the next row
        buttonsLayout->addLayout(hLayout, 2, 0, 1, 2);


        // Main Layout
        mainLayout = new QVBoxLayout(this);
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
         connect(topBar,&TopBar::profileClicked,this,&TeacherHomePage::openProfilePage);
        connect(topBar, &TopBar::homeButtonClicked, this, &TeacherHomePage::gotoHomePage);
         connect(topBar, &TopBar::logoutClicked, this, [this]() {
             emit logoutRequested();
             qDebug()<<"Logout signal emitted";
         });
        setLayout(mainLayoutBox);

    }
void TeacherHomePage::handleButtonClick(const QString &buttonText) {
    qDebug() << "Button Clicked:" << buttonText; // Debugging Output
    if (buttonText == "Upload Lecture") {
        if(!lecture){
            lecture = new UploadLecture(uniqueId,this,topBar);
            stackWidget->addWidget(lecture);
            connect(lecture,&UploadLecture::backButtonClicked,this,&TeacherHomePage::gotoBackPage);
        }
        topBar->setParent(lecture);
        stackWidget->setCurrentWidget(lecture);
    } else if (buttonText == "All Lectures"||buttonText == "All Assignments") {
        if(!allcourses)
        {
            allcourses = new CourseWidget(uniqueId,this,topBar);
            stackWidget->addWidget(allcourses);
            qDebug() << "Opening All Courses Page...";
            connect(allcourses, &CourseWidget::backButtonClicked, this, &TeacherHomePage::gotoBackPage);
        }
        topBar->setParent(allcourses);
        stackWidget->setCurrentWidget(allcourses);
    } else if (buttonText == "Upload Assignment") {
        if(!pdf)
        {
            pdf = new UploadPdf(uniqueId,this,topBar);
            stackWidget->addWidget(pdf);
             connect(pdf, &UploadPdf::backButtonClicked, this, &TeacherHomePage::gotoBackPage);
        }
        topBar->setParent(pdf);
        stackWidget->setCurrentWidget(pdf);

    } else if (buttonText == "Assignments") {
        qDebug() << "Opening All assignments Page...";

    }}
void TeacherHomePage::gotoBackPage() {
    topBar->setParent(homePageWidget);
    topBar->move(0, 0);
    topBar->show();
    mainLayout->insertWidget(0, topBar);  // Add this line to properly position the topBar

    stackWidget->setCurrentIndex(0);
}
    void TeacherHomePage::openProfilePage(){
        if(!profilePage){
        profilePage = new ProfilePage(email,this,topBar);
            stackWidget->addWidget(profilePage);
        }
        topBar->setParent(profilePage);
        stackWidget->setCurrentWidget(profilePage);
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
        // mainLayout->removeWidget(topBar);
        // mainLayout->insertWidget(0, topBar);
        qDebug() << "✅ Successfully switched to home page!";
    }
    TeacherHomePage ::~TeacherHomePage(){
        delete stackWidget;
    };
