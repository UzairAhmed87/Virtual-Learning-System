#include "LoginPage.h"
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QFrame>
#include <QButtonGroup>
#include <QGraphicsDropShadowEffect>
#include <QStyle>
#include <QRegularExpression>
#include "DatabaseManager.h"
#include "mesgboxutil.h"

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent), loginDialog(nullptr)
{
    initializeUI();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(800, 500);
    setStyleSheet(getStyleSheet());

    // Signal connections will be made when the UI elements are created
}

LoginPage::~LoginPage()
{
    // No need to delete UI elements as they are children of this widget
    // and will be cleaned up automatically
}

void LoginPage::initializeUI()
{
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Main content
    QWidget* contentWidget = new QWidget();
    QHBoxLayout* contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    // Left panel with intro text
    QWidget* leftPanel = createLeftPanel();
    contentLayout->addWidget(leftPanel);

    // Right panel with login dialog
    QWidget* rightPanel = createRightPanel();
    contentLayout->addWidget(rightPanel);

    mainLayout->addWidget(contentWidget, 1);

    // Set initial role
    currentRole = "Student";
}

QWidget* LoginPage::createLeftPanel()
{
    QWidget* leftPanel = new QWidget();
    leftPanel->setObjectName("leftPanel");

    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(50, 250, 20, 80);
    leftLayout->setSpacing(20);

    // Headline text
    QLabel* headlineLabel = new QLabel("Get started with");
    headlineLabel->setObjectName("headlineLabel");

    QLabel* subHeadlineLabel = new QLabel("ONLINE EDUCATION!");
    subHeadlineLabel->setObjectName("subHeadlineLabel");

    QLabel* descriptionLabel = new QLabel("Onboard and train your students\nwith powerful e-learning solution");
    descriptionLabel->setObjectName("descriptionLabel");
    descriptionLabel->setWordWrap(true);

    // Start button
    QPushButton* startButton = new QPushButton("Start!");
    startButton->setObjectName("startButton");
    startButton->setFixedSize(150, 50);
    leftLayout->setAlignment(Qt::AlignCenter);
    // Add widgets to layout
    leftLayout->addWidget(headlineLabel);
    leftLayout->addWidget(subHeadlineLabel);
    leftLayout->addWidget(descriptionLabel);
    leftLayout->addSpacing(30);
    leftLayout->addWidget(startButton);
    leftLayout->addStretch(1);

    connect(startButton, &QPushButton::clicked, this, &LoginPage::showLoginDialog);

    return leftPanel;
}

