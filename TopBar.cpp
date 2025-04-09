#include "TopBar.h"
#include "ProfilePage.h"
#include "AdminHomePage.h"
#include "LoginPage.h"

TopBar::TopBar(QWidget *parent,const QString &email) : QWidget(parent),emailID(email) {
    setFixedHeight(80);
    setMinimumWidth(800);
    setContentsMargins(0,0,0,0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // ✅ Force background color with stylesheet
    QPalette pal;
    pal.setColor(QPalette::Window, QColor("#1B263B")); // Use Window role instead of Background
    setAutoFillBackground(true);  // Ensure background color is applied
    setPalette(pal);
    setStyleSheet("background-color: #1B263B; padding: 5px; border-bottom: 3px solid #415a77;");

    this->topLayout = new QHBoxLayout(this);
    this->topLayout->setContentsMargins(10, 0, 30, 0);
    this->topLayout->setSpacing(50);
    setAttribute(Qt::WA_StyledBackground,true);
    // Logo
    QLabel *logoLabel = new QLabel(this);
    QPixmap logoPixmap("../images/logo.png");
    logoPixmap = logoPixmap.scaled(150, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setScaledContents(true);
    logoLabel->setMaximumSize(150, 60);
    logoLabel->setStyleSheet("border: none;");
    this->topLayout->addWidget(logoLabel);

    // Home Button
    QPushButton *homeButton = new QPushButton(this);
    homeButton->setIcon(QIcon("images/home.png"));
    homeButton->setIconSize(QSize(45, 45));
    homeButton->setCursor(Qt::PointingHandCursor);
    homeButton->setStyleSheet(
        "QPushButton { background: none; border: none; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        );

    connect(homeButton,&QPushButton::clicked,this,&TopBar::onHomeClicked);

    // Profile Button
    QPushButton *profileButton = new QPushButton(this);
    QPixmap profilePixmap("images/ProfileIcon.png");
    profilePixmap = profilePixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    profileButton->setIcon(QIcon(profilePixmap));
    profileButton->setIconSize(QSize(40,40));
    profileButton->setCursor(Qt::PointingHandCursor);
    profileButton->setStyleSheet(
        "QPushButton { border-radius: 20px; background: transparent; border: 2px solid transparent; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        );
    connect(profileButton,&QPushButton::clicked,this,&TopBar::onProfileClicked);
    // Logout Button
    QPushButton *logoutButton = new QPushButton("Logout", this);
    logoutButton->setFixedHeight(40);
    logoutButton->setCursor(Qt::PointingHandCursor);
    logoutButton->setStyleSheet(
        "QPushButton { color: white; background-color: #1E90FF; border: 2px solid #1E90FF;font-weight:bold; font-size: 16px; border-radius: 5px; }"
        "QPushButton:hover { background: transparent; }"
        "QPushButton:pressed { background-color: #1B263B; }"
        );
    connect(logoutButton, &QPushButton::clicked, this, [=]() {
        emit logoutClicked();});
    topLayout->addStretch();
    topLayout->addWidget(homeButton);
    topLayout->addWidget(profileButton);
    topLayout->addWidget(logoutButton);
}
void TopBar::onProfileClicked(){
    emit profileClicked(emailID);
}
void TopBar::onHomeClicked(){
    qDebug() << "Home button clicked! Emitting signal...";
    emit homeButtonClicked();
    qDebug()<< "Signal emitted";
}
