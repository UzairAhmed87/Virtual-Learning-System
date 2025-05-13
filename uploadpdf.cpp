#include "uploadpdf.h"
#include "supabaseconfig.h"
#include <QDateTimeEdit>
#include <QTextEdit>

UploadPdf::UploadPdf(const QString &ID, QWidget *parent, QWidget *topBar) : QWidget(parent) {
    uniqueId = ID;
    setWindowState(Qt::WindowMaximized);

    // Main background with gradient
    setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #0F172A, stop:1 #1E293B);");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Top Bar
    QWidget *topBarPlaceholder = new QWidget();
    topBarPlaceholder->setFixedHeight(topBar->height());
    mainLayout->addWidget(topBarPlaceholder, 0, Qt::AlignTop);

    // Functional Window
   QWidget *functionalWindow = new QWidget(this);
    functionalWindow->setMinimumSize(800, 600);
    functionalWindow->setMaximumWidth(1000);
    functionalWindow->setObjectName("functionalWindow");
    functionalWindow->setStyleSheet(
        "QWidget#functionalWindow {"
        "   background-color: rgba(30, 41, 59, 0.8);"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "   border-radius: 24px;"
        "   padding: 20px;"
        "}"
        );

    QVBoxLayout *functionalLayout = new QVBoxLayout(functionalWindow);
    functionalLayout->setContentsMargins(30, 20, 30, 20);
    functionalLayout->setSpacing(25);

    // Title Label
    QLabel *titleLabel = new QLabel("Upload Assignment", functionalWindow);
    titleLabel->setStyleSheet(
        "font-size: 28px; font-weight: bold; color: white; background:transparent;"
        "margin-bottom: 10px;"
        );
    titleLabel->setAlignment(Qt::AlignCenter);
    functionalLayout->addWidget(titleLabel, 0, Qt::AlignHCenter);

    // PDF Preview Section
    QWidget *pdfContainer = new QWidget(this);
    pdfContainer->setObjectName("pdfContainer");
    pdfContainer->setStyleSheet(
        "QWidget#pdfContainer {"
        "   background-color: rgba(15, 23, 42, 0.5);"
        "   border-radius: 20px;"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "}"
        );

    QVBoxLayout *pdfContainerLayout = new QVBoxLayout(pdfContainer);
    pdfContainerLayout->setContentsMargins(20, 20, 20, 20);

    // PDF File Label (reduced size)
    pdfFileLabel = new QLabel("No PDF selected", this);
    pdfFileLabel->setAlignment(Qt::AlignCenter);
    pdfFileLabel->setStyleSheet(
        "QLabel {"
        "   color: rgba(255, 255, 255, 0.7);"
        "   font-size: 16px;"
        "   padding: 15px;"
        "   background-color: rgba(15, 23, 42, 0.5);"
        "   border-radius: 16px;"
        "   border: 2px dashed rgba(255, 255, 255, 0.2);"
        "}"
        );
    pdfFileLabel->setFixedSize(300, 150); // Reduced size
    pdfContainerLayout->addWidget(pdfFileLabel, 0, Qt::AlignCenter);

    functionalLayout->addWidget(pdfContainer);

    // File Selection Button
    fileButton = new QPushButton("Select PDF File", this);
    fileButton->setCursor(Qt::PointingHandCursor);
    fileButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #3B82F6;"
        "   color: white;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   padding: 12px 20px;"
        "   border-radius: 10px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #60A5FA;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2563EB;"
        "}"
        );
    fileButton->setFixedWidth(200);
    QHBoxLayout *selectButtonLayout = new QHBoxLayout();
    selectButtonLayout->setAlignment(Qt::AlignCenter);
    selectButtonLayout->addWidget(fileButton);
    functionalLayout->addLayout(selectButtonLayout);

    // Input Fields
    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->setContentsMargins(40, 0, 40, 0);
    formLayout->setSpacing(15);

    // Description Input
    QLabel *descriptionLabel = new QLabel("Description", this);
    descriptionLabel->setStyleSheet("font-weight: bold; color: white; font-size: 16px;background:transparent;");
    formLayout->addWidget(descriptionLabel);

    descriptionInput = new QTextEdit(this);
    descriptionInput->setPlaceholderText("Enter description");
    descriptionInput->setFixedHeight(50);
    descriptionInput->setStyleSheet(
        "QTextEdit {"
        "   background-color: rgba(15, 23, 42, 0.5);"
        "   color: white;"
        "   padding: 12px 16px;"
        "   font-size: 15px;"
        "   border-radius: 10px;"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "}"
        "QTextEdit:focus {"
        "   border: 1px solid #3B82F6;"
        "}"
        );
    formLayout->addWidget(descriptionInput);

    // Deadline Input
    QLabel *deadlineLabel = new QLabel("Deadline", this);
    deadlineLabel->setStyleSheet("font-weight: bold; color: white; font-size: 16px;background:transparent");
    formLayout->addWidget(deadlineLabel);

    QHBoxLayout *deadlineLayout = new QHBoxLayout();
    deadlineLayout->setSpacing(10);

    deadlineInput = new QDateTimeEdit(this);
    deadlineInput->setCalendarPopup(true); // Enable calendar popup
    deadlineInput->setDisplayFormat("yyyy-MM-dd hh:mm AP"); // Format with AM/PM
    deadlineInput->setDateTime(QDateTime::currentDateTime()); // Default to current date/time
    deadlineInput->setFixedHeight(45);
    deadlineInput->setStyleSheet(
        "QDateTimeEdit {"
        "   background-color: rgba(15, 23, 42, 0.5);"
        "   color: white;"
        "   padding: 12px 16px;"
        "   font-size: 15px;"
        "   border-radius: 10px;"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "}"
        "QDateTimeEdit:focus {"
        "   border: 1px solid #3B82F6;"
        "}"
        "QDateTimeEdit::drop-down {"
        "   subcontrol-origin: padding;"
        "   subcontrol-position: center right;"
        "color:white;"
        "   width: 30px;"
        "   border-left: none;"
        "}"
        );
    deadlineLayout->addWidget(deadlineInput);

    formLayout->addLayout(deadlineLayout);

    // Course Selection
    QLabel *courseLabel = new QLabel("Select Course", this);
    courseLabel->setStyleSheet("font-weight: bold; color: white; font-size: 16px;background:transparent;");
    formLayout->addWidget(courseLabel);

    courseDropdown = new QComboBox(this);
    courseDropdown->setFixedHeight(45);
    courseDropdown->setStyleSheet(
        "QComboBox {"
        "   background-color: rgba(15, 23, 42, 0.5);"
        "   color: white;"
        "   padding: 12px 16px;"
        "   font-size: 15px;"
        "   border-radius: 10px;"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "}"
        "QComboBox:focus, QComboBox:on {"
        "   border: 1px solid #3B82F6;"
        "}"
        "QComboBox::drop-down {"
        "   subcontrol-origin: padding;"
        "   subcontrol-position: center right;"
        "   width: 30px;"
        "   border-left: none;"
        "}"
        "QComboBox QAbstractItemView {"
        "   background-color: #1E293B;"
        "   color: white;"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "   border-radius: 8px;"
        "   selection-background-color: #3B82F6;"
        "}"
        );
    formLayout->addWidget(courseDropdown);

    functionalLayout->addLayout(formLayout);

    // Upload & Reset Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(20);

    uploadButton = new QPushButton("Upload PDF", this);
    uploadButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #3B82F6;"
        "   color: white;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   padding: 12px 20px;"
        "   border-radius: 10px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #60A5FA;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2563EB;"
        "}"
        );
    uploadButton->setFixedSize(180, 45);
    uploadButton->setVisible(false);

    resetButton = new QPushButton("Reset", this);
    resetButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #EF4444;"
        "   color: white;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   padding: 12px 20px;"
        "   border-radius: 10px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #F87171;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #DC2626;"
        "}"
        );
    resetButton->setFixedSize(180, 45);

    buttonLayout->addWidget(uploadButton);
    buttonLayout->addWidget(resetButton);
    functionalLayout->addLayout(buttonLayout);

    mainLayout->addWidget(functionalWindow, 0, Qt::AlignCenter);

    // Back Button
    QPushButton *backButton = new QPushButton(this);
    backButton->setIcon(QIcon("images/back_arrow.png"));
    backButton->setIconSize(QSize(32, 32));
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(30, 41, 59, 0.7);"
        "   border: none;"
        "   border-radius: 20px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(59, 130, 246, 0.7);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(37, 99, 235, 0.7);"
        "}"
        );
    backButton->setFixedSize(40, 40);
    backButton->setGeometry(20, 80, 40, 40);
    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit backButtonClicked();
    });

    // Connect signals
    connect(fileButton, &QPushButton::clicked, this, &UploadPdf::selectFile);
    connect(uploadButton, &QPushButton::clicked, this, &UploadPdf::uploadFile);
    connect(resetButton, &QPushButton::clicked, this, &UploadPdf::resetScreen);

    loadCourses();
}

