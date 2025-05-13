#include "allCourses.h"
#include "DatabaseManager.h"
#include "TopBar.h"
#include <QMessageBox>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QHeaderView>
#include <QPropertyAnimation>
#include <QEvent>
#include "mesgboxutil.h"

CourseWidget::CourseWidget(const QString &ID, QWidget *parent, QWidget *topBar) : QWidget(parent), uniqueId(ID) {
    setupUI(topBar);
    loadCourses();
}

void CourseWidget::setupUI(QWidget *topBar) {
    this->setMinimumSize(800, 500);
    this->setStyleSheet("background-color: #0D1B2A;");  // Dark blue background

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(15);

    // Add topbar placeholder
    QWidget *topBarPlaceholder = new QWidget();
    topBarPlaceholder->setFixedHeight(topBar->height());
    mainLayout->addWidget(topBarPlaceholder, 0, Qt::AlignTop);

    // Back button
    QPushButton *backButton = new QPushButton;
    backButton->setIcon(QIcon("images/back_arrow.png")); // Use an appropriate left arrow icon
    backButton->setIconSize(QSize(30, 30));
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton { background: none; border: none; padding: 5px; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        "QPushButton:pressed { background-color: rgba(255, 255, 255, 0.3); border-radius: 5px; }"
        );
    connect(backButton, &QPushButton::clicked, this, [=](){
        emit backButtonClicked();
    });
    mainLayout->addWidget(backButton, 0, Qt::AlignLeft);

    // Heading
    heading = new QLabel("All Courses", this);
    heading->setStyleSheet("font-size: 22px; color: white; font-weight: bold;");
    heading->setContentsMargins(0, 0, 0, 0);
    heading->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(heading);

    // Search, Filter, Refresh options
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search course...");
    searchBar->setStyleSheet(
        "QLineEdit {"
        "   background-color: #2C3E50;"  // Default color (dark blue)
        "   color: white;"
        "   border: 1px solid #34495E;"
        "   padding: 5px;"
        "   border-radius: 5px;"
        "}"
        "QLineEdit:focus {"
        "   background-color: #1B263B;"  // Active color (lighter blue)
        "   border: 1px solid #1B263B;"
        "}"
        );
    searchBar->installEventFilter(this);
    searchBar->setContentsMargins(20, 0, 20, 0);

    refreshBtn = new QPushButton("Refresh", this);
    filterBtn = new QPushButton("Filter", this);
    QString buttonStyle =
        "QPushButton {"
        "    background-color: #1A2B50; color: white; padding: 6px; border-radius: 5px;"
        "     border: 1px solid transparent;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0D1B2A; border: 1px solid #FFFFFF;"
        "}";

    refreshBtn->setStyleSheet(buttonStyle);
    filterBtn->setStyleSheet(buttonStyle);
    filterBtn->setContentsMargins(0, 0, 20, 0);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchBar);
    searchLayout->addWidget(refreshBtn);
    searchLayout->addWidget(filterBtn);
    mainLayout->addLayout(searchLayout);

    // Table Setup
    table = new QTableWidget(0, 6, this);
    table->setHorizontalHeaderLabels({"Code", "Name", "Department", "Teacher", "Students Enrolled", "Actions"});
    table->setStyleSheet("background-color: #1B263B; color: white; font-size: 14px;");
    table->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: #162c5d; color: white; font-weight: bold; }");
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    table->horizontalHeader()->setStretchLastSection(true); // Makes the last column fill remaining space

    table->setColumnWidth(0, 100);  // Code column
    table->setColumnWidth(1, 250);  // Name column - wider to fit course names
    table->setColumnWidth(2, 250);  // Department column
    table->setColumnWidth(3, 200);  // Teacher column
    table->setColumnWidth(4, 150);  // Students Enrolled column
    mainLayout->addWidget(table);

    // Total courses label
    totalCoursesLabel = new QLabel("Total Courses: 0", this);
    totalCoursesLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: white; background-color:#1B263B;");
    mainLayout->addWidget(totalCoursesLabel, 0, Qt::AlignLeft);

    setLayout(mainLayout);

    // Connect signals and slots
    connect(searchBar, &QLineEdit::textChanged, this, &CourseWidget::searchCourse);
    connect(refreshBtn, &QPushButton::clicked, this, &CourseWidget::refreshTable);
    connect(filterBtn, &QPushButton::clicked, this, &CourseWidget::filterCourses);
}

