#include "allCourses.h"
#include "DatabaseManager.h"
#include <QMessageBox>
#include <QVector>
#include <QString>
#include <QDebug>
#include "mesgboxutil.h"
CourseWidget::CourseWidget(const QString &ID,QWidget *parent,QWidget *topBar) : QWidget(parent),uniqueId(ID) {
    this->setMinimumSize(800, 500);

    setStyleSheet("background-color: #0D1B2A ; color: white;");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // topbar = new TopBar(this);

    QWidget *topBarPlaceholder = new QWidget();
    topBarPlaceholder->setFixedHeight(topBar->height());
    mainLayout->addWidget(topBarPlaceholder, 0, Qt::AlignTop);
    QWidget *contentWidget = new QWidget(this);
    contentWidget->setStyleSheet("background-color: #0D1B2A;");
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);

    // Courses
    struct Course {
        QString name;
        QString code;
        QString teacher;
    };
    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Database connection is not open!",
            "Database Error",
            "OK"
            );
        return;
    }

    QVector<Course> courses;
    if(uniqueId != ""){
        QSqlQuery Teacherquery;
        Teacherquery.prepare("SELECT course_name,course_code,teacher_id FROM vls_schema.courses WHERE teacher_id = :uniqueId");
        Teacherquery.bindValue(":uniqueId", uniqueId);

        if(!Teacherquery.exec()) {
            MessageBoxUtil::showCustomMessage(
                this,
                "Failed to retrieve courses: " + Teacherquery.lastError().text(),
                "Database Error",
                "OK"
                );
            return;
        }

        while (Teacherquery.next()) {
            Course course;
            course.name = Teacherquery.value(0).toString();
            course.code = Teacherquery.value(1).toString();

            // Fetch teacher's name based on teacher_id
            QSqlQuery teacherQuery;
            teacherQuery.prepare("SELECT first_name,last_name FROM vls_schema.users WHERE unique_id = :id");
            teacherQuery.bindValue(":id", Teacherquery.value(2));

            if (teacherQuery.exec() && teacherQuery.next()) {
                QString firstName = teacherQuery.value(0).toString();
                QString lastName =  teacherQuery.value(1).toString();
                QString fullName = firstName + lastName;
                course.teacher = fullName;
            } else {
                course.teacher = "Unknown";  // Fallback if teacher not found
            }

            courses.append(course);
        }
    } else {
        QSqlQuery query;
        query.prepare("SELECT course_name,course_code,teacher_id FROM vls_schema.courses");

        if(!query.exec()) {
            MessageBoxUtil::showCustomMessage(
                this,
                "Failed to retrieve courses: " + query.lastError().text(),
                "Database Error",
                "OK"
                );
            return;
        }


        while (query.next()) {
            Course course;
            course.name = query.value(0).toString();
            course.code = query.value(1).toString();

            // Fetch teacher's name based on teacher_id
            QSqlQuery teacherQuery;
            teacherQuery.prepare("SELECT first_name,last_name FROM vls_schema.users WHERE unique_id = :id");
            teacherQuery.bindValue(":id", query.value(2));

            if (teacherQuery.exec() && teacherQuery.next()) {
                QString firstName = teacherQuery.value(0).toString();
                QString lastName =  teacherQuery.value(1).toString();
                QString fullName = firstName + lastName;
                course.teacher = fullName;
            } else {
                course.teacher = "Unknown";  // Fallback if teacher not found
            }

            courses.append(course);
        }}
    int totalCourses = courses.size();

    // Scroll Area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet(
        "QScrollArea { border: none; }"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #212121;"
        "    width: 8px;"
        "    margin: 4px 0px 4px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #888;"
        "    min-height: 20px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 10px;"
        "    background: #444;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #212121;"
        "}"
        );

    QWidget *scrollWidget = new QWidget();
    QGridLayout *scrollLayout = new QGridLayout(scrollWidget);  // Grid layout for 2 columns
    scrollLayout->setContentsMargins(0, 0, 0, 0);
    scrollLayout->setSpacing(20); // Or adjust to what looks nice
    scrollLayout->setAlignment(Qt::AlignTop);
    // **Back Button in Scrollable Layout**
    QPushButton *backButton = new QPushButton(this);
    backButton->setIcon(QIcon("images/back_arrow.png"));
    backButton->setIconSize(QSize(30, 30));
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton { background: none; border: none; padding: 5px; }"
        "QPushButton:hov      // courseButton->setFixedSize(600,150);er { background-color: rgba(255, 255, 255, 0.2); border-radius: 5px; }"
        "QPushButton:pressed { background-color: rgba(255, 255, 255, 0.3); border-radius: 5px; }"
        );

    connect(backButton, &QPushButton::clicked, this, [=](){
        emit backButtonClicked();
    });
    mainLayout->addWidget(backButton, 0, Qt::AlignLeft);
    // backButton->setGeometry(10, 70, 40, 40);
    QLabel *titleLabel = new QLabel("Your Courses", scrollWidget);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; background: transparent;");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *totalLabel = new QLabel("Total Courses: " + QString::number(totalCourses), scrollWidget);
    totalLabel->setStyleSheet("font-size: 14px; color: gray; background: transparent;");
    totalLabel->setAlignment(Qt::AlignLeft);
    QWidget *headerWidget = new QWidget();
    QVBoxLayout *headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setSpacing(20);
    headerLayout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    headerLayout->addWidget(totalLabel, 0, Qt::AlignLeft);
    scrollLayout->addWidget(headerWidget, 0, 0, 1, 3);  // Now you control it cleanly
     // Total courses text

    // Gradient Colors (Dark & Muted Shades)
    QStringList gradientColors = {
        "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #1976D2, stop:1 #0D47A1)", // Deep Blue
        "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #1E88E5, stop:1 #1565C0)", // Lighter but Deep Blue
        "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #283593, stop:1 #1A237E)", // Royal Blue
        "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #0D47A1, stop:1 #082567)", // Dark Navy Blue
        "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #37474F, stop:1 #263238)", // Dark Greyish Blue
        "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #607D8B, stop:1 #455A64)", // Blue-Grey
        "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #546E7A, stop:1 #37474F)", // Muted Blue-Grey
    };


    // Add courses in two columns
    int row = 1;
    int col = 0;

    for (const Course &course : courses) {
        int randomIndex = QRandomGenerator::global()->bounded(gradientColors.size());
        QString randomGradient = gradientColors[randomIndex];

        QPushButton *courseButton = new QPushButton();
        courseButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        courseButton->setFixedSize(350, 120);  // Adjust width for two columns
        courseButton->setStyleSheet(
            "QPushButton {"
            "background: " + randomGradient + ";"
                               "color: white;"
                               "font-size: 22px;"
                               "font-weight: bold;"
                               "padding: 20px 30px;"  // More padding (top/bottom: 20px, left/right: 30px)
                               "border-radius: 12px;"
                               "text-align: left;"
                               "}"
                               "QPushButton:pressed {"
                               "background: rgba(255, 255, 255, 0.2);"
                               "}"
            );



        QVBoxLayout *buttonLayout = new QVBoxLayout(courseButton);
        QLabel *courseNameLabel = new QLabel(course.name);
        courseNameLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white; background: transparent; padding-left: 20px;");
        courseNameLabel->setWordWrap(true);
        QHBoxLayout *bottomLayout = new QHBoxLayout();
        QLabel *teacherLabel = new QLabel(course.teacher);
        teacherLabel->setStyleSheet("font-size: 16px; color: white; background: transparent; padding-left: 20px;");
        QLabel *codeLabel = new QLabel(course.code);
        codeLabel->setStyleSheet("font-size: 14px; color: white; background: transparent; padding-right: 20px;");
        bottomLayout->addWidget(teacherLabel);
        bottomLayout->addStretch();
        bottomLayout->addWidget(codeLabel);

        buttonLayout->addWidget(courseNameLabel);
        buttonLayout->addLayout(bottomLayout);

        scrollLayout->addWidget(courseButton, row, col, Qt::AlignTop);

        col++;
        if (col >= 3) {  // Move to next row after 2 columns
            col = 0;
            row++;
        }
    }
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    contentLayout->addWidget(scrollArea);
    mainLayout->addWidget(contentWidget);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(15);
    setLayout(mainLayout);
}
