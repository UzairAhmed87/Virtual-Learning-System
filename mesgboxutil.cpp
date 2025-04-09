#include "mesgboxutil.h"
#include <QPushButton>
#include <QBoxLayout>

void MessageBoxUtil::showCustomMessage(QWidget* parent,
                                       const QString& message,
                                       const QString& windowTitle,
                                       const QString& buttonText)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(windowTitle);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::NoButton);
    msgBox.addButton(buttonText, QMessageBox::AcceptRole);

    // Setting up the message box with custom properties
    setupMessageBox(msgBox, message, true);
    msgBox.exec();
}

bool MessageBoxUtil::showCustomQuestion(QWidget* parent,
                                        const QString& message,
                                        const QString& windowTitle,
                                        const QString& button1Text,
                                        const QString& button2Text,
                                        const QString& imagePath)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(windowTitle);
    msgBox.setText(message);

    // Add custom buttons for Yes/No confirmation
    QPushButton* btn1 = msgBox.addButton(button1Text, QMessageBox::YesRole);
    QPushButton* btn2 = msgBox.addButton(button2Text, QMessageBox::NoRole);

    // Setting up the message box with custom properties
    setupMessageBox(msgBox, message, false);
    msgBox.exec();

    // Return whether the first button (Yes) was clicked
    return msgBox.clickedButton() == btn1;
}

void MessageBoxUtil::setupMessageBox(QMessageBox& msgBox,
                                     const QString& message,
                                     bool centerSingleButton)
{
    // Set the message in the message box
    msgBox.setText(message);

    // If an image path is provided, set the image in the message box
    QString fixedImagePath = "../images/icon.png";  // 💡 Use resource system if possible
    QPixmap pixmap(fixedImagePath);
    if (!pixmap.isNull()) {
        msgBox.setIconPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Set a fixed size or min size for the message box to make the window consistent
    // msgBox.setFixedSize(400, 200); // Set your desired size (width, height)

    // If only one button exists and it should be centered
    if (centerSingleButton && msgBox.buttons().size() == 1) {
        QPushButton* btn = qobject_cast<QPushButton*>(msgBox.buttons().first());
        if (btn) {
            btn->setStyleSheet("margin-left: auto; margin-right: 0;");
        }
    }

    // Apply custom stylesheet to the message box and buttons
    msgBox.setStyleSheet(R"(
    QMessageBox { background-color: #1b263b; color: white; font-size: 16px; }
    QLabel { background-color: transparent; color: white; }
    QPushButton {
        background-color: #1E90FF;
        color: white;
        border: 2px solid #1E90FF;
        border-radius: 5px;
        padding: 8px;
    }
    QPushButton:hover {
        background-color: transparent;
    }
)");
}
