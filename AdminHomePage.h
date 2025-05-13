#ifndef ADMINHOMEPAGE_H
#define ADMINHOMEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QPixmap>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QString>
#include "registerUser.h"
#include "createcourse.h"
#include "TopBar.h"
#include "allusers.h"
#include "ProfilePage.h"
#include "allCourses.h"
#include "enrollStudent.h"

class AdminHomePage : public QWidget {
    Q_OBJECT

public:
    explicit AdminHomePage(const QString &userEmail="", QWidget *parent = nullptr);
    ~AdminHomePage();

signals:
    void logoutRequested();

private slots:
    void handleButtonClick(const QString &buttonText);
    void gotoBackPage();
    void gotoHomePage();
    void openProfilePage();

    //protected:
    // bool eventFilter(QObject *obj, QEvent *event) override;

private:
    // UI Components
    QWidget *homePageWidget;
    QWidget *buttonsWidget;
    QWidget *welcomeWidget;
    QVBoxLayout *mainLayout;
    QGridLayout *buttonsLayout;
    QStackedWidget *stackWidget;
    TopBar *topBar;

    // Page components
    RegisterUserForm *registerUser;
    createcourse *course;
    AllUsers *users;
    CourseWidget *allcourses;
    ProfilePage *profilePage;
    EnrollStudent *enroll;

    // User data
    QString email;

    // Helper methods
    void createButtonsGrid();
    void createWelcomeWidget();
    QGraphicsDropShadowEffect* createGlowEffect(QWidget *widget);
    void animateButtonClick(QPushButton *button);
};

#endif // ADMINHOMEPAGE_H