void CourseWidget::loadCourses() {
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

    QVector<CourseData> courses;

    if (uniqueId != "") {
        // Load courses for a specific teacher
        QSqlQuery Teacherquery;
        Teacherquery.prepare("SELECT course_name, course_code, teacher_id, department FROM vls_schema.courses WHERE teacher_id = :uniqueId");
        Teacherquery.bindValue(":uniqueId", uniqueId);

        if (!Teacherquery.exec()) {
            MessageBoxUtil::showCustomMessage(
                this,
                "Failed to retrieve courses: " + Teacherquery.lastError().text(),
                "Database Error",
                "OK"
                );
            return;
        }

        while (Teacherquery.next()) {
            CourseData course;
            course.name = Teacherquery.value(0).toString();
            course.code = Teacherquery.value(1).toString();
            course.department = Teacherquery.value(3).toString();
            course.teacherId = Teacherquery.value(2).toString();

            // Fetch teacher's name based on teacher_id
            QSqlQuery teacherQuery;
            teacherQuery.prepare("SELECT first_name, last_name FROM vls_schema.users WHERE unique_id = :id");
            teacherQuery.bindValue(":id", course.teacherId);

            if (teacherQuery.exec() && teacherQuery.next()) {
                QString firstName = teacherQuery.value(0).toString();
                QString lastName = teacherQuery.value(1).toString();
                course.teacher = firstName + " " + lastName;
            } else {
                course.teacher = "Unknown";  // Fallback if teacher not found
            }

            // Get student count (this is a placeholder - implement according to your DB schema)
            QSqlQuery studentCountQuery;
            studentCountQuery.prepare("SELECT COUNT(*) FROM vls_schema.course_enrollments WHERE course_code = :code");
            studentCountQuery.bindValue(":code", course.code);

            if (studentCountQuery.exec() && studentCountQuery.next()) {
                course.studentsEnrolled = studentCountQuery.value(0).toString();
            } else {
                course.studentsEnrolled = "0";
            }

            courses.append(course);
        }
    } else {
        // Load all courses
        QSqlQuery query;
        query.prepare("SELECT course_name, course_code, teacher_id, department FROM vls_schema.courses");

        if (!query.exec()) {
            MessageBoxUtil::showCustomMessage(
                this,
                "Failed to retrieve courses: " + query.lastError().text(),
                "Database Error",
                "OK"
                );
            return;
        }

        while (query.next()) {
            CourseData course;
            course.name = query.value(0).toString();
            course.code = query.value(1).toString();
            course.department = query.value(3).toString();
            course.teacherId = query.value(2).toString();

            // Fetch teacher's name based on teacher_id
            QSqlQuery teacherQuery;
            teacherQuery.prepare("SELECT first_name, last_name FROM vls_schema.users WHERE unique_id = :id");
            teacherQuery.bindValue(":id", course.teacherId);

            if (teacherQuery.exec() && teacherQuery.next()) {
                QString firstName = teacherQuery.value(0).toString();
                QString lastName = teacherQuery.value(1).toString();
                course.teacher = firstName + " " + lastName;
            } else {
                course.teacher = "Unknown";  // Fallback if teacher not found
            }

            // Get student count (this is a placeholder - implement according to your DB schema)
            QSqlQuery studentCountQuery;
            studentCountQuery.prepare("SELECT COUNT(*) FROM vls_schema.course_enrollments WHERE course_code = :code");
            studentCountQuery.bindValue(":code", course.code);

            if (studentCountQuery.exec() && studentCountQuery.next()) {
                course.studentsEnrolled = studentCountQuery.value(0).toString();
            } else {
                course.studentsEnrolled = "0";
            }

            courses.append(course);
        }
    }

    // Store courses for filtering
    coursesData = courses;

    // Populate table with courses
    populateTable(courses);
}

void CourseWidget::populateTable(const QVector<CourseData> &courses) {
    table->setRowCount(0);  // Clear existing rows

    for (const CourseData &course : courses) {
        int row = table->rowCount();
        table->insertRow(row);

        // Add course data to table
        table->setItem(row, 0, new QTableWidgetItem(course.code));
        table->setItem(row, 1, new QTableWidgetItem(course.name));
        table->setItem(row, 2, new QTableWidgetItem(course.department));
        table->setItem(row, 3, new QTableWidgetItem(course.teacher));
        table->setItem(row, 4, new QTableWidgetItem(course.studentsEnrolled));

        // Create Update button
        QPushButton *updateBtn = new QPushButton("Update");
        updateBtn->setStyleSheet("background: green; color: white;");
        connect(updateBtn, &QPushButton::clicked, this, [this, row]() { updateCourse(row); });

        // Create Delete button
        QPushButton *deleteBtn = new QPushButton("Delete");
        deleteBtn->setStyleSheet("background: #FF5C5C; color: white;");
        connect(deleteBtn, &QPushButton::clicked, this, [this, row]() { deleteCourse(row); });

        // Add buttons to a widget
        QWidget *btnWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(btnWidget);
        layout->addWidget(updateBtn);
        layout->addWidget(deleteBtn);
        layout->setAlignment(Qt::AlignCenter);
        layout->setSpacing(5);
        layout->setContentsMargins(0, 0, 0, 0);
        btnWidget->setLayout(layout);

        // Add buttons to the actions column (last column)
        table->setCellWidget(row, 5, btnWidget);
    }

    updateCoursesCount(); // Update count after populating table
}

