#ifndef TEACHERHOMEPAGE_H
#define TEACHERHOMEPAGE_H

#include <QWidget>
#include "TopBar.h"

class TeacherHomePage : public QWidget {
    Q_OBJECT
private:
    TopBar *topBar;
public:
    explicit TeacherHomePage(QWidget *parent = nullptr);
    ~TeacherHomePage();
};

#endif // ADMINHOMEPAGE_H
