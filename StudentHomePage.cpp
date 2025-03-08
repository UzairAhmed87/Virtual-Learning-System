#include "StudentHomePage.h"
#include <QApplication>
#include "TopBar.h"

StudentHomePage::StudentHomePage(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Student Home Page");
    setMinimumSize(800, 500);
    setStyleSheet("background-color: #0d1b2a;");

    // Create the top bar
    topBar = new TopBar(this);

    // Create the buttons grid
    createButtonsGrid();

    // Main Layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // Remove margins
    mainLayout->setSpacing(0); // Remove extra spacing
    mainLayout->addWidget(topBar);
    mainLayout->addWidget(buttonsWidget);
    setLayout(mainLayout);
}



void StudentHomePage::createButtonsGrid() {
    buttonsWidget = new QWidget(this);
    buttonsLayout = new QGridLayout(buttonsWidget);
    buttonsLayout->setContentsMargins(40, 30, 40, 30);
    buttonsLayout->setHorizontalSpacing(25);
    buttonsLayout->setVerticalSpacing(25);

    QStringList buttonLabels = {
        "Grades", "Lectures",
        "Enrolled Courses", "Assignments"
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
    }
};

StudentHomePage::~StudentHomePage(){

};
