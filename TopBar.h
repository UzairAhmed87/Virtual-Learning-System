#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QIcon>
class TopBar : public QWidget {
    Q_OBJECT
public:
    explicit TopBar(QWidget *parent = nullptr);

signals:
    void profileClicked();
    void homeButtonClicked();
private:
    QHBoxLayout *topLayout;
private slots:
    void onProfileClicked();
    void onHomeClicked();
};

#endif // TOPBAR_H
