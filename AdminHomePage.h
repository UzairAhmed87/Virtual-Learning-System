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
#include "allCourses.h"
#include "enrollStudent.h"
class AdminHomePage : public QWidget {
    Q_OBJECT
private slots:
    void handleButtonClick(const QString &buttonText);
    void gotoBackPage();
    void gotoHomePage();
    void openProfilePage();
    // void handleLogout();
public:
    explicit AdminHomePage(const QString &userEmail="",QWidget *parent = nullptr);
    ~AdminHomePage();
    // In AdminHomePage.h (and similarly for other pages)
signals:
    void logoutRequested();
private:
    QWidget *homePageWidget;
    ProfilePage *profilePage;
    QStackedWidget *stackWidget;
    QVBoxLayout *mainLayout;
    RegisterUserForm *registerUser;
    createcourse *course;
    CourseWidget *allcourses;
    EnrollStudent *enroll;
    AllUsers *users;
    QString email;
    TopBar *topBar;
};

#endif // ADMINHOMEPAGE_H
