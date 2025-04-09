#ifndef CREATECOURSE_H
#define CREATECOURSE_H
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QPixmap>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include "TopBar.h"

class createcourse : public QWidget {
Q_OBJECT
public:
    explicit createcourse(QWidget *parent = nullptr,QWidget *topBar=nullptr);
    void reset();
    ~createcourse();
private:
    // TopBar *topbar;
    QLineEdit *courseNameField;
    QLineEdit *courseCodeField;
    QLineEdit *descriptionField;
    QComboBox *instructorDropdown;
    QComboBox *departmentDropdown;
signals:
    void backButtonClicked();
};

#endif // CREATECOURSE_H


