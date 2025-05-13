#include "StudentUploadPDF.h"
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "mesgboxutil.h"

StudentUploadPDF::StudentUploadPDF(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-color: #0D1B2A;");
    // setWindowState(Qt::WindowMaximized);
    this->setMinimumSize(800, 700);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Initialize network manager
    networkManager = new QNetworkAccessManager(this);

    QWidget *functionalWindow = new QWidget(this);
    functionalWindow->setStyleSheet("background-color: #283044; border-radius: 20px;");
    functionalWindow->setFixedSize(1000, 700);

    QVBoxLayout *functionalLayout = new QVBoxLayout(functionalWindow);
    functionalLayout->setAlignment(Qt::AlignCenter);

    assignmentDetails = new QLabel("Assignment Details", this);
    assignmentDetails->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    assignmentDetails->setAlignment(Qt::AlignCenter);
    assignmentDetails->setFixedSize(950, 50);
    functionalLayout->addWidget(assignmentDetails, 0, Qt::AlignCenter);

    functionalLayout->addSpacing(20);

    QHBoxLayout *downloadLayout = new QHBoxLayout();
    downloadLayout->setAlignment(Qt::AlignCenter);

    QPushButton *downloadPdfIconButton = new QPushButton(this);
    downloadPdfIconButton->setIcon(QIcon("images/pdficon.png"));
    downloadPdfIconButton->setIconSize(QSize(40, 40));
    downloadPdfIconButton->setCursor(Qt::PointingHandCursor);
    downloadPdfIconButton->setStyleSheet(
        "QPushButton { background-color: white; border: none; padding: 5px; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        "QPushButton:pressed { background-color: rgba(255, 255, 255, 0.2); border-radius: 5px; }"
        );
    downloadLayout->addWidget(downloadPdfIconButton);

    downloadButton = new QPushButton("Download Assignment", this);
    downloadButton->setCursor(Qt::PointingHandCursor);
    downloadButton->setStyleSheet(
        "QPushButton { padding: 12px; font-size: 16px; background-color: #212735; color: white; border-radius: 20px; }"
        "QPushButton:hover { background-color: #2E3542; }"
        "QPushButton:pressed { background-color: #3A4252; }"
        );
    connect(downloadButton, &QPushButton::clicked, this, &StudentUploadPDF::downloadAssignment);
    downloadLayout->addWidget(downloadButton);

    functionalLayout->addLayout(downloadLayout);
    functionalLayout->addSpacing(20);

    teacherDescription = new QTextBrowser(this);
    teacherDescription->setStyleSheet(
        "QTextBrowser { background-color: #212735; color: white; border-radius: 10px; padding: 8px; font-size: 14px; }"
        );
    teacherDescription->setFixedSize(950, 100);
    teacherDescription->setReadOnly(true);
    functionalLayout->addWidget(teacherDescription, 0, Qt::AlignCenter);

    functionalLayout->addSpacing(20);

    pdfFileLabel = new QLabel("No Assignment PDF selected", this);
    pdfFileLabel->setAlignment(Qt::AlignCenter);
    pdfFileLabel->setStyleSheet("color: white; font-size: 14px;");
    pdfFileLabel->setFixedSize(950, 50);
    functionalLayout->addWidget(pdfFileLabel, 0, Qt::AlignCenter);

    QHBoxLayout *fileSelectionLayout = new QHBoxLayout();
    fileSelectionLayout->setAlignment(Qt::AlignCenter);

    QPushButton *pdfIconButton = new QPushButton(this);
    pdfIconButton->setIcon(QIcon("images/pdficon.png"));
    pdfIconButton->setIconSize(QSize(40, 40));
    pdfIconButton->setCursor(Qt::PointingHandCursor);
    pdfIconButton->setStyleSheet(
        "QPushButton { background-color: white; border: none; padding: 5px; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        "QPushButton:pressed { background-color: rgba(255, 255, 255, 0.2); border-radius: 5px; }"
        );
    fileSelectionLayout->addWidget(pdfIconButton);

    fileButton = new QPushButton("Select Assignment PDF", this);
    fileButton->setCursor(Qt::PointingHandCursor);
    fileButton->setStyleSheet(
        "QPushButton { padding: 12px; font-size: 16px; background-color: #212735; color: white; border-radius: 20px; }"
        "QPushButton:hover { background-color: #2E3542; }"
        "QPushButton:pressed { background-color: #3A4252; }"
        );
    connect(fileButton, &QPushButton::clicked, this, &StudentUploadPDF::selectFile);
    fileSelectionLayout->addWidget(fileButton);

    functionalLayout->addLayout(fileSelectionLayout);
    functionalLayout->addSpacing(20);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);

    uploadButton = new QPushButton("Upload Assignment", this);
    resetButton = new QPushButton("Reset", this);

    const QString buttonStyle = R"(
        QPushButton { color: white; padding: 15px; border-radius: 20px; }
        QPushButton:hover { filter: brightness(1.2); }
        QPushButton:pressed { filter: brightness(0.8); }
    )";

    uploadButton->setFixedSize(150, 50);
    uploadButton->setStyleSheet("background-color: #0078D4;" + buttonStyle);
    uploadButton->setVisible(false);

    resetButton->setFixedSize(150, 50);
    resetButton->setStyleSheet("background-color: #E04A4A;" + buttonStyle);
    resetButton->setVisible(false);

    buttonLayout->addWidget(uploadButton);
    buttonLayout->addWidget(resetButton);
    functionalLayout->addLayout(buttonLayout);

    mainLayout->addWidget(functionalWindow, 0, Qt::AlignCenter);

    connect(uploadButton, &QPushButton::clicked, this, &StudentUploadPDF::uploadFile);
    connect(resetButton, &QPushButton::clicked, this, &StudentUploadPDF::resetScreen);
}

