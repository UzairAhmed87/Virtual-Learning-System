#ifndef UPLOADPDF_H
#define UPLOADPDF_H

#include <QWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QTextEdit>
#include "TopBar.h"
#include <QComboBox>

class UploadPdf : public QWidget {
    Q_OBJECT
public:
    explicit UploadPdf(const QString &ID="",QWidget *parent = nullptr,QWidget *topBar=nullptr);

private slots:
    void selectFile();      // Open PDF
    void resetScreen();     // Reset UI
    void openPDF();         // Open selected PDF
    void uploadFile();
    void insertFileMetadata(const QString &fileUrl);
    void loadCourses();
private:
    QComboBox *courseDropdown;
    QString filePath;
    QLabel *pdfFileLabel;   // Label to show selected file path
    QPushButton *selectButton, *uploadButton, *resetButton;
    // TopBar *topBar;
    QPushButton *openButton;
    QTextEdit *descInput;
    QPushButton *fileButton;
    QString uniqueId;
signals:
    void backButtonClicked();
};

#endif // UPLOADPDF_H
