#include "ProfilePage.h"
#include "StudentHomePage.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QFrame>
#include <QFileDialog>
#include "TopBar.h"
#include <QString>
#include "DatabaseManager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QDebug>

ProfilePage::ProfilePage(const QString &userEmail, QWidget *parent, QWidget *topBar) : QWidget(parent), email(userEmail) {
    setWindowTitle("Profile Page");
    setMinimumSize(800, 500);
    setStyleSheet("background-color: #0d1b2a;"); // Dark background
    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection is not open!");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT unique_id, first_name, last_name, gender, phone, role, department, email, fee_status FROM vls_schema.users WHERE email = :email");
    query.bindValue(":email", email);
    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to retrieve users: " + query.lastError().text());
        return;
    }
    while (query.next()) {
        uniqueId = query.value("unique_id").toString();
        firstName = query.value("first_name").toString();
        lastName = query.value("last_name").toString();
        role = query.value("role").toString();
        fullName = firstName + " " + lastName;
        email = query.value("email").toString();
        gender = query.value("gender").toString();
        department = query.value("department").toString();
        phone = query.value("phone").toString();
        feeStatus = query.value("fee_status").toString();
    }

    // Fetch batch for students
    if (role == "student") {
        QSqlQuery Batchquery;
        Batchquery.prepare("SELECT EXTRACT(YEAR FROM created_at) FROM vls_schema.users WHERE email = :email");
        Batchquery.bindValue(":email", email);
        if (Batchquery.exec() && Batchquery.next()) {
            batch = Batchquery.value(0).toString();
            qDebug() << "Registration Year:" << batch;
        }
    }

    // Fetch courses for teachers
    if (role == "teacher") {
        QSqlQuery courseQuery;
        courseQuery.prepare("SELECT course_name FROM vls_schema.courses WHERE teacher_id = :teacher_id");
        courseQuery.bindValue(":teacher_id", uniqueId);
        if (!courseQuery.exec()) {
            QMessageBox::critical(this, "Database Error", "Failed to retrieve courses: " + courseQuery.lastError().text());
            return;
        }
        while (courseQuery.next()) {
            courses.append(courseQuery.value("course_name").toString());
        }
        qDebug() << "Courses taught:" << courses;
    }

    // Create the main content
    createMainContent();
    QWidget *topBarPlaceholder = new QWidget();
    topBarPlaceholder->setFixedHeight(topBar->height());

    // Main Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0); // Remove margins
    layout->setSpacing(0); // Remove extra spacing
    layout->addWidget(topBarPlaceholder, 0, Qt::AlignTop);
    // layout->addWidget(topBar);
    layout->addWidget(mainContent);
    setLayout(layout);
}

