    #ifndef MESSAGEBOXUTIL_H
#define MESSAGEBOXUTIL_H

#include <QMessageBox>
#include <QString>
#include <QPixmap>

class MessageBoxUtil
{
public:
    // Show a message box with a single customizable button
    static void showCustomMessage(QWidget* parent,
                                  const QString& message,
                                  const QString& windowTitle,
                                  const QString& buttonText);

    // Show a message box with two customizable buttons
    static bool showCustomQuestion(QWidget* parent,
                                   const QString& message,
                                   const QString& windowTitle,
                                   const QString& button1Text,
                                   const QString& button2Text,
                                   const QString& imagePath = QString());

private:
    static void setupMessageBox(QMessageBox& msgBox,
                                const QString& message,
                                bool centerSingleButton);
};

#endif // MESSAGEBOXUTIL_H
