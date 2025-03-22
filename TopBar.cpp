#include "TopBar.h"
#include "ProfilePage.h"
#include "AdminHomePage.h"

TopBar::TopBar(QWidget *parent,const QString &email) : QWidget(parent),emailID(email) {
    setFixedHeight(60);
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
    this->topLayout->setContentsMargins(10, 0, 10, 0);
    this->topLayout->setSpacing(30);
    setAttribute(Qt::WA_StyledBackground,true);
    // Logo
    QLabel *logoLabel = new QLabel(this);
    QPixmap logoPixmap("images/logo.png");
    logoPixmap = logoPixmap.scaled(115, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setFixedSize(115, 40);
    logoLabel->setStyleSheet("border: none; margin-bottom: -3px;");
    this->topLayout->addWidget(logoLabel);

    // Home Button
    QPushButton *homeButton = new QPushButton(this);
    homeButton->setIcon(QIcon("images/home.png"));
    homeButton->setIconSize(QSize(35, 35));
    homeButton->setCursor(Qt::PointingHandCursor);
    homeButton->setStyleSheet(
        "QPushButton { background: none; border: none; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        );

    // connect(homeButton,&QPushButton::clicked,this,&TopBar::onHomeClicked);

    // Profile Button
    QPushButton *profileButton = new QPushButton(this);
    QPixmap profilePixmap("images/ProfileIcon.png");
    profilePixmap = profilePixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    profileButton->setIcon(QIcon(profilePixmap));
    profileButton->setIconSize(QSize(30, 30));
    profileButton->setCursor(Qt::PointingHandCursor);
    profileButton->setStyleSheet(
        "QPushButton { border-radius: 20px; background: transparent; border: 2px solid transparent; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        );
    connect(profileButton,&QPushButton::clicked,this,&TopBar::onProfileClicked);
    // Logout Button
    QPushButton *logoutButton = new QPushButton("Logout", this);
    logoutButton->setCursor(Qt::PointingHandCursor);
    logoutButton->setStyleSheet(
        "QPushButton { color: white; background-color: #1E90FF; border: 2px solid #1E90FF;font-weight:bold; font-size: 16px; border-radius: 5px; }"
        "QPushButton:hover { background: transparent; }"
        "QPushButton:pressed { background-color: #1B263B; }"
        );

    topLayout->addStretch();
    topLayout->addWidget(homeButton);
    topLayout->addWidget(profileButton);
    topLayout->addWidget(logoutButton);
}
void TopBar::onProfileClicked(){
    emit profileClicked(emailID);
}
// void TopBar::onHomeClicked(){
//     qDebug() << "Home button clicked! Emitting signal...";
//     emit homeButtonClicked();
//     qDebug()<< "Signal emitted";
// }
