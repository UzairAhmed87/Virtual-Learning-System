#ifndef STUDENTHOMEPAGE_H
#define STUDENTHOMEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QPixmap>
#include <QSpacerItem>
#include "TopBar.h"
#include "ProfilePage.h"
#include <QStackedWidget>
#include <QString>
#include "allCourses.h"

class StudentHomePage : public QWidget {
    Q_OBJECT

public:
    explicit StudentHomePage(const QString &userEmail="",QWidget *parent = nullptr);
    ~StudentHomePage();
private:
    // Top Bar Widgets
    QWidget *homePageWidget;
    ProfilePage *profilePage;
    QStackedWidget *stackWidget;
    QHBoxLayout *topLayout;
    QLabel *logoLabel;
    QPushButton *homeButton;
    QPushButton *profileButton;
    QPushButton *logoutButton;

    // Buttons Grid Widgets
    QWidget *buttonsWidget;
    QGridLayout *buttonsLayout;

    // Main Layout
    QVBoxLayout *mainLayout;
    TopBar *topBar;
    QString email;
    CourseWidget *allcourses;
    QString uniqueId;
    // Helper function to create the buttons grid
    void createButtonsGrid();
private slots:
    void handleButtonClick(const QString &buttonText);
    void gotoBackPage();
    void gotoHomePage();
    void openProfilePage();
signals:
    void logoutRequested();
};

#endif // STUDENTHOMEPAGE_H
