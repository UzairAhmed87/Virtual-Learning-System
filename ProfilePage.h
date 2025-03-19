#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include "TopBar.h"

class ProfilePage : public QWidget {
    Q_OBJECT

public:
    explicit ProfilePage(QWidget *parent = nullptr);


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
    TopBar *topBar;

    // Helper function to create the main content
    void createMainContent();
};

#endif // PROFILEPAGE_H
