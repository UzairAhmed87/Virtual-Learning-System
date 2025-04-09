#include "uploadpdf.h"
#include <QFile>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpPart>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include "DatabaseManager.h"

UploadPdf::UploadPdf(const QString &ID,QWidget *parent, QWidget *topBar) : QWidget(parent) {
    // Set Window Properties
    setStyleSheet("background-color: #0D1B2A;");
    setWindowState(Qt::WindowMaximized);
    uniqueId=ID;
    // Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Top Bar
    mainLayout->addWidget(topBar);

    // Back Button
    QPushButton *backButton = new QPushButton(this);
    backButton->setIcon(QIcon("images/back_arrow.png"));
    backButton->setIconSize(QSize(30, 30));
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(R"(
        QPushButton { background: none; border: none; padding: 5px; }
        QPushButton:hover { background-color: rgba(255, 255, 255, 0.2); border-radius: 5px; }
        QPushButton:pressed { background-color: rgba(255, 255, 255, 0.3); border-radius: 5px; }
    )");
    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit backButtonClicked();
    });
    backButton->setGeometry(10, 70, 40, 40);

    // Functional Window
    QWidget *functionalWindow = new QWidget(this);
    functionalWindow->setStyleSheet("background-color: #283044; border-radius: 20px;");
    functionalWindow->setFixedSize(800, 550);

    QVBoxLayout *functionalLayout = new QVBoxLayout(functionalWindow);
    functionalLayout->setAlignment(Qt::AlignCenter);

    // PDF File Label
    pdfFileLabel = new QLabel("No PDF selected", this);
    pdfFileLabel->setAlignment(Qt::AlignCenter);
    pdfFileLabel->setStyleSheet("color: white; font-size: 14px;");
    pdfFileLabel->setFixedSize(750, 50);
    functionalLayout->addWidget(pdfFileLabel, 0, Qt::AlignCenter);

    // File Selection / Open Button
    fileButton = new QPushButton("Select PDF File", this);
    fileButton->setCursor(Qt::PointingHandCursor);
    fileButton->setStyleSheet(R"(
        QPushButton { padding: 12px; font-size: 16px; background-color: #212735; color: white; border-radius: 20px; }
        QPushButton:hover { background-color: #2E3542; }
        QPushButton:pressed { background-color: #3A4252; }
    )");
    connect(fileButton, &QPushButton::clicked, this, &UploadPdf::selectFile);
    functionalLayout->addWidget(fileButton, 0, Qt::AlignCenter);

    // Add Vertical Space
    functionalLayout->addSpacing(20);
    // Course Code Label
    QLabel *courseLabel = new QLabel("Select Course:", this);
    courseLabel->setStyleSheet("font-weight: bold;color:white; font-size: 20px;");
    courseLabel->setAlignment(Qt::AlignLeft);
    functionalLayout->addWidget(courseLabel, 0, Qt::AlignLeft);

    // Course Code Dropdown
    courseDropdown = new QComboBox(this);
    courseDropdown->setStyleSheet(R"(
    QComboBox { background-color: #212735; color: white; padding: 8px; font-size: 14px; border-radius: 10px; }
    QComboBox QAbstractItemView { background-color: #212735; color: white; selection-background-color: #0078D4; }
)");
    courseDropdown->setFixedSize(750, 40);
    functionalLayout->addWidget(courseDropdown, 0, Qt::AlignCenter);



    // Description Label
    QLabel *descLabel = new QLabel("Enter Description:", this);
    descLabel->setStyleSheet("font-weight: bold;color:white; font-size: 20px;");
    descLabel->setAlignment(Qt::AlignLeft);
    functionalLayout->addWidget(descLabel, 0, Qt::AlignLeft);

    // Description Input Field
    descInput = new QTextEdit(this);
    descInput->setPlaceholderText("Enter description (optional)");
    descInput->setStyleSheet(R"(
        QTextEdit { background-color: #212735; color: white; border-radius: 10px; padding: 8px; font-size: 14px; }
    )");
    descInput->setFixedSize(750, 100);
    functionalLayout->addWidget(descInput, 0, Qt::AlignCenter);

    // Add Vertical Space before Buttons
    functionalLayout->addSpacing(20);

    // Upload & Reset Buttons Layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);

    uploadButton = new QPushButton("Upload PDF", this);
    resetButton = new QPushButton("Reset", this);

    // const QString buttonStyle = R"(
    //     QPushButton { color: white; padding: 15px; border-radius: 20px; }
    //     QPushButton:hover { filter: brightness(1.2); }
    //     QPushButton:pressed { filter: brightness(0.8); }
    // )";

    // uploadButton->setFixedSize(150, 50);
    uploadButton->setStyleSheet(R"(
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
    uploadButton->setVisible(false);  // Initially hidden

    resetButton->setFixedSize(150, 50);
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


    buttonLayout->addWidget(uploadButton);
    buttonLayout->addWidget(resetButton);
    functionalLayout->addLayout(buttonLayout);

    mainLayout->addWidget(functionalWindow, 0, Qt::AlignCenter);

    // Signal Connections
    connect(uploadButton, &QPushButton::clicked, this, &UploadPdf::uploadFile);
    connect(resetButton, &QPushButton::clicked, this, &UploadPdf::resetScreen);
    loadCourses();
}
void UploadPdf::loadCourses(){
    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection is not open!");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT course_code FROM vls_schema.courses WHERE teacher_id = :uniqueId");
    query.bindValue(":uniqueId",uniqueId);
    qDebug()<<"unique Id:" << uniqueId;
    if(!query.exec())
    {
        QMessageBox::critical(this, "Database Error", "Failed to retrieve courses: " + query.lastError().text());
        return;
    };
    while (query.next()) {
        QString courseCode = query.value(0).toString();
        courseDropdown->addItem(courseCode);
    }

}

void UploadPdf::selectFile() {
    filePath = QFileDialog::getOpenFileName(this, "Select PDF File", "", "PDF Files (*.pdf)");

    if (!filePath.isEmpty()) {
        pdfFileLabel->setText("Selected: " + filePath);
        qDebug() << "Selected PDF:" << filePath;

        // Change "Select PDF File" to "Open PDF"
        fileButton->setText("Open PDF");
        disconnect(fileButton, &QPushButton::clicked, this, &UploadPdf::selectFile);
        connect(fileButton, &QPushButton::clicked, this, &UploadPdf::openPDF);

        uploadButton->setVisible(true);  // Show Upload button
    }
}

void UploadPdf::resetScreen() {
    filePath.clear();
    pdfFileLabel->setText("No PDF selected");
    descInput->clear();

    fileButton->setText("Select PDF File");
    disconnect(fileButton, &QPushButton::clicked, this, &UploadPdf::openPDF);
    connect(fileButton, &QPushButton::clicked, this, &UploadPdf::selectFile);

    uploadButton->setVisible(false);  // Hide Upload button again
}

void UploadPdf::uploadFile() {
    if (filePath.isEmpty()) {
        qDebug() << "No file selected for upload!";
        return;
    }

    qDebug() << "Uploading file: " << filePath;
    qDebug() << "Description: " << descInput->toPlainText();

    // Prepare the file for upload
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening file!";
        return;
    }

    // Prepare the network request for file upload
    QNetworkRequest uploadRequest(QUrl("https://xubfmxtcefkzpfjytavd.supabase.co/storage/v1/object/assignments/" + QFileInfo(filePath).fileName()));
    uploadRequest.setRawHeader("Authorization", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inh1YmZteHRjZWZrenBmanl0YXZkIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTc0MzY3NjI0NSwiZXhwIjoyMDU5MjUyMjQ1fQ.ujizd3now55PzNIbkEeH9xR3LOoz1JxWaJlUrrAnOTs");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->put(uploadRequest, file.readAll());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "File upload failed:" << reply->errorString();
            QMessageBox::warning(this, "Upload Failed", "File upload failed!");
        } else {
            qDebug() << "File uploaded successfully!";
            QString fileUrl = "https://xubfmxtcefkzpfjytavd.supabase.co/storage/v1/object/public/assignments/" + QFileInfo(filePath).fileName();
            insertFileMetadata(fileUrl);
        }
        reply->deleteLater();
    });
}

void UploadPdf::insertFileMetadata(const QString &fileUrl) {
    // Prepare metadata for insertion into the Supabase database
    QJsonObject json;
    json["teacher_id"] = uniqueId;  // Replace with actual teacher ID
    json["file_name"] = QFileInfo(filePath).fileName();
    json["file_url"] = fileUrl;
    json["file_description"] = descInput->toPlainText();
    json["file_type"] = "pdf";
    json["course_code"]=courseDropdown->currentText();
    // Create the database request
    QNetworkRequest dbRequest(QUrl("https://xubfmxtcefkzpfjytavd.supabase.co/rest/v1/files"));
    dbRequest.setRawHeader("Authorization", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inh1YmZteHRjZWZrenBmanl0YXZkIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTc0MzY3NjI0NSwiZXhwIjoyMDU5MjUyMjQ1fQ.ujizd3now55PzNIbkEeH9xR3LOoz1JxWaJlUrrAnOTs");
    dbRequest.setRawHeader("apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inh1YmZteHRjZWZrenBmanl0YXZkIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTc0MzY3NjI0NSwiZXhwIjoyMDU5MjUyMjQ1fQ.ujizd3now55PzNIbkEeH9xR3LOoz1JxWaJlUrrAnOTs");
    dbRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(dbRequest, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error inserting metadata:" << reply->errorString();
            QMessageBox::warning(this, "Metadata Insertion Failed", "Failed to insert metadata into the database.");
        } else {
            qDebug() << "File metadata inserted successfully!";
            QMessageBox::information(this, "Upload", "File and metadata uploaded successfully!");
        }
        reply->deleteLater();
    });
}

void UploadPdf::openPDF() {
    if (filePath.isEmpty()) {
        qDebug() << "No file selected!";
        return;
    }

    qDebug() << "Opening PDF:" << filePath;
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}
