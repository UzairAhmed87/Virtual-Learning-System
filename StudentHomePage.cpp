#include "StudentHomePage.h"
#include <QApplication>

StudentHomePage::StudentHomePage(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Student Home Page");
    setMinimumSize(800, 500);
    setStyleSheet("background-color: #0d1b2a;");

    // Create the top bar
    createTopBar();

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

void StudentHomePage::createTopBar() {
    topBar = new QWidget(this);
    topBar->setFixedHeight(60);
    topBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Full width
    topBar->setStyleSheet(
        "background-color: #1B263B; padding: 5px; border-bottom: 3px solid #415a77;"
        );

    topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(10, 0, 10, 0);
    topLayout->setSpacing(15);
    topBar->setLayout(topLayout);

    // Logo
    logoLabel = new QLabel(topBar);
    QPixmap logoPixmap("/home/uzair/Desktop/adminhome/images/logo.png");
    logoPixmap = logoPixmap.scaled(115, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setFixedSize(115, 40);
    logoLabel->setStyleSheet("border: none; margin-bottom: -3px;");

    // Home Button
    homeButton = new QPushButton(topBar);
    homeButton->setIcon(QIcon("/home/uzair/Desktop/adminhome/images/home.png"));
    homeButton->setIconSize(QSize(35, 35));
    homeButton->setCursor(Qt::PointingHandCursor);
    homeButton->setStyleSheet(
        "QPushButton { background: none; border: none; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        );

    // Profile Button
    profileButton = new QPushButton(topBar);
    QPixmap profilePixmap("/home/uzair/Desktop/adminhome/images/ProfileIcon.png");
    profilePixmap = profilePixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    profileButton->setIcon(QIcon(profilePixmap));
    profileButton->setIconSize(QSize(30, 30));
    profileButton->setCursor(Qt::PointingHandCursor);
    profileButton->setStyleSheet(
        "QPushButton {"
        "border-radius: 20px; background: transparent; border: 2px solid transparent;"
        "}"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        );

    // Logout Button
    logoutButton = new QPushButton("Logout", topBar);
    logoutButton->setCursor(Qt::PointingHandCursor);
    logoutButton->setStyleSheet(
        "QPushButton {"
        "   color: #1b263b; "
        "   background-color: #778da9; "
        "   border: 2px solid #778da9; "
        "   font-size: 16px; "
        "border-radius : 5px; "

        "} "
        "QPushButton:hover { "
        " color : white;"
        "background: transparent;"
        "}"
        );
    // Add widgets to the top bar
    topLayout->addWidget(logoLabel);
    topLayout->addStretch();
    topLayout->addWidget(homeButton);
    topLayout->addWidget(profileButton);
    topLayout->addWidget(logoutButton);
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
