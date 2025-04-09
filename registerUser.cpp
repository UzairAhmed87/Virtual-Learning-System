#include "registerUser.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "TopBar.h"
#include "DatabaseManager.h"
#include <QCryptographicHash>
#include "BackButton.h"
#include <QDebug>
#include "mesgboxutil.h"

RegisterUserForm::RegisterUserForm(QWidget *parent,QWidget *topBar) : QWidget(parent) {
    this->setMinimumSize(800, 500); // Forces window to be 800x500
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    QWidget *topBarPlaceholder = new QWidget();
    topBarPlaceholder->setFixedHeight(topBar->height());
    mainLayout->addWidget(topBarPlaceholder, 0, Qt::AlignTop);
   // topBar = new TopBar(this);

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





    // **Form Fields**
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(15);

    QLabel *firstNameLabel = new QLabel("First Name:");
    firstNameLabel->setStyleSheet("font-weight: bold; color: white;");
    firstNameField = new QLineEdit;
    firstNameField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(firstNameLabel, firstNameField);

    QLabel *lastNameLabel = new QLabel("Last Name:");
    lastNameLabel->setStyleSheet("font-weight: bold; color: white;");
    lastNameField = new QLineEdit;
    lastNameField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(lastNameLabel, lastNameField);


    QLabel *emailLabel = new QLabel("Email:");
    emailLabel->setStyleSheet("font-weight: bold; color: white;");
    emailField = new QLineEdit;
    emailField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(emailLabel, emailField);


    QLabel *passwordLabel = new QLabel("Password:");
    passwordLabel->setStyleSheet("font-weight: bold; color: white;");
    passwordField = new QLineEdit;
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(passwordLabel, passwordField);

    QLabel *phoneLabel = new QLabel("Phone Number:");
    phoneLabel->setStyleSheet("font-weight: bold; color: white;");
    phoneField = new QLineEdit;
    phoneField->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(phoneLabel, phoneField);

    QLabel *genderLabel = new QLabel("Gender:");
    genderLabel->setStyleSheet("font-weight: bold; color: white;");
    genderDropdown = new QComboBox;
    genderDropdown->addItems({"Male", "Female", "Prefer Not To Say"});
    genderDropdown->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(genderLabel, genderDropdown);

    QLabel *userTypeLabel = new QLabel("User Type:");
    userTypeLabel->setStyleSheet("font-weight: bold; color: white;");
    userTypeDropdown = new QComboBox;
    userTypeDropdown->addItems({"Student", "Teacher", "Admin"});
    userTypeDropdown->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    formLayout->addRow(userTypeLabel, userTypeDropdown);

    // Create the dropdowns but hide them initially
    QLabel *departmentLabel = new QLabel("Department:");
    departmentLabel->setStyleSheet("font-weight: bold; color: white;");
    departmentDropdown = new QComboBox;
    departmentDropdown->addItems({"CS", "IT", "SE", "EE"});
    departmentDropdown->setStyleSheet("background-color: #1B263B; color: white; padding: 10px; border-radius: 5px;");
    departmentLabel->show();
    departmentDropdown->show();
    connect(userTypeDropdown, &QComboBox::currentTextChanged, this, [=](const QString &selectedType) {
        if (selectedType == "Student") {
            departmentLabel->show();
            departmentDropdown->show();
        } else {
            departmentLabel->hide();
            departmentDropdown->hide();
        }
    });


    // **Buttons**
    registerButton = new QPushButton("Register");
    registerButton->setStyleSheet(R"(
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

    resetButton = new QPushButton("Reset");
    resetButton->setStyleSheet(R"(
    QPushButton {
        background-color: #FF5C5C;
        color: white;
        font-size: 16px;
        padding: 12px 24px;
        border-radius: 8px;
        border: 2px solid #FF5C5C;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: transparent;
    }
    QPushButton:pressed { background-color: #1B263B; }
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

    mainLayout->addWidget(backButton,0, Qt::AlignLeft);
    mainLayout->addWidget(headingLabel);
    mainLayout->addWidget(formContainer);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(buttonContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0); // **Added Padding**
    mainLayout->setSpacing(15); // **Balanced Spacing**
    setLayout(mainLayout);
    connect(resetButton, &QPushButton::clicked, this,&RegisterUserForm::reset);



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
    connect(registerButton, &QPushButton::clicked, this,[=](){
        QString firstName = firstNameField->text().trimmed();
        QString lastName = lastNameField->text().trimmed();
        QString email = emailField->text().trimmed();
        QString password = passwordField->text().trimmed();
        QString phone = phoneField->text().trimmed();
        QString gender = genderDropdown->currentText();
        QString role = userTypeDropdown->currentText();
        QString department = departmentDropdown->currentText();
        /*QMessageBox mesgBox;
        mesgBox.setStyleSheet(
            "QMessageBox { background-color: #1b263b; color: white; font-size: 16px; }"
            "QLabel { color: white; }"
            "QPushButton { background-color: #778da9; color: white;border:2px solid #778da9; border-radius: 5px; padding: 8px; }"
            "QPushButton:hover { background-color: transparent; }"
            )*/;

        // **Checking Required Fields**
        // Set up stylesheet only once if needed in constructor, not here

        // **Checking Required Fields**
        if (firstName.isEmpty() || lastName.isEmpty() || email.isEmpty() || password.isEmpty() || phone.isEmpty()) {
            MessageBoxUtil::showCustomMessage(
                this,
                "All fields are required!",
                "Validation Error",
                "OK"
                );
            return;
        }

        // **Password Length Check**
        if (password.length() < 6) {
            MessageBoxUtil::showCustomMessage(
                this,
                "Password must be at least 6 characters long!",
                "Validation Error",
                "OK"
                );
            return;
        }

        // **Check if the database is connected**
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

        // **Check if email already exists**
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT 1 FROM vls_schema.users WHERE email = :email LIMIT 1");
        checkQuery.addBindValue(email);

        if (!checkQuery.exec()) {
            MessageBoxUtil::showCustomMessage(
                this,
                "Failed to check existing user: " + checkQuery.lastError().text(),
                "Database Error",
                "OK"
                );
            return;
        }

        if (checkQuery.next()) {
            MessageBoxUtil::showCustomMessage(
                this,
                "Email is already registered!",
                "Registration Error",
                "OK"
                );
            return;
        }

        QString newUniqueID = generateUniqueID(role, department);

        // **Insert new user**
        QSqlQuery query;
        if (role == "Student") {
            role = role.toLower();
            query.prepare("INSERT INTO vls_schema.users (first_name, last_name, email, password_hash, phone, gender, role, department, unique_id, fee_status) "
                          "SELECT :first_name, :last_name, :email, crypt(:password, gen_salt('bf')), :phone, :gender, :role, :department, :unique_id, :fee_status");
            query.bindValue(":fee_status", "Paid");
            query.bindValue(":department", department);
        } else {
            role = role.toLower();
            query.prepare("INSERT INTO vls_schema.users (first_name, last_name, email, password_hash, phone, gender, role, unique_id) "
                          "SELECT :first_name, :last_name, :email, crypt(:password, gen_salt('bf')), :phone, :gender, :role, :unique_id");
        }

        // Common fields
        query.bindValue(":first_name", firstName);
        query.bindValue(":last_name", lastName);
        query.bindValue(":email", email);
        query.bindValue(":password", password);
        query.bindValue(":phone", phone);
        query.bindValue(":gender", gender);
        query.bindValue(":role", role);
        query.bindValue(":unique_id", newUniqueID);

        qDebug() << "Query: " << query.executedQuery();
        qDebug() << "first_name: " << firstName;
        qDebug() << "password: " << password;

        if (!query.exec()) {
            MessageBoxUtil::showCustomMessage(
                this,
                "Query execution failed: " + query.lastError().text(),
                "Database Error",
                "OK"
                );
            return;
        }

        // ✅ Success Message
        MessageBoxUtil::showCustomMessage(
            this,
            "User registered successfully!",
            "Success",
            "OK"
            );
        reset();


    });
}
QString RegisterUserForm::generateUniqueID(QString role, QString department){
    QString prefix;
    if(role=="Student") prefix =department + "-";
    else if(role=="Teacher") prefix = "TCH-";
    else prefix = "AD-";
    QSqlQuery query;
    query.prepare("SELECT unique_id FROM vls_schema.users WHERE unique_id LIKE :prefix ORDER BY unique_id DESC LIMIT 1");
    query.bindValue(":prefix",prefix+"%");
    if(!query.exec()|| !query.next()){
        return prefix+ "001";
    }
    QString lastID = query.value(0).toString();
    int lastNumber = lastID.section('-',1,1).toInt();
    lastNumber++;
    return prefix+ QString("%1").arg(lastNumber,3,10,QChar('0'));
}
void RegisterUserForm::reset(){
    firstNameField->clear();
    lastNameField->clear();
    emailField->clear();
    passwordField->clear();
    phoneField->clear();
    userTypeDropdown->setCurrentIndex(0); // Reset dropdown to first item
    genderDropdown->setCurrentIndex(0);
    departmentDropdown->setCurrentIndex(0);
}
RegisterUserForm::~RegisterUserForm(){

}
