#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

signals:
    void loginSuccessful(QString role,QString email);  // Signal emitted when login is successful

private slots:
    void handleLogin();

private:
    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H
