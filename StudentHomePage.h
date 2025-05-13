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
#include "AllLecture.h"
#include "AllAssignments.h"
#include <QGraphicsDropShadowEffect>
#include "StudentAllCourses.h"

class StudentHomePage : public QWidget {
    Q_OBJECT

public:
    explicit StudentHomePage(const QString &userEmail="", QWidget *parent = nullptr);
    ~StudentHomePage();

private:
    // Main Widgets
    QWidget *homePageWidget;
    QWidget *welcomeWidget;
    QWidget *buttonsWidget;
    ProfilePage *profilePage;
    QStackedWidget *stackWidget;
    TopBar *topBar;
    StudentsAllCourses *allcourses;
    UploadPage *allLecturesPage;
    AllAssignments *allAssignmentsPage;  // Added for assignments page

    // Layouts
    QVBoxLayout *mainLayout;
    QGridLayout *buttonsLayout;

    // Data
    QString email;
    QString uniqueId;

    // Helper functions
    void createWelcomeWidget();
    void createButtonsGrid();
    void setupMainLayout();

private slots:
    void handleButtonClick(const QString &buttonText);
    void gotoBackPage();
    void gotoHomePage();
    void openProfilePage();

signals:
    void logoutRequested();
};

#endif // STUDENTHOMEPAGE_H
