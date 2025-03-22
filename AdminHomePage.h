#ifndef ADMINHOMEPAGE_H
#define ADMINHOMEPAGE_H

#include <QWidget>
#include "registerUser.h"
#include "createcourse.h"
#include "TopBar.h"
#include "allusers.h"
#include "ProfilePage.h"
#include <QStackedWidget>
#include <QString>

class AdminHomePage : public QWidget {
    Q_OBJECT
private slots:
    void handleButtonClick(const QString &buttonText);
    void gotoBackPage();
    // void gotoHomePage();
    void openProfilePage();
public:
    explicit AdminHomePage(const QString &userEmail="",QWidget *parent = nullptr);
    ~AdminHomePage();
private:
    QWidget *homePageWidget;
    ProfilePage *profilePage;
    QStackedWidget *stackWidget;
    RegisterUserForm *registerUser;
    createcourse *course;
    AllUsers *users;
    QString email;
    TopBar *topBar;
};

#endif // ADMINHOMEPAGE_H