QWidget* LoginPage::createRightPanel()
{
    QWidget* rightPanel = new QWidget();
    rightPanel->setObjectName("rightPanel");

    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(20, 20, 50, 20);

    // Add illustration image
    QLabel* illustrationLabel = new QLabel();
    illustrationLabel->setObjectName("illustrationLabel");
    QPixmap illustration(":/images/BigLogo.png"); // Keeping the original path as requested
    if (illustration.isNull()) {
        illustrationLabel->setText("[Logo Placeholder]");
    } else {
        illustrationLabel->setPixmap(illustration.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    illustrationLabel->setAlignment(Qt::AlignCenter);

    rightLayout->addWidget(illustrationLabel, 1);

    return rightPanel;
}

void LoginPage::showLoginDialog()
{
    // Create login dialog if it doesn't exist yet
    if (!loginDialog) {
        loginDialog = new QWidget(this);
        loginDialog->setObjectName("loginDialog");
        loginDialog->setMinimumSize(500, 650);

        // Add drop shadow effect
        QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect();
        shadowEffect->setBlurRadius(20);
        shadowEffect->setColor(QColor(0, 0, 0, 60));
        shadowEffect->setOffset(0, 0);
        loginDialog->setGraphicsEffect(shadowEffect);

        // Create layout
        QVBoxLayout* dialogLayout = new QVBoxLayout(loginDialog);
        dialogLayout->setContentsMargins(30, 30, 30, 30);
        dialogLayout->setSpacing(20);


        // Close button
        QPushButton* closeButton = new QPushButton("x");
        closeButton->setObjectName("closeButton");
        closeButton->setFixedSize(30, 30);
        connect(closeButton, &QPushButton::clicked, loginDialog, &QWidget::hide);

        QHBoxLayout* headerLayout = new QHBoxLayout();
        headerLayout->addStretch(1);
        headerLayout->addWidget(closeButton);
        dialogLayout->addLayout(headerLayout);

        // Title
        QLabel* titleLabel = new QLabel("Choose Account Type");
        titleLabel->setObjectName("dialogTitle");
        titleLabel->setAlignment(Qt::AlignCenter);
        dialogLayout->addWidget(titleLabel);

        // Account type selection
        QHBoxLayout* accountTypeLayout = new QHBoxLayout();
        accountTypeLayout->setSpacing(15);

        // Create role selection widgets
        roleButtonGroup = new QButtonGroup(this);

        // Student option
        QWidget* studentWidget = createRoleWidget("Student", ":/images/student.png");
        studentWidget->setStyleSheet("background-color: white; ");
        studentButton = qobject_cast<QRadioButton*>(studentWidget->findChild<QRadioButton*>());
        roleButtonGroup->addButton(studentButton);
        accountTypeLayout->addWidget(studentWidget);

        // Teacher option
        QWidget* teacherWidget = createRoleWidget("Teacher", ":/images/teacher.png");
        teacherWidget->setStyleSheet("background-color: white; ");
        teacherButton = qobject_cast<QRadioButton*>(teacherWidget->findChild<QRadioButton*>());
        roleButtonGroup->addButton(teacherButton);
        accountTypeLayout->addWidget(teacherWidget);

        // Admin option
        QWidget* adminWidget = createRoleWidget("Admin", ":/images/admin.png");
        adminWidget->setStyleSheet("background-color: white; ");
        adminButton = qobject_cast<QRadioButton*>(adminWidget->findChild<QRadioButton*>());
        roleButtonGroup->addButton(adminButton);
        accountTypeLayout->addWidget(adminWidget);

        dialogLayout->addLayout(accountTypeLayout);

        // Welcome message
        welcomeLabel = new QLabel("Hello student!");
        welcomeLabel->setObjectName("welcomeLabel");
        welcomeLabel->setAlignment(Qt::AlignCenter);
        welcomeLabel->setFixedHeight(40);
        dialogLayout->addWidget(welcomeLabel);

        QLabel* instructionLabel = new QLabel("Please fill out the form below to get started");
        instructionLabel->setObjectName("instructionLabel");
        instructionLabel->setAlignment(Qt::AlignCenter);
        instructionLabel->setFixedHeight(10);
        dialogLayout->addWidget(instructionLabel);

        dialogLayout->addSpacing(10);

        // Login form
        emailField = new QLineEdit();
        emailField->setObjectName("emailField");
        emailField->setPlaceholderText("Username or email");
        dialogLayout->addWidget(emailField);

        passwordField = new QLineEdit();
        passwordField->setObjectName("passwordField");
        passwordField->setPlaceholderText("Password");
        passwordField->setEchoMode(QLineEdit::Password);
        dialogLayout->addWidget(passwordField);

        // Remember me & Login button
        QHBoxLayout* loginOptionsLayout = new QHBoxLayout();
        loginOptionsLayout->setSpacing(10);

        showPasswordCheckbox = new QCheckBox("Show password");
        showPasswordCheckbox->setObjectName("showPasswordCheckbox");
        showPasswordCheckbox->setStyleSheet("background-color: transparent; "
                                          "font-size: 14px;"
                                          "color: white;");
        connect(showPasswordCheckbox, &QCheckBox::toggled, this, &LoginPage::togglePasswordVisibility);
        loginButton = new QPushButton("LOGIN");
        loginButton->setObjectName("loginButton");
        loginButton->setFixedHeight(40);

        loginOptionsLayout->addWidget(showPasswordCheckbox);
        loginOptionsLayout->addStretch(1);
        loginOptionsLayout->addWidget(loginButton);

        dialogLayout->addLayout(loginOptionsLayout);

        // Forgot password
        // forgotPasswordLabel = new ClickableLabel("Forgot password?");
        // forgotPasswordLabel->setObjectName("forgotPasswordLabel");
        // forgotPasswordLabel->setAlignment(Qt::AlignCenter);
        // dialogLayout->addWidget(forgotPasswordLabel);

        dialogLayout->addSpacing(0);

        // // Social login options
        // QHBoxLayout* socialLoginLayout = new QHBoxLayout();
        // socialLoginLayout->setContentsMargins(50, 0, 50, 0);

        // QPushButton* googleLoginButton = new QPushButton("Sign in with Google");
        // googleLoginButton->setObjectName("googleLoginButton");
        // googleLoginButton->setIcon(QIcon(":/images/icon.png")); // Keep original path

        // //socialLoginLayout->addWidget(googleLoginButton);
        // dialogLayout->addLayout(socialLoginLayout);

        // Connect signals with slots now that the elements are created
        connect(loginButton, &QPushButton::clicked, this, &LoginPage::handleLogin);
        connect(emailField, &QLineEdit::returnPressed, this, &LoginPage::handleLogin);
        connect(passwordField, &QLineEdit::returnPressed, this, &LoginPage::handleLogin);
        connect(forgotPasswordLabel, &ClickableLabel::clicked, this, &LoginPage::handleForgotPassword);
        connect(studentButton, &QRadioButton::toggled, this, &LoginPage::updateRoleSelection);
        connect(teacherButton, &QRadioButton::toggled, this, &LoginPage::updateRoleSelection);
        connect(adminButton, &QRadioButton::toggled, this, &LoginPage::updateRoleSelection);

        // Set initial role
        studentButton->setChecked(true);

        // Center the dialog in the parent
        loginDialog->setGeometry(
            QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                loginDialog->size(),
                this->rect()
                )
            );
    }

    // Show the dialog
    loginDialog->show();
}
void LoginPage::togglePasswordVisibility(bool checked)
{
    if (checked) {
        passwordField->setEchoMode(QLineEdit::Normal);
    } else {
        passwordField->setEchoMode(QLineEdit::Password);
    }
}

QWidget* LoginPage::createRoleWidget(const QString& roleName, const QString& iconPath)
{
    QFrame* roleFrame = new QFrame();
    roleFrame->setObjectName("roleFrame_" + roleName.toLower());
    roleFrame->setFrameShape(QFrame::StyledPanel);
    roleFrame->setFixedSize(120, 140);

    QVBoxLayout* frameLayout = new QVBoxLayout(roleFrame);
    frameLayout->setContentsMargins(10, 10, 10, 10);
    frameLayout->setSpacing(5);

    // Icon
    QLabel* iconLabel = new QLabel();
    QPixmap icon(iconPath);
    if (icon.isNull()) {
        // Fallback to placeholder
        iconLabel->setText("[" + roleName + "]");
        iconLabel->setAlignment(Qt::AlignCenter);
    } else {
        iconLabel->setPixmap(icon.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        iconLabel->setAlignment(Qt::AlignCenter);
    }

    // Role text
    QLabel* roleLabel = new QLabel(roleName);
    roleLabel->setAlignment(Qt::AlignCenter);
    roleLabel->setObjectName("roleLabel");

    // Hidden radio button
    QRadioButton* roleRadio = new QRadioButton();
    roleRadio->setObjectName("roleRadio_" + roleName.toLower());
    roleRadio->hide(); // Make it invisible but functional

    frameLayout->addWidget(iconLabel);
    frameLayout->addWidget(roleLabel);
    frameLayout->addWidget(roleRadio);

    // Make the frame clickable to select the radio button
    roleFrame->installEventFilter(this);

    return roleFrame;
}

void LoginPage::updateRoleSelection()
{
    // Update current role based on which radio button is checked
    if (studentButton && studentButton->isChecked()) {
        currentRole = "Student";
        if (welcomeLabel) {
            welcomeLabel->setText("Hello Student!");
        }
    }
    else if (teacherButton && teacherButton->isChecked()) {
        currentRole = "Teacher";
        if (welcomeLabel) {
            welcomeLabel->setText("Hello Teacher!");
        }
    }
    else if (adminButton && adminButton->isChecked()) {
        currentRole = "Admin";
        if (welcomeLabel) {
            welcomeLabel->setText("Hello Admin!");
        }
    }

    // Update style sheet to highlight selected role
    updateRoleFrameStyles();
}

void LoginPage::updateRoleFrameStyles()
{
    if (!loginDialog) return;

    // Find all role frames
    QList<QFrame*> frames = loginDialog->findChildren<QFrame*>(QRegularExpression("^roleFrame_.*"));

    for (QFrame* frame : frames) {
        if (frame->objectName() == "roleFrame_" + currentRole.toLower()) {
            frame->setProperty("selected", true);
        } else {
            frame->setProperty("selected", false);
        }

        // Force style update
        frame->style()->unpolish(frame);
        frame->style()->polish(frame);
    }
}

void LoginPage::handleLogin()
{
    QString email = emailField->text();
    QString password = passwordField->text();
    QString role = currentRole;

    if (email.isEmpty() || password.isEmpty()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Please enter both email and password.âš ï¸ ",
                       "Login Error",
            "OK"
            );
        return;
    }

    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Database connection is not open! ",
            "Database Error",
            "OK"
            );
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id, first_name, last_name, email, phone, gender, role, department "
                  "FROM vls_schema.users "
                  "WHERE email = :email "
                  "AND password_hash = crypt(:password, password_hash) "
                  "AND LOWER(role) = LOWER(:role)");

    query.bindValue(":email", email);
    query.bindValue(":password", password);
    query.bindValue(":role", role);

    if (!query.exec()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Query execution failed: " + query.lastError().text(),
            "Database Error",
            "OK"
            );
        return;
    }

    if (query.next()) {
        QString firstName = query.value(1).toString();
        MessageBoxUtil::showCustomMessage(
            this,
            " Welcome " + firstName + "!",
            "Login Successful",
            "OK"
            );

        // Hide login dialog if shown
        if (loginDialog && loginDialog->isVisible()) {
            loginDialog->hide();
        }

        emit loginSuccessful(role, email);
    } else {
        MessageBoxUtil::showCustomMessage(
            this,
            "Invalid email, password, or role.",
            "Login Failed",
            "OK"
            );
    }
}

