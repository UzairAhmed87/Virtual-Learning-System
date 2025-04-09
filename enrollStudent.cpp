#include "enrollStudent.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QFont>
#include <QScrollArea>
#include <QCompleter>
#include <QStringListModel>
#include <QDebug> // Debugging
#include <QMessageBox>
#include "DatabaseManager.h"
#include "mesgboxutil.h"

EnrollStudent::EnrollStudent(QWidget *parent,QWidget *topbar) : QWidget(parent) {
    this->setMinimumSize(800,500);
    topBarWidget = new QWidget();
    topBarWidget->setFixedHeight(topbar->height());

    // Hardcoded student department data
    studentDepartments["CT-221"] = "Computer Science";
    studentDepartments["CT-224"] = "Software Engineering";
    studentDepartments["CT-235"] = "Electrical Engineering";
    studentDepartments["CT-244"] = "Mechanical Engineering";
    studentDepartments["CT-247"] = "Faarigh Department";

    setupUI();

    connect(studentDropdown, &QComboBox::currentIndexChanged, this, &EnrollStudent::onStudentSelected);
    connect(enrollButton, &QPushButton::clicked, this, &EnrollStudent::enrollStudent);

    loadStudents();
}

void EnrollStudent::setupUI() {
    this->setStyleSheet("background-color: #0D1B2A;");

    // Top Bar
    // topBar = new TopBar(this);

    // Back Button
    QPushButton *backButton = new QPushButton;
    backButton->setIcon(QIcon("images/back_arrow.png"));
    backButton->setIconSize(QSize(30, 30));
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton { background: none; border: none; padding: 5px; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        );
    connect(backButton, &QPushButton::clicked, this, [=](){
        emit backButtonClicked();
    });

    // **Heading**
    QLabel *headingLabel = new QLabel("Enroll Student");
    headingLabel->setAlignment(Qt::AlignCenter);
    headingLabel->setStyleSheet("font-size: 30px; font-weight: bold; color: white;");


    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(15);

    // Student Dropdown
    studentDropdown = new QComboBox();
    studentDropdown->setEditable(true);
    studentDropdown->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px; font-size: 16px; font-weight: bold;");

    // Search Completer
    QCompleter *completer = new QCompleter(studentDropdown);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->popup()->setStyleSheet(
        "QListView {"
        "background-color: #1B263B; color: white; padding: 2px; border-radius: 0px; font-size: 16px; font-weight: bold;"
        "}"
        );
    studentDropdown->setCompleter(completer);
    studentDropdown->setFixedWidth(850);

    // **Department Display**
    QLabel *departmentLabel = new QLabel("Department:");
    departmentLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #ffffff;");

    departmentDisplay = new QLabel("N/A");
    departmentDisplay->setStyleSheet("font-size: 18px; color: lightgray; font-weight: bold;");
    departmentDisplay->setAlignment(Qt::AlignLeft);

    courseListWidget = new QListWidget();
    courseListWidget->setViewMode(QListWidget::IconMode);  // Enables multi-column layout
    courseListWidget->setFlow(QListView::LeftToRight);     // Items flow from left to right
    courseListWidget->setWrapping(true);                  // Wrap items into new row
    courseListWidget->setGridSize(QSize(400, 50));        // Set item width & height
    courseListWidget->setSpacing(10);                     // Space between items

    courseListWidget->setStyleSheet(
        "QListWidget {"
        "   background-color: rgba(255, 255, 255, 0.08);"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "   color: white;"
        "   font-size: 16px;"
        "}"
        "QListWidget::item {"
        "   padding: 12px;"
        "   border-radius: 8px;"
        "   margin: 5px 10px;" /* Adjust margin to align in two columns */
        "   background-color: rgba(0, 0, 0, 0.3);"
        "   text-align: center;"
        "   min-width: 500px;" /* Ensures equal width */
        "   min-height: 50px;" /* Ensures equal height */
        "   display: inline-block;" /* Helps in column arrangement */
        "}"
        "QListWidget::item:hover {"
        "   background-color: rgba(0, 150, 255, 0.3);"
        "}"
        "QListWidget::item:selected {"
        "   background-color: rgba(0, 150, 255, 0.5);"
        "   border: none;"
        "}"
        );


    courseListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    // courseListWidget->setFixedWidth(850);
    // courseListWidget->setFixedHeight(300);

    courseListWidget->setMaximumSize(850,300);
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);

    scrollArea->setWidget(courseListWidget);
    scrollArea->setStyleSheet("border: none;");

    QLabel *studentLabel = new QLabel("Select Student:");
    studentLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #ffffff;");
    QLabel *courseLabel = new QLabel("Select Courses:");
    courseLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #ffffff;");

    formLayout->addRow(studentLabel, studentDropdown);
    formLayout->addRow(departmentLabel, departmentDisplay); // Add department row
    formLayout->addRow(courseLabel, scrollArea);
    formLayout->setAlignment(scrollArea, Qt::AlignTop);
    QWidget *formContainer = new QWidget;
    QVBoxLayout *formContainerLayout = new QVBoxLayout(formContainer);
    formContainerLayout->addLayout(formLayout);
    formContainerLayout->setContentsMargins(0, 0, 0, 0);
    formLayout->setSpacing(10);
    // formContainerLayout->setContentsMargins(20, 0, 20, 0); // Add padding only here


    //Enroll Button
    enrollButton = new QPushButton("Enroll");
    enrollButton->setStyleSheet(R"(
    QPushButton {
        background-color: #1E90FF;
        color: white;
        font-size: 16px;
        padding: 12px 24px;
        border-radius: 8px;
        border:2px solid #1E90FF;
        font-weight: bold;
    }
    QPushButton:hover {

        background: transparent;

    }
    QPushButton:pressed { background-color: #1B263B; }
)");
    QPushButton *resetButton = new QPushButton("Reset");
    resetButton->setStyleSheet(R"(
    QPushButton {
        background-color: #FF5C5C;
        color: white;
        font-size: 16px;
        padding: 12px 24px;
        border:2px solid #FF5C5C;
        border-radius: 8px;
        font-weight: bold;
    }
    QPushButton:hover {
        background: transparent;
    }
    QPushButton:pressed { background-color: #1B263B; }
)");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(enrollButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch(); // Push buttons to the center
    // **Set Spacing & Alignment**
    buttonLayout->setSpacing(15);  // Space between buttons
    buttonLayout->setContentsMargins(0, 0, 0, 0);  // Add margin above buttons
    buttonLayout->setAlignment(Qt::AlignCenter);
     QWidget *buttonContainer = new QWidget;
    buttonContainer->setLayout(buttonLayout);
    QWidget *headerContainer = new QWidget();
    QVBoxLayout *headerLayout = new QVBoxLayout(headerContainer);
    headerLayout->setSpacing(0); // Minimal spacing between back button and heading
    headerLayout->setContentsMargins(10, 0, 10, 0);
    headerLayout->addWidget(backButton, 0, Qt::AlignLeft);
    headerLayout->addWidget(headingLabel);
    // Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(topBarWidget, 0, Qt::AlignTop);
    mainLayout->addWidget(headerContainer);
    mainLayout->addWidget(formContainer,0,Qt::AlignCenter);
    mainLayout->addWidget(buttonContainer,0,Qt::AlignCenter);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(mainLayout);
}
void EnrollStudent::enrollStudent() {
    QString studentId = studentDropdown->currentData().toString();
    QString studentName = studentDropdown->currentText();  // Get student name
    QStringList selectedCourses;

    // Step 1: Get selected courses from the list
    for (int i = 0; i < courseListWidget->count(); ++i) {
        QListWidgetItem *item = courseListWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            selectedCourses.append(item->data(Qt::UserRole).toString());

        }
    }

    // Step 2: Check if the student is selected
    if (studentId.isEmpty()) {
        MessageBoxUtil::showCustomMessage(this, "Please select a student.", "Enrollment Failed", "OK");
        return;
    }

    // Step 3: Check if at least one course is selected
    if (selectedCourses.isEmpty()) {
        MessageBoxUtil::showCustomMessage(this, "Please select at least one course.", "Enrollment Failed", "OK");
        return;
    }

    // Step 4: Insert selected courses into the database
    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        MessageBoxUtil::showCustomMessage(this, "Database connection is not open!", "Database Error", "OK");
        return;
    }

    // Begin transaction to ensure data consistency
    QSqlQuery query;
    db.transaction();

    bool enrollmentSuccess = true;
    for (const QString &course : selectedCourses) {
        // Prepare query to insert enrollment record for the student and course
        query.prepare("INSERT INTO vls_schema.enrollments (student_id,course_code)"
                      "SELECT :studentId,:courseName");
        query.bindValue(":studentId", studentId);
        query.bindValue(":courseName", course);

        // Execute the query for each course
        if (!query.exec()) {
            qDebug() << "Error enrolling student in course: " << query.lastError().text();
            enrollmentSuccess = false;
            break;
        }
    }

    // Commit the transaction if all queries are successful
    if (enrollmentSuccess) {
        db.commit();
        // Display success message
        MessageBoxUtil::showCustomMessage(this, "Student " + studentName + " has been enrolled in selected courses.", "Enrollment Successful", "OK");
    } else {
        db.rollback();  // Rollback if there was any error
        MessageBoxUtil::showCustomMessage(this, "An error occurred while enrolling the student.", "Enrollment Failed", "OK");
    }

    // Clear selections after enrollment
    for (int i = 0; i < courseListWidget->count(); ++i) {
        courseListWidget->item(i)->setCheckState(Qt::Unchecked);
    }
}