void CourseWidget::deleteCourse(int row) {
    if (row < 0 || row >= table->rowCount()) return;

    QString courseCode = table->item(row, 0)->text();

    int confirm = QMessageBox::question(this, "Delete Course", "Are you sure you want to delete this course?");
    if (confirm == QMessageBox::Yes) {
        // Delete from database
        QSqlQuery query;
        query.prepare("DELETE FROM vls_schema.courses WHERE course_code = :code");
        query.bindValue(":code", courseCode);

        if (query.exec()) {
            table->removeRow(row);
            updateCoursesCount(); // Update count after deleting
            MessageBoxUtil::showCustomMessage(this, "Course deleted successfully!", "Success", "OK");
        } else {
            MessageBoxUtil::showCustomMessage(
                this,
                "Failed to delete course: " + query.lastError().text(),
                "Database Error",
                "OK"
                );
        }
    }
}

void CourseWidget::updateCourse(int row) {
    if (row < 0 || row >= table->rowCount()) return;

    QString courseCode = table->item(row, 0)->text();

    // Improved styling for the dialog box
    QString style = R"(
        QDialog {
            background-color: #0D1B2A;
            color: white;
            border: 2px solid #4a90e2;
            border-radius: 10px;
        }
        QLineEdit, QComboBox {
            background-color: #1e1e1e;
            color: #ffffff;
            padding: 8px;
            border: 1px solid #4a90e2;
            border-radius: 5px;
            font-size: 14px;
            min-height: 25px;
        }
        QLabel {
            color: white;
            font-size: 18px;
            font-weight: bold;
            margin-top: 10px;
            margin-bottom: 5px;
        }
        QPushButton {
            background-color: #4a90e2;
            color: white;
            border-radius: 5px;
            padding: 12px;
            font-size: 16px;
            font-weight: bold;
            margin-top: 20px;
        }
        QPushButton:hover {
            background-color: #3b7fc4;
        }
    )";

    QDialog dialog(this);
    dialog.setStyleSheet(style);
    dialog.setWindowTitle("Update Course Teacher");
    dialog.setFixedSize(400, 200); // Reduced size since we only need one field

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    // Add some spacing at the top for better appearance
    layout->addSpacing(10);

    // Get teachers from database for dropdown
    QSqlQuery teachersQuery;
    teachersQuery.prepare("SELECT unique_id, first_name, last_name FROM vls_schema.users WHERE role = 'teacher'");

    QLabel* teacherLabel = new QLabel("Select new teacher:");
    QComboBox* teacherCombo = new QComboBox();

    QString currentTeacherId;

    if (teachersQuery.exec()) {
        while (teachersQuery.next()) {
            QString teacherId = teachersQuery.value(0).toString();
            QString teacherName = teachersQuery.value(1).toString() + " " + teachersQuery.value(2).toString();
            teacherCombo->addItem(teacherName, teacherId);
        }
    } else {
        // Fallback with dummy data if query fails
        teacherCombo->addItem("Wajih uddin");
        teacherCombo->addItem("Samia Masood");
        teacherCombo->addItem("Furqan Ali");
        teacherCombo->addItem("Mubashir Khan");
        teacherCombo->addItem("Jawad Ali");
    }

    // Set current teacher as selected if possible
    QString currentTeacher = table->item(row, 3)->text();
    int index = teacherCombo->findText(currentTeacher);
    if (index >= 0) {
        teacherCombo->setCurrentIndex(index);
    }

    layout->addWidget(teacherLabel);
    layout->addWidget(teacherCombo);

    QPushButton* saveButton = new QPushButton("Save");
    layout->addWidget(saveButton);

    // Add some spacing at the bottom for better appearance
    layout->addSpacing(10);

    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        QString newTeacherName = teacherCombo->currentText();
        QString newTeacherId = teacherCombo->currentData().toString();

        if (!newTeacherName.isEmpty()) {
            // Update in database
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE vls_schema.courses SET teacher_id = :teacherId WHERE course_code = :code");
            updateQuery.bindValue(":teacherId", newTeacherId);
            updateQuery.bindValue(":code", courseCode);

            if (updateQuery.exec()) {
                table->item(row, 3)->setText(newTeacherName);
                MessageBoxUtil::showCustomMessage(this, "Course teacher updated successfully!", "Success", "OK");
                dialog.accept();
            } else {
                MessageBoxUtil::showCustomMessage(
                    this,
                    "Failed to update course: " + updateQuery.lastError().text(),
                    "Database Error",
                    "OK"
                    );
            }
        }
    });

    dialog.exec();
}