void UploadPdf::loadCourses() {
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
    QSqlQuery query;
    query.prepare("SELECT course_code FROM vls_schema.courses WHERE teacher_id = :uniqueId");
    query.bindValue(":uniqueId", uniqueId);
    qDebug() << "unique Id:" << uniqueId;
    if (!query.exec()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Failed to retrieve courses: " + query.lastError().text(),
            "Database Error",
            "OK"
            );
        return;
    }
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

        fileButton->setText("Open PDF");
        disconnect(fileButton, &QPushButton::clicked, this, &UploadPdf::selectFile);
        connect(fileButton, &QPushButton::clicked, this, &UploadPdf::openPDF);

        uploadButton->setVisible(true);
    }
}

void UploadPdf::resetScreen() {
    filePath.clear();
    pdfFileLabel->setText("No PDF selected");
    descriptionInput->clear();
    deadlineInput->setDateTime(QDateTime::currentDateTime());

    fileButton->setText("Select PDF File");
    disconnect(fileButton, &QPushButton::clicked, this, &UploadPdf::openPDF);
    connect(fileButton, &QPushButton::clicked, this, &UploadPdf::selectFile);

    uploadButton->setVisible(false);
}

void UploadPdf::uploadFile() {
    if (filePath.isEmpty()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "No file selected!",
            "Error",
            "OK"
            );
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Failed to open the file.",
            "Error",
            "OK"
            );
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QString fileName = QFileInfo(filePath).fileName();
    QString uploadUrl = SupabaseConfig::getAssignmentStorageUrl() + fileName;

    QNetworkRequest uploadRequest{QUrl(uploadUrl)};
    uploadRequest.setRawHeader("Authorization", SupabaseConfig::getServiceRoleKey().toUtf8());

    networkManager = new QNetworkAccessManager(this);
    QNetworkReply *uploadReply = networkManager->put(uploadRequest, fileData);
    connect(uploadReply, &QNetworkReply::finished, this, [=]() {
        if (uploadReply->error() != QNetworkReply::NoError) {
            MessageBoxUtil::showCustomMessage(
                this,
                "PDF file upload failed.",
                "Upload Failed",
                "OK"
                );
            qDebug() << "File Upload Error: " << uploadReply->errorString();
        } else {
            qDebug() << "PDF uploaded. Proceeding to metadata...";
            QString fileUrl = SupabaseConfig::getPublicLectureUrl() + fileName;
            insertFileMetadata(fileUrl);
        }
        uploadReply->deleteLater();
    });
}

