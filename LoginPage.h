#ifndef LOGINPAGE_H
#define LOGINPAGE_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QEvent>

// Custom ClickableLabel class for clickable text elements like "Forgot Password"
class ClickableLabel : public QLabel {
    Q_OBJECT
public:
    explicit ClickableLabel(const QString& text = "", QWidget* parent = nullptr)
        : QLabel(text, parent) {
        setCursor(Qt::PointingHandCursor);
    }
    ~ClickableLabel() = default;
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent*) override {
        emit clicked();
    }
};

class LoginPage : public QWidget
{
    Q_OBJECT
public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

signals:
    void loginSuccessful(QString role, QString email);  // Signal emitted when login is successful

private slots:
    void handleLogin();
    void handleForgotPassword();
    void showLoginDialog();
    void updateRoleSelection();
    void  togglePasswordVisibility(bool checked);
private:
    // UI initialization methods
    void initializeUI();
    QWidget* createNavBar();
    QWidget* createLeftPanel();
    QWidget* createRightPanel();
    QWidget* createRoleWidget(const QString& roleName, const QString& iconPath);
    void updateRoleFrameStyles();
    QString getStyleSheet();

    // Event filter for handling clicks on role frames
    bool eventFilter(QObject* watched, QEvent* event) override;

    // Login dialog and UI elements
    QWidget* loginDialog = nullptr;
    QLineEdit* emailField = nullptr;
    QLineEdit* passwordField = nullptr;
    QPushButton* loginButton = nullptr;
    QCheckBox* showPasswordCheckbox = nullptr;
    ClickableLabel* forgotPasswordLabel = nullptr;
    QLabel* welcomeLabel = nullptr;  // Added to store reference to the welcome label

    // Role selection elements
    QButtonGroup* roleButtonGroup = nullptr;
    QRadioButton* studentButton = nullptr;
    QRadioButton* teacherButton = nullptr;
    QRadioButton* adminButton = nullptr;
    QString currentRole;
};

#endif // LOGINPAGE_H
