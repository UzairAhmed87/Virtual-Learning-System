#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QIcon>
#include <QString>
class TopBar : public QWidget {
    Q_OBJECT
public:
    explicit TopBar(QWidget *parent = nullptr,const QString &email="");

signals:
    void profileClicked(const QString &email);
    void homeButtonClicked();
    void logoutClicked();
private:
    QHBoxLayout *topLayout;
    QString emailID;
private slots:
    void onProfileClicked();
    void onHomeClicked();
};

#endif // TOPBAR_H
