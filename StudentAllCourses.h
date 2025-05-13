#ifndef STUDENTSALLCOURSES_H
#define STUDENTSALLCOURSES_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>
#include <QDebug>

class StudentsAllCourses : public QWidget {
    Q_OBJECT

public:
    explicit StudentsAllCourses(const QString &ID = "", QWidget *parent = nullptr, QWidget *topBar = nullptr);

signals:
    void backButtonClicked();

private:
    QString uniqueId;
};

#endif // STUDENTSALLCOURSES_H
