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

class StudentHomePage : public QWidget {
    Q_OBJECT

public:
    explicit StudentHomePage(QWidget *parent = nullptr);
    ~StudentHomePage();
private:
    // Top Bar Widgets
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

    // Helper function to create the top bar
    void createTopBar();


    // Helper function to create the buttons grid
    void createButtonsGrid();
};

#endif // STUDENTHOMEPAGE_H
