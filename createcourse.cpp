#include "createcourse.h"
#include "BackButton.h"
#include "TopBar.h"
#include "DatabaseManager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

createcourse::createcourse(QWidget *parent) : QWidget(parent) {
    this->setMinimumSize(800, 500); // Forces window to be 800x500

    setStyleSheet("background-color: #0D1B2A; color: white; font-size: 16px;");
    topbar = new TopBar(this);
    QPushButton *backButton = new QPushButton;
    backButton->setIcon(QIcon("images/back_arrow.png")); // Use an appropriate left arrow icon
    backButton->setIconSize(QSize(30, 30));
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton { background: none; border: none; padding: 5px; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        );

    // **Connect Back Button to Close Form**
    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit backButtonClicked();
    });



    QLabel *headingLabel = new QLabel("Create Course");
    headingLabel->setAlignment(Qt::AlignCenter);
    headingLabel->setStyleSheet("font-size: 30px; font-weight: bold; color: white; margin-top: 0px;");


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);

    // **Form Fields**
    QFormLayout *formLayout = new QFormLayout;
    formLayout->setSpacing(15);

    QLabel *courseNameLabel = new QLabel("Course Name:");
    courseNameLabel->setStyleSheet("font-weight: bold; color: white;");
    courseNameField = new QLineEdit;
    courseNameField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(courseNameLabel, courseNameField);

    QLabel *courseCodeLabel = new QLabel("Course Code:");
    courseCodeLabel->setStyleSheet("font-weight: bold; color: white;");
    courseCodeField = new QLineEdit;
    courseCodeField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(courseCodeLabel, courseCodeField);


    QLabel *descriptionLabel = new QLabel("Description:");
    descriptionLabel->setStyleSheet("font-weight: bold; color: white;");
    descriptionField = new QLineEdit;
    descriptionField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(descriptionLabel, descriptionField);

    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection is not open!");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT unique_id, first_name, last_name FROM vls_schema.users WHERE role = 'teacher'");

    QLabel *instructorLabel = new QLabel("Instructor:");
    instructorLabel->setStyleSheet("font-weight: bold; color: white;");
    instructorDropdown = new QComboBox;
    instructorDropdown->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    if (query.exec()) {
        instructorDropdown->clear(); // Clear existing items
        instructorDropdown->addItem("Select Instructor", ""); // Default option (empty value)

        while (query.next()) {
            QString teacherId = query.value(0).toString(); // Get unique_id (e.g., "TCH-001")
            QString teacherName = query.value(1).toString() + " " + query.value(2).toString(); // Full name

            instructorDropdown->addItem(teacherName,teacherId); // Store unique_id as item data
        }
    } else {
        qDebug() << "Error fetching instructors:" << query.lastError().text();
    }
    formLayout->addRow(instructorLabel,instructorDropdown);
    QLabel *departmentLabel = new QLabel("Department:");
    departmentLabel->setStyleSheet("font-weight: bold; color: white;");
    departmentDropdown = new QComboBox;
    departmentDropdown->addItems({"Select Department", "CS", "IT", "SE", "EE"});
    departmentDropdown->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(departmentLabel, departmentDropdown);


    // **Buttons**
    QPushButton *registerButton = new QPushButton("Create");
    registerButton->setStyleSheet(R"(
    QPushButton {
        background-color: #4169E1;
        font-size: 16px;
        padding: 12px 24px;
        border-radius: 8px;
        border:2px solid #4169E1;
        font-weight: bold;
    }
    QPushButton:hover {

        background-color: transparent;

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




    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(); // Push buttons to the center
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch(); // Push buttons to the center
    // **Set Spacing & Alignment**
    buttonLayout->setSpacing(15);  // Space between buttons
    buttonLayout->setContentsMargins(0, 0, 0, 0);  // Add margin above buttons
    buttonLayout->setAlignment(Qt::AlignCenter); // Center the buttons


    QWidget *buttonContainer = new QWidget;
    // **Form Section with Padding**
    QWidget *formContainer = new QWidget;
    QVBoxLayout *formContainerLayout = new QVBoxLayout(formContainer);
    formContainerLayout->addLayout(formLayout);
    formContainerLayout->setContentsMargins(20, 0, 20, 0); // Add padding only here

    formLayout->setSpacing(10);



    mainLayout->addWidget(topbar,0, Qt::AlignTop);
    mainLayout->addWidget(backButton, 0, Qt::AlignLeft);
    mainLayout->addWidget(headingLabel, 0, Qt::AlignTop);
    mainLayout->addWidget(formContainer);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(buttonContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0); // **Added Padding**
    mainLayout->setSpacing(15); // **Balanced Spacing**1


    setLayout(mainLayout);

    connect(resetButton, &QPushButton::clicked, this,&createcourse::reset);


    // Name Validation (Only Alphabets and Spaces)
    QRegularExpression courseNameRegex("^[A-Za-z ]+$");
    QRegularExpressionValidator *courseNameValidator = new QRegularExpressionValidator(courseNameRegex, this);
    courseNameField->setValidator(courseNameValidator);

    QRegularExpression courseCodeRegex("^[A-Za-z0-9_-]+$");
    QRegularExpressionValidator *courseCodeValidator = new QRegularExpressionValidator(courseCodeRegex, this);
    courseCodeField->setValidator(courseCodeValidator);


    QRegularExpression descriptionRegex("^[A-Za-z0-9.,!?()'\"\\-\\s]+$");
    QRegularExpressionValidator *descriptionValidator = new QRegularExpressionValidator(descriptionRegex, this);
    descriptionField->setValidator(descriptionValidator);


    connect(registerButton, &QPushButton::clicked, this, [=]() {
        QString courseName = courseNameField->text().trimmed();
        QString courseCode = courseCodeField->text().trimmed();
        QString description = descriptionField->text().trimmed();
        QString department =  departmentDropdown->currentText();
        QString teacherId = instructorDropdown->currentData().toString();
        QMessageBox mesgBox;
        mesgBox.setStyleSheet(
            "QMessageBox { background-color: #1b263b; color: white; font-size: 16px; }"
            "QLabel { color: white; }"
            "QPushButton { background-color: #778da9; color: white;border:2px solid #778da9; border-radius: 5px; padding: 8px; }"
            "QPushButton:hover { background-color: transparent; }"
            );

        // Checking Required Fields
        if (courseName.isEmpty() ||courseCode.isEmpty() || description.isEmpty()) {
            QMessageBox::warning(this, "Validation Error", "All fields are required!");
            return;
        }
        if (instructorDropdown->currentIndex() == 0 || departmentDropdown->currentIndex() == 0) {
            QMessageBox::warning(this, "Validation Error", "Please select an instructor and department!");
            return;
        }
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT 1 FROM vls_schema.courses WHERE course_code = :courseCode LIMIT 1");
        checkQuery.addBindValue(courseCode);

        if (!checkQuery.exec()) {
            QMessageBox::critical(this, "Database Error", "Failed to check existing user: " + checkQuery.lastError().text());
            return;
        }

        if (checkQuery.next()) {
            QMessageBox::warning(this, "Registration Error", "Course code already exists!");
            return;
        }
        QSqlQuery registerQuery;
        registerQuery.prepare("INSERT INTO vls_schema.courses (course_name, course_code, description, department, teacher_id) "
                      "SELECT :courseName, :courseCode, :description, :department, :teacherId");

        registerQuery.bindValue(":courseName",courseName);
        registerQuery.bindValue(":courseCode",courseCode);
        registerQuery.bindValue(":description",description);
        registerQuery.bindValue(":department",department);
        registerQuery.bindValue(":teacherId",teacherId);
        if (!registerQuery.exec()) {
            mesgBox.setWindowTitle("Database Error");
            mesgBox.setText("Query execution failed: " + registerQuery.lastError().text());
            mesgBox.setIcon(QMessageBox::Critical);
            mesgBox.exec();
            return;
        }
        QMessageBox::information(this, "Success", "Course Created Successfully!");
        reset();
    });


}
void createcourse::reset(){
    courseNameField->clear();
    courseCodeField->clear();
    descriptionField->clear();
    instructorDropdown->setCurrentIndex(0); // Reset dropdown to first item
    departmentDropdown->setCurrentIndex(0);
}
createcourse::~createcourse() {
    // Destructor implementation
}
