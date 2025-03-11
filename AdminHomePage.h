#ifndef ADMINHOMEPAGE_H
#define ADMINHOMEPAGE_H

#include <QWidget>
#include "registerUser.h"
#include "TopBar.h"
#include <QStackedWidget>
class AdminHomePage : public QWidget {
    Q_OBJECT
private slots:
    void handleButtonClick(const QString &buttonText);
    void gotoBackPage();
public:
    explicit AdminHomePage(QWidget *parent = nullptr);
    ~AdminHomePage();
private:
    QStackedWidget *stackWidget;
    RegisterUserForm *registerUser;
    TopBar *topBar;
};

#endif // ADMINHOMEPAGE_H
