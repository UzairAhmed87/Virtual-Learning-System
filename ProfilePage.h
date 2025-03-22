#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include "TopBar.h"
#include <QString>

class ProfilePage : public QWidget {
    Q_OBJECT

public:
    explicit ProfilePage(const QString &email,QWidget *parent = nullptr);


public slots:
    void uploadPicture();  // Declare the uploadPicture slot
signals:
    void homeButtonClicked(); // Signal for home button click

private:

    // Main Content Widgets
    QWidget *mainContent;
    QVBoxLayout *mainLayout;
    QLabel *profilePictureLabel;
    QLabel *nameLabel;
    QLabel *emailLabel;
    QLabel *enrollmentIdLabel;
    QString uniqueId;
    QString role;
    QString email;
    QString gender;
    QString department;
    QString phone;
    QString fullName;
    QString feeStatus;
    QString batch;
    QString firstName;
    QString lastName;
    TopBar *topBar;

    // Helper function to create the main content
    void createMainContent();
};

#endif // PROFILEPAGE_H