void StudentUploadPDF::setAssignmentDetails(const QString& title, const QString& description, const QString& pdfPath) {
    assignmentDetails->setText("Assignment: " + title);
    teacherDescription->setText(description);
    teacherFilePath = pdfPath;
    qDebug() << "setAssignmentDetails: teacherFilePath =" << teacherFilePath;
    resetScreen();
}

void StudentUploadPDF::selectFile() {
    filePath = QFileDialog::getOpenFileName(this, "Select Assignment PDF", "", "PDF Files (*.pdf)");
    if (!filePath.isEmpty()) {
        pdfFileLabel->setText("Selected: " + filePath);
        fileButton->setText("Open PDF");
        disconnect(fileButton, &QPushButton::clicked, this, &StudentUploadPDF::selectFile);
        connect(fileButton, &QPushButton::clicked, this, &StudentUploadPDF::openPDF);
        uploadButton->setVisible(true);
        resetButton->setVisible(true);
    }
}

void StudentUploadPDF::resetScreen() {
    filePath.clear();
    pdfFileLabel->setText("No Assignment PDF selected");
    pdfFileLabel->setStyleSheet("color: white; font-size: 14px;");
    fileButton->setText("Select Assignment PDF");
    disconnect(fileButton, &QPushButton::clicked, this, &StudentUploadPDF::openPDF);
    connect(fileButton, &QPushButton::clicked, this, &StudentUploadPDF::selectFile);
    uploadButton->setVisible(false);
    resetButton->setVisible(false);
}

void StudentUploadPDF::uploadFile() {
    if (filePath.isEmpty()) return;
    MessageBoxUtil::showCustomMessage(this, "Assignment uploaded successfully!",
                                      "Upload", "OK");
    pdfFileLabel->setStyleSheet("color: white; font-size: 14px; background-color: #4CAF50; border-radius: 10px; padding: 5px;");
    pdfFileLabel->setText("Uploaded: " + filePath);
}

void StudentUploadPDF::openPDF() {
    if (filePath.isEmpty()) return;
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void StudentUploadPDF::downloadAssignment() {
    if (teacherFilePath.isEmpty()) {
        qDebug() << "downloadAssignment: teacherFilePath is empty";
        MessageBoxUtil::showCustomMessage(this, "No assignment file available!",
                                          "Download", "OK");
        return;
    }

    // Validate URL
    QUrl url(teacherFilePath);
    if (!url.isValid()) {
        qDebug() << "downloadAssignment: Invalid URL =" << teacherFilePath;
        MessageBoxUtil::showCustomMessage(this, "Invalid assignment file URL!",
                                          "Download", "OK");
        return;
    }

    // Extract file name from URL or use default
    QString fileName = QFileInfo(url.path()).fileName();
    if (fileName.isEmpty()) {
        fileName = "assignment.pdf";
    }
    qDebug() << "downloadAssignment: fileName =" << fileName;

    // Ask user where to save the file
    QString savePath = QFileDialog::getSaveFileName(
        this, "Save Assignment",
        QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + fileName,
        "PDF Files (*.pdf)"
        );

    if (savePath.isEmpty()) {
        qDebug() << "downloadAssignment: User cancelled save dialog";
        return; // User cancelled
    }
    qDebug() << "downloadAssignment: savePath =" << savePath;

    // Download file using QNetworkAccessManager
    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        qDebug() << "downloadAssignment: Network reply finished";
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            qDebug() << "downloadAssignment: Downloaded" << data.size() << "bytes";

            QFile file(savePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(data);
                file.close();
                qDebug() << "downloadAssignment: File saved successfully to" << savePath;
                MessageBoxUtil::showCustomMessage(this, "Assignment downloaded successfully to: " + savePath,
                                                  "Download", "OK");
            } else {
                qDebug() << "downloadAssignment: Failed to open file for writing, error =" << file.errorString();
                MessageBoxUtil::showCustomMessage(this, "Failed to save file: " + file.errorString(),
                                                  "Download", "OK");
            }
        } else {
            qDebug() << "downloadAssignment: Network error =" << reply->errorString();
            MessageBoxUtil::showCustomMessage(this, "Failed to download assignment: " + reply->errorString(),
                                              "Download", "OK");
        }
        reply->deleteLater();
    });
}
