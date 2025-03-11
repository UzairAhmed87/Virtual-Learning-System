#include "registerUser.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include "TopBar.h"


RegisterUserForm::RegisterUserForm(QWidget *parent) : QWidget(parent) {
    this->setMinimumSize(800, 500); // Forces window to be 800x500

   topBar = new TopBar(this);

    setStyleSheet("background-color: #0D1B2A; color: white; font-size: 16px;");
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



    // Register User Heading
    QLabel *headingLabel = new QLabel("Register User");
    headingLabel->setAlignment(Qt::AlignCenter);
    headingLabel->setStyleSheet("font-size: 30px; font-weight: bold; color: white; margin-top: 0px;");



    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);

    // **Form Fields**
    QFormLayout *formLayout = new QFormLayout;
    formLayout->setSpacing(15);

    QLabel *firstNameLabel = new QLabel("First Name:");
    firstNameLabel->setStyleSheet("font-weight: bold; color: white;");
    QLineEdit *firstNameField = new QLineEdit;
    firstNameField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(firstNameLabel, firstNameField);

    QLabel *lastNameLabel = new QLabel("Last Name:");
    lastNameLabel->setStyleSheet("font-weight: bold; color: white;");
    QLineEdit *lastNameField = new QLineEdit;
    lastNameField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(lastNameLabel, lastNameField);


    QLabel *emailLabel = new QLabel("Email:");
    emailLabel->setStyleSheet("font-weight: bold; color: white;");
    QLineEdit *emailField = new QLineEdit;
    emailField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(emailLabel, emailField);


    QLabel *passwordLabel = new QLabel("Password:");
    passwordLabel->setStyleSheet("font-weight: bold; color: white;");
    QLineEdit *passwordField = new QLineEdit;
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(passwordLabel, passwordField);

    QLabel *phoneLabel = new QLabel("Phone Number:");
    phoneLabel->setStyleSheet("font-weight: bold; color: white;");
    QLineEdit *phoneField = new QLineEdit;
    phoneField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(phoneLabel, phoneField);

    QLabel *genderLabel = new QLabel("Gender:");
    genderLabel->setStyleSheet("font-weight: bold; color: white;");
    QComboBox *genderDropdown = new QComboBox;
    genderDropdown->addItems({"Male", "Female", "Prefer Not To Say"});
    genderDropdown->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(genderLabel, genderDropdown);

    QLabel *userTypeLabel = new QLabel("User Type:");
    userTypeLabel->setStyleSheet("font-weight: bold; color: white;");
    QComboBox *userTypeDropdown = new QComboBox;
    userTypeDropdown->addItems({"Student", "Teacher", "Admin"});
    userTypeDropdown->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(userTypeLabel, userTypeDropdown);

    // Create the dropdowns but hide them initially
    QLabel *departmentLabel = new QLabel("Department:");
    departmentLabel->setStyleSheet("font-weight: bold; color: white;");
    QComboBox *departmentDropdown = new QComboBox;
    departmentDropdown->addItems({"CS", "IT", "SE", "EE"});
    departmentDropdown->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    departmentLabel->hide();
    departmentDropdown->hide();

    QLabel *courseLabel = new QLabel("Course:");
    courseLabel->setStyleSheet("font-weight: bold; color: white;");
    QComboBox *courseDropdown = new QComboBox;
    courseDropdown->addItems({"OOP", "DSA", "AI", "DBMS"});
    courseDropdown->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    courseLabel->hide();
    courseDropdown->hide();
    connect(userTypeDropdown, &QComboBox::currentTextChanged, [=](const QString &selectedType) {
        if (selectedType == "Student") {
            departmentLabel->show();
            departmentDropdown->show();
            courseLabel->hide();
            courseDropdown->hide();
        } else if (selectedType == "Teacher") {
            courseLabel->show();
            courseDropdown->show();
            departmentLabel->hide();
            departmentDropdown->hide();
        } else {
            departmentLabel->hide();
            departmentDropdown->hide();
            courseLabel->hide();
            courseDropdown->hide();
        }
    });

    // **Buttons**
    QPushButton *registerButton = new QPushButton("Register");
    registerButton->setStyleSheet(R"(
    QPushButton {
        background-color: #778da9;
        color: white;
        font-size: 16px;
        padding: 12px 24px;
        border-radius: 8px;
        font-weight: bold;
    }
    QPushButton:hover {

        background-color: #1B263B;

    }
)");

    QPushButton *resetButton = new QPushButton("Reset");
    resetButton->setStyleSheet(R"(
    QPushButton {
        background-color: #FF5C5C;
        color: white;
        font-size: 16px;
        padding: 12px 24px;
        border-radius: 8px;
        border: none;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: #E04A4A;
    }
)");



    QWidget *buttonContainer = new QWidget;
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(); // Push buttons to the center
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch(); // Push buttons to the center
    // **Set Spacing & Alignment**
    buttonLayout->setSpacing(20);  // Space between buttons
    buttonLayout->setContentsMargins(0, 0, 0, 0);  // Add margin above buttons
    buttonLayout->setAlignment(Qt::AlignCenter); // Center the buttons


    // **Form Section with Padding**
    QWidget *formContainer = new QWidget;
    QVBoxLayout *formContainerLayout = new QVBoxLayout(formContainer);
    formContainerLayout->addLayout(formLayout);
    formContainerLayout->setContentsMargins(20, 0, 20, 0); // Add padding only here
    formLayout->setSpacing(10);
    buttonLayout->setSpacing(15);

    formLayout->addRow(departmentLabel, departmentDropdown);
    formLayout->addRow(courseLabel, courseDropdown);
    mainLayout->addWidget(topBar,0, Qt::AlignTop);
    mainLayout->addWidget(backButton, 0, Qt::AlignLeft);
    mainLayout->addWidget(headingLabel, 0, Qt::AlignTop);
    mainLayout->addWidget(formContainer);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(buttonContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0); // **Added Padding**
    mainLayout->setSpacing(15); // **Balanced Spacing**


    setLayout(mainLayout);
    connect(resetButton, &QPushButton::clicked, this, [=]() {
        firstNameField->clear();
        lastNameField->clear();
        emailField->clear();
        passwordField->clear();
        phoneField->clear();
        userTypeDropdown->setCurrentIndex(0); // Reset dropdown to first item
        genderDropdown->setCurrentIndex(0);
    });



    // Name Validation (Only Alphabets and Spaces)
    QRegularExpression firstNameRegex("^[A-Za-z ]+$");
    QRegularExpressionValidator *firstNameValidator = new QRegularExpressionValidator(firstNameRegex, this);
    firstNameField->setValidator(firstNameValidator);

    QRegularExpression lastNameRegex("^[A-Za-z ]+$");
    QRegularExpressionValidator *lastNameValidator = new QRegularExpressionValidator(lastNameRegex, this);
    lastNameField->setValidator(lastNameValidator);


    // Email Validation
    QRegularExpression emailRegex(R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");
    QRegularExpressionValidator *emailValidator = new QRegularExpressionValidator(emailRegex, this);
    emailField->setValidator(emailValidator);

    // Phone Number Validation (Only Digits, Length 10-15)
    QRegularExpression phoneRegex("^[0-9]{10,15}$");
    QRegularExpressionValidator *phoneValidator = new QRegularExpressionValidator(phoneRegex, this);
    phoneField->setValidator(phoneValidator);

    // Password Validation (Manually Checked in Register Button Click)
    connect(registerButton, &QPushButton::clicked, this, [=]() {
        QString firstName = firstNameField->text().trimmed();
        QString lastName = lastNameField->text().trimmed();
        QString email = emailField->text().trimmed();
        QString password = passwordField->text();
        QString phone = phoneField->text().trimmed();
        QString userType = userTypeDropdown->currentText();

        // Checking Required Fields
        if (firstName.isEmpty() ||lastName.isEmpty() || email.isEmpty() || password.isEmpty() || phone.isEmpty()) {
            QMessageBox::warning(this, "Validation Error", "All fields are required!");
            return;
        }

        // Password Length Check
        if (password.length() < 6) {
            QMessageBox::warning(this, "Validation Error", "Password must be at least 6 characters long!");
            return;
        }

        QMessageBox::information(this, "Success", "User Registered Successfully!");
    });


}
