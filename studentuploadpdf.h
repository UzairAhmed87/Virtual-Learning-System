#ifndef STUDENTUPLOADPDF_H
#define STUDENTUPLOADPDF_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QNetworkAccessManager>

class StudentUploadPDF : public QWidget {
    Q_OBJECT
public:
    explicit StudentUploadPDF(QWidget *parent = nullptr);
    void setAssignmentDetails(const QString& title, const QString& description, const QString& pdfPath);

private slots:
    void selectFile();
    void resetScreen();
    void uploadFile();
    void openPDF();
    void downloadAssignment();

private:
    QLabel *assignmentDetails;
    QTextBrowser *teacherDescription;
    QLabel *pdfFileLabel;
    QPushButton *fileButton;
    QPushButton *uploadButton;
    QPushButton *resetButton;
    QPushButton *downloadButton;
    QString filePath;
    QString teacherFilePath;
    QNetworkAccessManager *networkManager;
};

#endif // STUDENTUPLOADPDF_H
