#ifndef COURSEWIDGET_H
#define COURSEWIDGET_H

#include <QWidget>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QGridLayout>
#include "TopBar.h"
#include "QResizeEvent"

class CourseWidget : public QWidget {
    Q_OBJECT
public:
    explicit CourseWidget(const QString &uniqueId = "",QWidget *parent = nullptr,QWidget *topBar=nullptr);

protected:
    // void resizeEvent(QResizeEvent *event) override;

private:
    // TopBar* topbar;
    QLabel* titleLabel;
    QPushButton* backButton;
    QLabel *totalCoursesLabel;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QGridLayout *gridLayout;
    QString uniqueId;

    struct Course {
        QString name;
        QString code;
        QString teacher;
    };

    int maxColumns = 2; // Default column count
    QVector<Course> courses;

    void setupUI();
    void addCourses();
    void updateLayout(int width);

signals:
    void backButtonClicked();
};

#endif // COURSEWIDGET_H