void UploadPdf::insertFileMetadata(const QString &fileUrl) {
    QJsonObject json;
    json["teacher_id"] = uniqueId;
    json["file_description"] = descriptionInput->toPlainText();
    json["deadline"] = deadlineInput->dateTime().toString(Qt::ISODate);
    json["file_url"] = fileUrl;
    json["file_type"] = "pdf";
    json["course_code"] = courseDropdown->currentText();
    qDebug() << "FULL JSON:" << QJsonDocument(json).toJson(QJsonDocument::Indented);
    qDebug()<< "deadline"<<deadlineInput->dateTime().toString(Qt::ISODate);
    QNetworkRequest request((QUrl(SupabaseConfig::getFilesApiUrl())));
    request.setRawHeader("Authorization", SupabaseConfig::getServiceRoleKey().toUtf8());
    request.setRawHeader("apikey", SupabaseConfig::getApiKey().toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Supabase response:" << reply->readAll();
            MessageBoxUtil::showCustomMessage(
                this,
                "Failed to upload metadata.",
                "Upload Failed",
                "OK"
                );
            qDebug() << "Upload error: " << reply->errorString();
        } else {
            MessageBoxUtil::showCustomMessage(
                this,
                "PDF uploaded successfully!",
                "Success",
                "OK"
                );
            qDebug() << "Upload successful!";
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
