#ifndef UPLOADPDF_H
#define UPLOADPDF_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QFile>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDesktopServices>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QLineEdit>
#include "DatabaseManager.h"
#include "mesgboxutil.h"
#include "SupabaseConfig.h"

class UploadPdf : public QWidget {
    Q_OBJECT

public:
    explicit UploadPdf(const QString &ID, QWidget *parent = nullptr, QWidget *topBar = nullptr);

private slots:
    void selectFile();
    void uploadFile();
    void resetScreen();
    void openPDF();
    void loadCourses();

signals:
    void backButtonClicked();

private:
    void insertFileMetadata(const QString &fileUrl);

    QString filePath;
    QString uniqueId;
    QLabel *pdfFileLabel;
    QPushButton *fileButton;
    QPushButton *uploadButton;
    QPushButton *resetButton;
    QComboBox *courseDropdown;
    QTextEdit *descInput;
    QLineEdit *titleInput;
    QTextEdit *descriptionInput;
    QDateTimeEdit *deadlineInput;
    QNetworkAccessManager *networkManager;
};

#endif // UPLOADPDF_H