void LoginPage::handleForgotPassword()
{
    MessageBoxUtil::showCustomMessage(
        this,
        "Please contact your administrator to reset your password.",
        "Password Recovery",
        "OK"
        );
}

bool LoginPage::eventFilter(QObject* watched, QEvent* event)
{
    // Handle clicks on role frames
    if (event->type() == QEvent::MouseButtonPress) {
        QFrame* frame = qobject_cast<QFrame*>(watched);
        if (frame && frame->objectName().startsWith("roleFrame_")) {
            // Find the radio button inside this frame and click it
            QRadioButton* radio = frame->findChild<QRadioButton*>();
            if (radio) {
                radio->setChecked(true);
                return true;
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

QString LoginPage::getStyleSheet()
{
    return R"(
        /* Main widget styles */
        QWidget {
            font-family: Arial, sans-serif;
            color: #333333;
            background-color: #0d1b2a;
        }

        /* Navigation bar styles */
        #navBar {
            background-color: #0d1b2a;
            border-bottom: 1px solid #e0e0e0;
        }

        #logoLabel {
            font-size: 22px;
            font-weight: bold;
            color: #4461F2;
        }

        #navLink {
            color: #555555;
            font-size: 14px;
        }

        #loginSignupLink {
            color: #4461F2;
            font-size: 14px;
            font-weight: bold;
        }

        /* Left panel styles */
        #leftPanel {
            background-color: #0d1b2a;
        }

        #headlineLabel {
            font-size: 32px;
            color: #7f8c8d;
        }

        #subHeadlineLabel {
            font-size: 36px;
            font-weight: bold;
            color: #1E90FF;
        }

        #descriptionLabel {
            font-size: 16px;
            color: white;
            margin-top: 10px;
        }

        #startButton {
            background-color: #1E90FF;
            color: white;
            border-radius: 25px;
            padding: 10px 20px;
            font-size: 16px;
            font-weight: bold;
            border: 2px solid #1E90FF;
        }

        #startButton:hover {
            background:transparent;
        }

        /* Login dialog styles */
        #loginDialog {
            background-color: #1b263a;
            border-radius: 10px;
        }

        #closeButton {
            background-color: transparent;
            color: #555555;
            font-size: 24px;
            border: none;
        }
        #closeButton:hover{
            background-color:#1E90FF;
        }
        #dialogTitle {
            font-size: 22px;
            font-weight: bold;
            color: #1E90FF;
             background-color: transparent;
        }

        #welcomeLabel {
            font-size: 18px;
            color: white;
            margin-top: 20px;
            background-color: transparent;
        }

        #instructionLabel {
            font-size: 14px;
            color: #7f8c8d;
            background-color: transparent;
        }

        #emailField, #passwordField {
            padding: 12px;
            border: 1px solid #e0e0e0;
            border-radius: 4px;
            font-size: 14px;
            background-color: white;
        }

        #loginButton {
            background-color: #1E90FF;
            color: white;
            border: 2px solid #1E90FF;
            border-radius: 4px;
            padding: 10px 20px;
            font-size: 14px;
            font-weight: bold;
            min-width: 120px;
        }

        #loginButton:hover {
            background:transparent;
        }

        #forgotPasswordLabel {
            color: #7f8c8d;
            font-size: 12px;
            cursor: pointer;
             background-color: transparent;
        }

        #forgotPasswordLabel:hover {
            color: #4461F2;
            text-decoration: underline;
        }

        #googleLoginButton {
            background-color: #4285F4;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-size: 14px;
        }

        #googleLoginButton:hover {
            background-color: #3367D6;
        }

        /* Role selection styles */
        QFrame[objectName^="roleFrame_"] {
            background-color: transparent;
            border: 1px solid #e0e0e0;
            border-radius: 6px;
            color: white;
       }

        QFrame[objectName^="roleFrame_"]:hover {
            border-color: #8E44F2;
            color: white;
            background-color: white;
        }

        QFrame[selected="true"] {
            border: 5px solid #8E44F2;
            background-color: #F5F0FF;
        }

        #roleLabel {
            font-size: 14px;
            font-weight: bold;
            color: #1E90FF;
            background-color: white;
        }
    )";
}
