#ifndef ENROLLSTUDENT_H
#define ENROLLSTUDENT_H

#include <QWidget>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QCheckBox>
#include "TopBar.h"

class EnrollStudent : public QWidget {
    Q_OBJECT

public:
    explicit EnrollStudent(QWidget *parent = nullptr,QWidget *topBar = nullptr);

private slots:
    void onStudentSelected(int index);
    void enrollStudent();
    void setupUI();
    // void loadCoursesForDepartment(const QString &department);

private:
    QComboBox *studentDropdown;
    QListWidget *courseListWidget;
    QPushButton *enrollButton;
    QPushButton *backButton;
    QLabel *headingLabel;
    QLabel *logoLabel;
    QWidget *topBarWidget;
    QLabel *departmentLabel;
    QLabel *departmentDisplay;
    QMap<QString, QString> studentDepartments;  // Hardcoded student data
    QString getStudentDepartment(const QString &studentId);

    void loadStudents();
    void loadCoursesForStudent(const QString &studentId);
signals:
    void backButtonClicked();

};

#endif // ENROLLSTUDENT_H
