#include "uploadlecture.h"
#include <QDebug>

UploadLecture::UploadLecture(const QString &Id, QWidget *parent, QWidget *topBar) : QWidget(parent) {
    uniqueId = Id;
    qDebug() << "unique id:" << uniqueId;
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
    functionalWindow = new QWidget(this);
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
    QLabel *titleLabel = new QLabel("Upload New Lecture", functionalWindow);
    titleLabel->setStyleSheet(
        "font-size: 28px; font-weight: bold; color: white; background:transparent;"
        "margin-bottom: 10px;"
        );
    titleLabel->setAlignment(Qt::AlignCenter);
    functionalLayout->addWidget(titleLabel, 0, Qt::AlignHCenter);

    // Video Preview Section
    QWidget *previewContainer = new QWidget(this);
    previewContainer->setObjectName("previewContainer");
    previewContainer->setFixedSize(400, 300);
    previewContainer->setStyleSheet(
        "QWidget#previewContainer {"
        "   background-color: rgba(15, 23, 42, 0.5);"
        "   border-radius: 20px;"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "}"
        );

    QVBoxLayout *previewLayout = new QVBoxLayout(previewContainer);
    previewLayout->setContentsMargins(20, 20, 20, 20);
    previewLayout->setSpacing(15);

    // Video Widget
    mediaPlayer = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    mediaPlayer->setVideoOutput(videoWidget);
    videoWidget->setFixedSize(360, 200);
    videoWidget->setStyleSheet("border-radius: 16px; background-color: #0F172A;");
    videoWidget->installEventFilter(this);

    // Play/Pause Button
    pauseButton = new QPushButton("Play", this);
    pauseButton->setCursor(Qt::PointingHandCursor);
    pauseButton->setStyleSheet(
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
    pauseButton->setFixedSize(120, 36);
    pauseButton->setVisible(false);
    connect(pauseButton, &QPushButton::clicked, this, &UploadLecture::togglePlayback);

    // Placeholder
    placeholderLabel = new QLabel(this);
    placeholderLabel->setFixedSize(360, 200);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    placeholderLabel->setStyleSheet(
        "background-color: rgba(15, 23, 42, 0.5);"
        "border-radius: 16px;"
        "border: 2px dashed rgba(255, 255, 255, 0.2);"
        "color: rgba(255, 255, 255, 0.7);"
        "font-size: 18px;"
        );

    QVBoxLayout *placeholderLayout = new QVBoxLayout(placeholderLabel);
    placeholderLayout->setAlignment(Qt::AlignCenter);
    placeholderLayout->setSpacing(10);

    QLabel *uploadText = new QLabel("Click 'Select MP4 File' to upload a lecture video", placeholderLabel);
    uploadText->setStyleSheet("font-size: 16px; color: rgba(255, 255, 255, 0.7); border: none; background: none;");
    placeholderLayout->addWidget(uploadText, 0, Qt::AlignCenter);

    // Stacked widget for switching between placeholder and video
    previewStack = new QStackedWidget(this);
    previewStack->addWidget(placeholderLabel);
    previewStack->addWidget(videoWidget);
    previewStack->setCurrentIndex(0);

    previewLayout->addWidget(previewStack, 0, Qt::AlignCenter);
    previewLayout->addWidget(pauseButton, 0, Qt::AlignCenter);

    functionalLayout->addWidget(previewContainer, 0, Qt::AlignCenter);

    // File Upload Button
    selectButton = new QPushButton("Select MP4 File", this);
    selectButton->setCursor(Qt::PointingHandCursor);
    selectButton->setStyleSheet(
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
    selectButton->setFixedWidth(200);
    QHBoxLayout *selectButtonLayout = new QHBoxLayout();
    selectButtonLayout->setAlignment(Qt::AlignCenter);
    selectButtonLayout->addWidget(selectButton);
    functionalLayout->addLayout(selectButtonLayout);

    // Input Fields
    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->setContentsMargins(40, 0, 40, 0);
    formLayout->setSpacing(15);

    QLabel *lectureTitleLabel = new QLabel("Lecture Title", this);
    lectureTitleLabel->setStyleSheet("font-weight: bold; color: white; font-size: 16px;background:transparent");
    formLayout->addWidget(lectureTitleLabel);

    titleInput = new QLineEdit(this);
    titleInput->setPlaceholderText("Enter Lecture Title");
    titleInput->setFixedHeight(45);
    titleInput->setStyleSheet(
        "QLineEdit {"
        "   background-color: rgba(15, 23, 42, 0.5);"
        "   color: white;"
        "   padding: 12px 16px;"
        "   font-size: 15px;"
        "   border-radius: 10px;"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "}"
        "QLineEdit:focus {"
        "   border: 1px solid #3B82F6;"
        "}"
        );
    formLayout->addWidget(titleInput);

    QLabel *courseLabel = new QLabel("Select Course", this);
    courseLabel->setStyleSheet("font-weight: bold; color: white; font-size: 16px;background:transparent");
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

    // Upload & Cancel Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(20);

    uploadButton = new QPushButton("Upload Lecture", this);
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

    cancelButton = new QPushButton("Reset", this);
    cancelButton->setStyleSheet(
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
    cancelButton->setFixedSize(180, 45);

    buttonLayout->addWidget(uploadButton);
    buttonLayout->addWidget(cancelButton);
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
    connect(selectButton, &QPushButton::clicked, this, &UploadLecture::selectFile);
    connect(uploadButton, &QPushButton::clicked, this, &UploadLecture::startUpload);
    connect(cancelButton, &QPushButton::clicked, this, &UploadLecture::resetScreen);

    mediaPlayer->pause();
    loadCourses();
}

void UploadLecture::loadCourses() {
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

void UploadLecture::selectFile() {
    filePath = QFileDialog::getOpenFileName(this, "Select MP4 File", "", "Videos (*.mp4)");
    if (!filePath.isEmpty()) {
        mediaPlayer->setSource(QUrl::fromLocalFile(filePath));
        mediaPlayer->pause();

        selectButton->setVisible(false);
        previewStack->setCurrentIndex(1);
        uploadButton->setVisible(true);
        pauseButton->setVisible(true);
    }
}

void UploadLecture::resetScreen() {
    filePath.clear();
    mediaPlayer->stop();

    selectButton->setVisible(true);
    previewStack->setCurrentIndex(0);
    pauseButton->setVisible(false);

    titleInput->clear();
    uploadButton->setVisible(false);
}

void UploadLecture::startUpload() {
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
    QString uploadUrl = SupabaseConfig::getLectureStorageUrl() + fileName;

    QNetworkRequest uploadRequest{QUrl(uploadUrl)};
    uploadRequest.setRawHeader("Authorization", SupabaseConfig::getServiceRoleKey().toUtf8());

    networkManager = new QNetworkAccessManager(this);
    QNetworkReply *uploadReply = networkManager->put(uploadRequest, fileData);
    connect(uploadReply, &QNetworkReply::finished, this, &UploadLecture::handleFileUploadReply);
}

void UploadLecture::handleFileUploadReply() {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Video file upload failed.",
            "Upload Failed",
            "OK"
            );
        qDebug() << "File Upload Error: " << reply->errorString();
    } else {
        qDebug() << "Video uploaded. Proceeding to metadata...";

        // Send metadata
        QJsonObject json;
        json["teacher_id"] = uniqueId;
        json["file_name"] = titleInput->text();
        json["file_url"] = SupabaseConfig::getPublicLectureUrl() + QFileInfo(filePath).fileName();
        json["file_type"] = "video";
        json["course_code"] = courseDropdown->currentText();

        QNetworkRequest request(QUrl(SupabaseConfig::getFilesApiUrl()));
        request.setRawHeader("Authorization", SupabaseConfig::getServiceRoleKey().toUtf8());
        request.setRawHeader("apikey", SupabaseConfig::getApiKey().toUtf8());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *metaReply = networkManager->post(request, QJsonDocument(json).toJson());
        connect(metaReply, &QNetworkReply::finished, this, &UploadLecture::handleUploadReply);
    }

    reply->deleteLater();
}

void UploadLecture::handleUploadReply() {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply->error() != QNetworkReply::NoError) {
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
            "Lecture uploaded successfully!",
            "Success",
            "OK"
            );
        qDebug() << "Upload successful!";
    }

    reply->deleteLater();
}

void UploadLecture::togglePlayback() {
    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
        pauseButton->setText("Play");
    } else {
        mediaPlayer->play();
        pauseButton->setText("Pause");
    }
}

bool UploadLecture::eventFilter(QObject *obj, QEvent *event) {
    if (obj == videoWidget && event->type() == QEvent::MouseButtonPress) {
        togglePlayback();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}
