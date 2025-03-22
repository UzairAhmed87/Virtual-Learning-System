#ifndef TEACHERHOMEPAGE_H
#define TEACHERHOMEPAGE_H

#include <QWidget>
#include "TopBar.h"
#include "ProfilePage.h"
#include <QStackedWidget>
#include <QString>
class TeacherHomePage : public QWidget {
    Q_OBJECT
private:
    TopBar *topBar;
    QWidget *homePageWidget;
    ProfilePage *profilePage;
    QStackedWidget *stackWidget;
    QString email;
public:
    explicit TeacherHomePage(const QString &userEmail="",QWidget *parent = nullptr);
    ~TeacherHomePage();
private slots:
    // void handleButtonClick(const QString &buttonText);
    // void gotoBackPage();
    // void gotoHomePage();
    void openProfilePage();
};

#endif // ADMINHOMEPAGE_H
