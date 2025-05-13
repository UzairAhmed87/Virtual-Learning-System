#ifndef ALLCOURSES_H
#define ALLCOURSES_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QStringList>
#include <QVector>
#include <QSqlQuery>
#include <QSqlError>
#include <QComboBox>
#include <QDialog>
#include <QRandomGenerator>

class CourseWidget : public QWidget {
    Q_OBJECT

public:
    explicit CourseWidget(const QString &ID = "", QWidget *parent = nullptr, QWidget *topBar = nullptr);

    // Structure for course data
    struct CourseData {
        QString name;
        QString code;
        QString department;
        QString teacher;
        QString teacherId;
        QString studentsEnrolled;
    };

signals:
    void backButtonClicked();

private slots:
    void searchCourse();
    void refreshTable();
    void filterCourses();
    void deleteCourse(int row);
    void updateCourse(int row);

private:
    void setupUI(QWidget *topBar);
    void loadCourses();
    void populateTable(const QVector<CourseData> &courses);
    void updateCoursesCount();
    bool eventFilter(QObject *obj, QEvent *event) override;

    // UI Components
    QLabel *heading;
    QLabel *totalCoursesLabel;
    QLineEdit *searchBar;
    QPushButton *refreshBtn;
    QPushButton *filterBtn;
    QTableWidget *table;

    // Data
    QString uniqueId; // Teacher ID if showing specific teacher's courses
    QVector<CourseData> coursesData; // Store courses for filtering
};

#endif // ALLCOURSES_H
