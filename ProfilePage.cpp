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

ProfilePage::ProfilePage(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Profile Page");
    setMinimumSize(800, 500);
    topBar = new TopBar(this);
    setStyleSheet("background-color: #0d1b2a;"); // Dark background

    // Create the top bar


    // Create the main content
    createMainContent();

    // Main Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0); // Remove margins
    layout->setSpacing(0); // Remove extra spacing
    layout->addWidget(topBar);
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
    QPixmap profilePicture("C:/Users/Lenovo/Downloads/VLS/Profile_Icon.png");
    profilePicture = profilePicture.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    profilePictureLabel->setPixmap(profilePicture);
    profilePictureLabel->setStyleSheet("border: none;");

    // Add Profile Picture to its layout
    profilePictureLayout->addWidget(profilePictureLabel);

    // Name Layout
    QVBoxLayout *nameLayout = new QVBoxLayout();
    nameLayout->setAlignment(Qt::AlignCenter); // Center the name

    // Name Label
    nameLabel = new QLabel("Muhammad Taha Farrukh", mainContent);
    nameLabel->setStyleSheet("color: white; font-size: 24px; font-weight: bold;");

    // Add Name to its layout
    nameLayout->addWidget(nameLabel);

    // Buttons Layout (Upload Picture and Change Password)
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setAlignment(Qt::AlignCenter);  // Center the buttons

    // Upload Picture Button
    QPushButton *uploadPictureButton = new QPushButton("Upload Picture", mainContent);
    uploadPictureButton->setFixedSize(150, 40);
    uploadPictureButton->setCursor(Qt::PointingHandCursor);
    uploadPictureButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4169E1; color: white; font-size: 16px; font-weight: bold;"
        "   border-radius: 5px; padding: 5px 10px; border: none;"
        "} "
        "QPushButton:hover {"
        "   background-color: #2d3e50; "
        "} "
        );

    // Connect Upload Picture Button to a slot
    connect(uploadPictureButton, &QPushButton::clicked, this, &ProfilePage::uploadPicture);

    // Change Password Button
    QPushButton *changePasswordButton = new QPushButton("Change Password", mainContent);
    changePasswordButton->setFixedSize(150, 40);
    changePasswordButton->setCursor(Qt::PointingHandCursor);
    changePasswordButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4169E1; color: white; font-size: 16px; font-weight: bold;"
        "   border-radius: 5px; padding: 5px 10px; border: none;"
        "} "
        "QPushButton:hover {"
        "   background-color: #2d3e50; "
        "} "
        );

    // Add buttons to the buttons layout
    buttonsLayout->addWidget(uploadPictureButton);
    buttonsLayout->addWidget(changePasswordButton);

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
    QLabel *genderLabel = new QLabel("<b>Gender:</b> Male", profileInfoFrame);
    genderLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");

    QLabel *birthDateLabel = new QLabel("<b>Date Of Birth:</b> 03/03/2005", profileInfoFrame);
    birthDateLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");

    QLabel *addressLabel = new QLabel("<b>Address:</b> DHA Phase 7, Karachi, Pakistan", profileInfoFrame);
    addressLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");
    addressLabel->setWordWrap(true);

    QLabel *phoneLabel = new QLabel("<b>Phone Number:</b> 0326881239", profileInfoFrame);
    phoneLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");
    phoneLabel->setWordWrap(true);

    // Add widgets to the personal details layout
    personalDetailsLayout->addWidget(personalDetailsLabel);
    personalDetailsLayout->addWidget(genderLabel);
    personalDetailsLayout->addWidget(birthDateLabel);
    personalDetailsLayout->addWidget(addressLabel);
    personalDetailsLayout->addWidget(phoneLabel);

    // Enrollment Details Section
    QVBoxLayout *enrollmentDetailsLayout = new QVBoxLayout();
    QLabel *classDetailsLabel = new QLabel("ENROLLMENT DETAILS", profileInfoFrame);
    classDetailsLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold; background: transparent; border: none;");

    // Add bold labels for Enrollment Details
    QLabel *IDLabel = new QLabel("<b>Student ID:</b> CT-201", profileInfoFrame);
    IDLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");

    QLabel *roleLabel = new QLabel("<b>Role:</b> Student", profileInfoFrame);
    roleLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");

    QLabel *departmentLabel = new QLabel("<b>Department:</b> Computer Science", profileInfoFrame);
    departmentLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");

    QLabel *feeStatusLabel = new QLabel("<b>Fee Status:</b> Paid", profileInfoFrame);
    feeStatusLabel->setStyleSheet("color: white; font-size: 16px; background: transparent; border: none;");

    // Add widgets to the enrollment details layout
    enrollmentDetailsLayout->addWidget(classDetailsLabel);
    enrollmentDetailsLayout->addWidget(IDLabel);
    enrollmentDetailsLayout->addWidget(roleLabel);
    enrollmentDetailsLayout->addWidget(departmentLabel);
    enrollmentDetailsLayout->addWidget(feeStatusLabel);

    // Add both layouts to the profile info layout
    profileInfoLayout->addLayout(personalDetailsLayout);
    profileInfoLayout->addLayout(enrollmentDetailsLayout);

    // Add Profile Info Frame to Main Layout
    mainLayout->addWidget(profileInfoFrame);
    mainLayout->addSpacing(20);
}

// Slot to handle Upload Picture button click
void ProfilePage::uploadPicture() {
    // Open a file dialog to select an image
    QString filePath = QFileDialog::getOpenFileName(this, "Select Profile Picture", "", "Images (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        // Load the selected image and scale it to fit the label
        QPixmap newProfilePicture(filePath);
        newProfilePicture = newProfilePicture.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        profilePictureLabel->setPixmap(newProfilePicture);  // Set the new image to the label
    }
}
