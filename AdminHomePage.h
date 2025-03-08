#ifndef ADMINHOMEPAGE_H
#define ADMINHOMEPAGE_H

#include <QWidget>
#include "registerUser.h"
#include "TopBar.h"
class AdminHomePage : public QWidget {
    Q_OBJECT
private slots:
    void handleButtonClick(const QString &buttonText);

public:
    explicit AdminHomePage(QWidget *parent = nullptr);
    ~AdminHomePage();
private:
    RegisterUserForm *registerUser;
    TopBar *topBar;
};

#endif // ADMINHOMEPAGE_H
