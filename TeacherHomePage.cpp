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
#include "TopBar.h"

    TeacherHomePage::TeacherHomePage(QWidget *parent) : QWidget(parent) {
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
                "   transition: 0.3s; }"
                "QPushButton:hover { background-color: #2d3e50; border-color: #778da9; box-shadow: 0px 0px 10px rgba(119, 141, 169, 0.6); }"
                "QPushButton:pressed { background-color: #0f1925; }"
                );

            buttonsLayout->addWidget(button, i / 2, i % 2);
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
            "   transition: 0.3s; }"
            "QPushButton:hover { background-color: #2d3e50; border-color: #778da9; box-shadow: 0px 0px 10px rgba(119, 141, 169, 0.6); }"
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
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
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
        setLayout(mainLayoutBox);

    }
    void TeacherHomePage::openProfilePage(){
        profilePage = new ProfilePage(this);
        stackWidget->addWidget(profilePage);
        stackWidget->setCurrentWidget(profilePage);
    }
    TeacherHomePage ::~TeacherHomePage(){

    };