void CourseWidget::searchCourse() {
    QString keyword = searchBar->text().toLower();
    for (int i = 0; i < table->rowCount(); ++i) {
        bool codeMatch = table->item(i, 0)->text().toLower().contains(keyword);  // Search in Code column (index 0)
        bool nameMatch = table->item(i, 1)->text().toLower().contains(keyword);  // Search in Name column (index 1)

        table->setRowHidden(i, !(codeMatch || nameMatch));  // Show row if either matches
    }
}

void CourseWidget::refreshTable() {
    searchBar->clear();
    loadCourses(); // Reload courses from database
}

bool CourseWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == searchBar) {
        if (event->type() == QEvent::Enter) {
            searchBar->setStyleSheet(
                "background-color: #1B263B; color: white; padding: 6px; border-radius: 5px;"
                "border: 1px solid white;"
                );
        } else if (event->type() == QEvent::Leave) {
            searchBar->setStyleSheet(
                "background-color: #1A2B50; color: white; padding: 6px; border-radius: 5px;"
                "border: 1px solid transparent;"
                );
        }
    }
    return QWidget::eventFilter(obj, event);
}

void CourseWidget::updateCoursesCount() {
    if (!table) return; // Ensure table exists
    int rowCount = table->rowCount();
    totalCoursesLabel->setText("Total Courses: " + QString::number(rowCount));
}

void CourseWidget::filterCourses() {
    // Apply styling for dialog
    QString style = R"(
        QDialog {
            background-color: #0D1B2A;
            color: white;
            border: 2px solid #4a90e2;
            border-radius: 10px;
        }
        QLineEdit, QComboBox {
            background-color: #1e1e1e;
            color: #ffffff;
            padding: 8px;
            border: 1px solid #4a90e2;
            border-radius: 5px;
            font-size: 14px;
            min-height: 25px;
        }
        QLabel {
            color: white;
            font-size: 18px;
            font-weight: bold;
            margin-top: 10px;
            margin-bottom: 5px;
        }
        QPushButton {
            background-color: #4a90e2;
            color: white;
            border-radius: 5px;
            padding: 12px;
            font-size: 16px;
            font-weight: bold;
            margin-top: 20px;
        }
        QPushButton:hover {
            background-color: #3b7fc4;
        }
    )";

    // Create department filter dialog
    QDialog filterDialog(this);
    filterDialog.setWindowTitle("Filter by Department");
    filterDialog.setFixedSize(400, 200);
    filterDialog.setStyleSheet(style);

    QVBoxLayout* filterLayout = new QVBoxLayout(&filterDialog);
    filterLayout->addSpacing(10);

    QLabel* filterLabel = new QLabel("Choose a Department:", &filterDialog);
    filterLayout->addWidget(filterLabel);

    QComboBox* filterCombo = new QComboBox(&filterDialog);
    // Add "All" option
    filterCombo->addItem("All");

    // Add department options from database
    QSqlQuery deptQuery;
    deptQuery.prepare("SELECT DISTINCT department FROM vls_schema.courses ORDER BY department");

    if (deptQuery.exec()) {
        while (deptQuery.next()) {
            QString dept = deptQuery.value(0).toString();
            if (!dept.isEmpty()) {
                filterCombo->addItem(dept);
            }
        }
    } else {
        // Fallback with common departments if query fails
        filterCombo->addItem("Computer Science");
        filterCombo->addItem("Mathematics");
        filterCombo->addItem("CIS");
    }

    filterLayout->addWidget(filterCombo);

    QPushButton* applyButton = new QPushButton("Apply Filter", &filterDialog);
    filterLayout->addWidget(applyButton);
    filterLayout->addSpacing(10);

    QString selectedDepartment;
    QObject::connect(applyButton, &QPushButton::clicked, [&]() {
        selectedDepartment = filterCombo->currentText();
        filterDialog.accept();
    });

    if (filterDialog.exec() == QDialog::Accepted && !selectedDepartment.isEmpty()) {
        QVector<CourseData> filteredCourses;

        // Filter courses by department
        for (const CourseData &course : coursesData) {
            if (selectedDepartment == "All" || course.department == selectedDepartment) {
                filteredCourses << course;
            }
        }

        populateTable(filteredCourses);
    }
}