void EnrollStudent::loadStudents() {
    studentDropdown->clear(); // Clear old items
    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        MessageBoxUtil::showCustomMessage(this, "Database connection is not open!", "Database Error", "OK");
        return;
    }
    QSqlQuery query;
    if (query.exec("SELECT unique_id FROM vls_schema.users WHERE role='student'")) {
        QStringList ids;
        while (query.next()) {
            QString id = query.value(0).toString();
            studentDropdown->addItem(id, id);  // Show ID and store ID as data
            ids << id;
        }

        // Update completer
        QCompleter *completer = studentDropdown->completer();
        completer->setModel(new QStringListModel(ids, completer));
    } else {
        qDebug() << "Error loading students:" << query.lastError().text();
        MessageBoxUtil::showCustomMessage(this, "Error loading students: " + query.lastError().text(), "Database Error", "OK");
    }
}

void EnrollStudent::loadCoursesForStudent(const QString &studentId) {
    courseListWidget->clear();  // Clear previous courses

    // Step 1: Fetch the department of the student
    QSqlQuery query;
    query.prepare("SELECT department FROM vls_schema.users WHERE unique_id = :studentId");
    query.bindValue(":studentId", studentId);

    QString studentDepartment;
    if (query.exec() && query.next()) {
        studentDepartment = query.value(0).toString();
    } else {
        qDebug() << "Error fetching student department:" << query.lastError().text();
        MessageBoxUtil::showCustomMessage(this, "Error fetching student department: " + query.lastError().text(), "Database Error", "OK");
        return;
    }

    // Step 2: Fetch courses from the database where the department matches
    query.prepare("SELECT course_name, course_code FROM vls_schema.courses WHERE department = :department");
    query.bindValue(":department", studentDepartment);

    if (query.exec()) {
        while (query.next()) {
            QString courseName = query.value(0).toString();
            QString courseCode = query.value(1).toString();

            QListWidgetItem *item = new QListWidgetItem(courseName);  // Show name
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
            item->setData(Qt::UserRole, courseCode);  // Store course code for DB use

            courseListWidget->addItem(item);  // ✅ Only add once
        }
    } else {
        qDebug() << "Error fetching courses for department:" << query.lastError().text();
        MessageBoxUtil::showCustomMessage(this, "Error fetching courses for department: " + query.lastError().text(), "Database Error", "OK");
        return;
    }
}


// When a student is selected, update the department label and load courses
void EnrollStudent::onStudentSelected(int index) {
    if (index < 0) {
        departmentDisplay->setText("N/A");
        courseListWidget->clear();
        return;
    }

    // Get the selected student ID from the dropdown
    QString studentId = studentDropdown->itemData(index).toString();
    qDebug() << "Selected Student ID: " << studentId;

    // Step 1: Fetch department of the selected student from the database
    QSqlQuery query;
    query.prepare("SELECT department FROM vls_schema.users WHERE unique_id = :studentId");
    query.bindValue(":studentId", studentId);

    QString department;
    if (query.exec() && query.next()) {
        department = query.value(0).toString();
    } else {
        qDebug() << "Error fetching student department:" << query.lastError().text();
        departmentDisplay->setText("N/A");
        courseListWidget->clear();
        MessageBoxUtil::showCustomMessage(this, "Error fetching student department: " + query.lastError().text(), "Database Error", "OK");
        return;
    }

    // Update the department label in the UI
    departmentDisplay->setText(department);

    // Step 2: Load the courses for this student based on the department
    loadCoursesForStudent(studentId);  // This will load courses based on the department
}


