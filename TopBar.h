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

private:
    QHBoxLayout *topLayout;
};

#endif // TOPBAR_H