void ProfilePage::createMainContent() {
    mainContent = new QWidget(this);
    mainContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout = new QVBoxLayout(mainContent);
    mainLayout->setContentsMargins(40, 30, 40, 30);
    mainLayout->setSpacing(15);
    mainLayout->setAlignment(Qt::AlignTop);

    // Profile Picture and Name Layout
    QHBoxLayout *profileTopLayout = new QHBoxLayout();
    profileTopLayout->setAlignment(Qt::AlignLeft);

    // Profile Picture Layout
    QVBoxLayout *profilePictureLayout = new QVBoxLayout();
    profilePictureLayout->setAlignment(Qt::AlignCenter); // Center the PFP

    // Profile Picture
    profilePictureLabel = new QLabel(mainContent);
    QPixmap profilePicture("images/ProfileIcon.png");
    profilePicture = profilePicture.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    profilePictureLabel->setPixmap(profilePicture);
    profilePictureLabel->setStyleSheet("border: none;");

    // Add Profile Picture to its layout
    profilePictureLayout->addWidget(profilePictureLabel);

    // Name Layout
    QVBoxLayout *nameLayout = new QVBoxLayout();
    nameLayout->setAlignment(Qt::AlignCenter); // Center the name

    // Name Label
    nameLabel = new QLabel(fullName, mainContent);
    nameLabel->setStyleSheet("color: white; font-size: 24px; font-weight: bold;");

    // Add Name to its layout
    nameLayout->addWidget(nameLabel);

    // Buttons Layout (Upload Picture and Change Password)
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setAlignment(Qt::AlignCenter);  // Center the buttons

    // Upload Picture Button
    QPushButton *uploadPictureButton = new QPushButton("Upload Picture", mainContent);
    uploadPictureButton->setMinimumSize(150, 40);
    uploadPictureButton->setCursor(Qt::PointingHandCursor);
    uploadPictureButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #1E90FF; color: white; font-size: 16px; font-weight: bold;"
        "   border-radius: 5px; padding: 5px 10px; border: 2px solid #1E90FF;"
        "} "
        "QPushButton:hover {"
        "   background: transparent; "
        "} "
        "QPushButton:pressed { background-color: #1B263B; }"
        );

    // Connect Upload Picture Button to a slot
    connect(uploadPictureButton, &QPushButton::clicked, this, &ProfilePage::uploadPicture);

    // Add buttons to the buttons layout
    buttonsLayout->addWidget(uploadPictureButton);

    // Add buttons layout to the name layout
    nameLayout->addLayout(buttonsLayout);

    // Add Profile Picture and Name Layouts to the Top Layout
    profileTopLayout->addLayout(profilePictureLayout);
    profileTopLayout->addSpacing(20); // Add spacing between PFP and name
    profileTopLayout->addLayout(nameLayout);

    // Add Profile Top Layout to Main Layout
    mainLayout->addLayout(profileTopLayout);
    mainLayout->addSpacing(20);

    // Create a frame for Profile Information
    QFrame *profileInfoFrame = new QFrame(mainContent);
    profileInfoFrame->setStyleSheet("border: 2px solid #415a77; border-radius: 5px; background-color: #1B263B;");
    QHBoxLayout *profileInfoLayout = new QHBoxLayout(profileInfoFrame);
    profileInfoLayout->setContentsMargins(10, 10, 10, 10);
    profileInfoLayout->setSpacing(20);

    // Personal Details Section
    QVBoxLayout *personalDetailsLayout = new QVBoxLayout();
    QLabel *personalDetailsLabel = new QLabel("PERSONAL DETAILS", profileInfoFrame);
    personalDetailsLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold; background: transparent; border: none;");

    // Add bold labels for Personal Details
    QLabel *firstNameLabel = new QLabel("<b>First Name: </b>"+firstName, profileInfoFrame);
    firstNameLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");
    QLabel *lastNameLabel = new QLabel("<b>Last Name: </b>"+lastName, profileInfoFrame);
    lastNameLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");
    QLabel *emailLabel = new QLabel("<b>Email: </b>"+email, profileInfoFrame);
    emailLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");
    QLabel *genderLabel = new QLabel("<b>Gender: </b>"+gender, profileInfoFrame);
    genderLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");
    QLabel *phoneLabel = new QLabel("<b>Phone Number: </b>"+phone, profileInfoFrame);
    phoneLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");
    phoneLabel->setWordWrap(true);

    // Add widgets to the personal details layout
    personalDetailsLayout->addWidget(personalDetailsLabel);
    personalDetailsLayout->addWidget(firstNameLabel);
    personalDetailsLayout->addWidget(lastNameLabel);
    personalDetailsLayout->addWidget(emailLabel);
    personalDetailsLayout->addWidget(genderLabel);
    personalDetailsLayout->addWidget(phoneLabel);

    // Enrollment Details Section
    QVBoxLayout *enrollmentDetailsLayout = new QVBoxLayout();
    QLabel *classDetailsLabel = new QLabel(role.toUpper()+" DETAILS", profileInfoFrame);
    classDetailsLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold; background: transparent; border: none;");

    // Add bold labels for Enrollment Details
    QLabel *IDLabel = new QLabel("<b>ID: </b>"+uniqueId, profileInfoFrame);
    IDLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");

    QLabel *roleLabel = new QLabel("<b>Role: </b>"+role, profileInfoFrame);
    roleLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");
    enrollmentDetailsLayout->addWidget(classDetailsLabel);
    enrollmentDetailsLayout->addWidget(IDLabel);
    enrollmentDetailsLayout->addWidget(roleLabel);

    if (role == "student") {
        QLabel *departmentLabel = new QLabel("<b>Department: </b>"+department, profileInfoFrame);
        departmentLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");

        QLabel *batchLabel = new QLabel("<b>Batch: </b>"+batch, profileInfoFrame);
        batchLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");

        QLabel *feeStatusLabel = new QLabel("<b>Fee Status: </b>"+feeStatus, profileInfoFrame);
        feeStatusLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");
        enrollmentDetailsLayout->addWidget(departmentLabel);
        enrollmentDetailsLayout->addWidget(batchLabel);
        enrollmentDetailsLayout->addWidget(feeStatusLabel);
    } else if (role == "teacher") {
        // QLabel *departmentLabel = new QLabel("<b>Department: </b>"+department, profileInfoFrame);
        // departmentLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");

        QString coursesText = courses.isEmpty() ? "None" : courses.join(", ");
        QLabel *coursesLabel = new QLabel("<b>Courses: </b>"+coursesText, profileInfoFrame);
        coursesLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");
        coursesLabel->setWordWrap(true);

        // enrollmentDetailsLayout->addWidget(departmentLabel);
        enrollmentDetailsLayout->addWidget(coursesLabel);
    }

    // Add both layouts to the profile info layout
    profileInfoLayout->addLayout(personalDetailsLayout);
    profileInfoLayout->addLayout(enrollmentDetailsLayout);

    // Add Profile Info Frame to Main Layout
    mainLayout->addWidget(profileInfoFrame);
    mainLayout->addSpacing(20);
}

void ProfilePage::uploadPicture() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select Profile Picture", "", "Images (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        QPixmap newProfilePicture(filePath);
        newProfilePicture = newProfilePicture.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        profilePictureLabel->setPixmap(newProfilePicture);
    }
}
