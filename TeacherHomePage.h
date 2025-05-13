#ifndef TEACHERHOMEPAGE_H
#define TEACHERHOMEPAGE_H
#include <QWidget>
#include "TopBar.h"
#include "ProfilePage.h"
#include <QStackedWidget>
#include <QString>
#include <QVBoxLayout>
#include "allCourses.h"
#include "uploadpdf.h"
#include "uploadlecture.h"
#include "AllLecture.h"
#include "allassignments.h"
// Add this to your TeacherHomePage.h file

class TeacherHomePage : public QWidget {
    Q_OBJECT

public:
    TeacherHomePage(const QString &userEmail, QWidget *parent = nullptr);
    ~TeacherHomePage();

signals:
    void logoutRequested();

private slots:
    void handleButtonClick(const QString &buttonText);
    void gotoBackPage();
    void openProfilePage();
    void gotoHomePage();

private:
    void setupUI();

    QString email;  // Store email for authentication/identification

    QVBoxLayout *mainLayout;
    QStackedWidget *stackWidget;
    QWidget *homePageWidget;
    QPushButton *backButton;
    TopBar *topBar;

    // Other widgets
    UploadPage *lecturesall;
    AllAssignments *allassignments;
    UploadLecture *lecture;
    UploadPdf *pdf;
    CourseWidget *allcourses;
    ProfilePage *profilePage;
    QString uniqueId;

};
#endif // TEACHERHOMEPAGE_H
