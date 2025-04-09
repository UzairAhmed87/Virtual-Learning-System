#include "uploadlecture.h"
#include <QJsonDocument>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include "DatabaseManager.h"

UploadLecture::UploadLecture(const QString &Id,QWidget *parent,QWidget *topBar) : QWidget(parent) {
    setStyleSheet("background-color: #0D1B2A;");
    setWindowState(Qt::WindowMaximized);
    uniqueId=Id;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Top Bar
    // topBar = new TopBar(this);
    mainLayout->addWidget(topBar);


    // Functional Window
    functionalWindow = new QWidget(this);
    functionalWindow->setStyleSheet("background-color: #283044; border-radius: 20px;");
    functionalWindow->setFixedSize(800, 550);

    QVBoxLayout *functionalLayout = new QVBoxLayout(functionalWindow);
    functionalLayout->setAlignment(Qt::AlignCenter);

    // Video Preview Section
    mediaPlayer = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    mediaPlayer->setVideoOutput(videoWidget);
    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    videoWidget->setFixedSize(500, 240);

    // Video Click to Toggle Play/Pause
    videoWidget->installEventFilter(this);

    // Play/Pause Button
    pauseButton = new QPushButton("Play", this);
    pauseButton->setCursor(Qt::PointingHandCursor);
    pauseButton->setStyleSheet(
        "QPushButton { padding: 8px; font-size: 14px; background-color: #212735; color: white; border-radius: 10px; }"
        "QPushButton:hover { background-color: #2E3542; }"
        "QPushButton:pressed { background-color: #3A4252; }"
        );
    pauseButton->setFixedSize(100, 30);
    connect(pauseButton, &QPushButton::clicked, this, &UploadLecture::togglePlayback);

    // Back Button (Independent, Positioned Manually)
    QPushButton *backButton = new QPushButton(this);
    backButton->setIcon(QIcon("images/back_arrow.png"));
    backButton->setIconSize(QSize(30, 30));
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton { background: none; border: none; padding: 5px; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.2); border-radius: 5px; }"
        "QPushButton:pressed { background-color: rgba(255, 255, 255, 0.3); border-radius: 5px; }"
        );
    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit backButtonClicked();
    });

    // Set Position (Beneath Logo)
    backButton->setGeometry(10, 70, 40, 40); // Adjust (x, y, width, height)

    // Video Container
    QWidget *videoContainer = new QWidget(this);
    // videoContainer->setStyleSheet("background-color: #444444; border: 2px solid red;");
    // videoContainer->setFixedSize(500, 240);
    QVBoxLayout *videoContainerLayout = new QVBoxLayout(videoContainer);
    videoContainerLayout->addWidget(videoWidget, 0, Qt::AlignCenter);
    videoContainerLayout->addWidget(pauseButton, 0, Qt::AlignCenter);
    videoContainerLayout->setContentsMargins(0, 0, 0, 0);
    videoContainer->setLayout(videoContainerLayout);


    // placeholderLabel = new QLabel(this);
    // QPixmap originalPixmap("../images/Upload.png");

    // // Don't set fixed size for the label, let it adapt to the image size
    // placeholderLabel->setPixmap(originalPixmap);
    // placeholderLabel->setAlignment(Qt::AlignCenter);

    // Create a container widget for the placeholder with layout
    QWidget* placeholderContainer = new QWidget(this);
    QVBoxLayout* placeholderLayout = new QVBoxLayout(placeholderContainer);
    placeholderLayout->setContentsMargins(0, 0, 0, 0);
    placeholderLayout->addWidget(placeholderLabel, 0, Qt::AlignCenter);
    placeholderContainer->setLayout(placeholderLayout);

    // Make the container the same size as the video container
    placeholderContainer->setFixedSize(500, 240);

    // Add placeholder and video container to preview stack
    previewStack = new QStackedWidget(this);
    previewStack->addWidget(placeholderContainer);
    previewStack->addWidget(videoContainer);
    previewStack->setCurrentIndex(0);

    // Add preview stack to layout
    functionalLayout->addWidget(previewStack, 0, Qt::AlignCenter);


    // File Upload Button
    selectButton = new QPushButton("Select MP4 File", this);
    selectButton->setCursor(Qt::PointingHandCursor);
    selectButton->setStyleSheet(
        "QPushButton { padding: 12px; font-size: 16px; background-color: #212735; color: white; border-radius: 20px; }"
        "QPushButton:hover { background-color: #2E3542; }"
        "QPushButton:pressed { background-color: #3A4252; }"
        );
    connect(selectButton, &QPushButton::clicked, this, &UploadLecture::selectFile);
    functionalLayout->addWidget(selectButton, 0, Qt::AlignCenter);

    // Input Fields
    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->setAlignment(Qt::AlignLeft);
    formLayout->setContentsMargins(10, 0, 10, 0);

    QLabel *titleLabel = new QLabel("Lecture Title:", this);
    titleLabel->setStyleSheet("font-weight: bold;color:white; font-size: 20px;");
    formLayout->addWidget(titleLabel);

    titleInput = new QLineEdit(this);
    titleInput->setPlaceholderText("Enter Lecture Title");
    titleInput->setFixedHeight(30);
    titleInput->setStyleSheet("background-color: #212735; color: white; padding: 8px; font-size: 14px; border-radius: 10px;");
    formLayout->addWidget(titleInput);
    courseDropdown = new QComboBox(this);
    courseDropdown->setStyleSheet(R"(
    QComboBox { background-color: #212735; color: white; padding: 8px; font-size: 14px; border-radius: 10px; }
    QComboBox QAbstractItemView { background-color: #212735; color: white; selection-background-color: #0078D4; }
)");
    QLabel *courseLabel = new QLabel("Select Course:",this);
    courseLabel->setStyleSheet("font-weight: bold;color:white; font-size: 20px;");
    formLayout->addWidget(courseLabel);
    courseDropdown->setFixedHeight(30);
    formLayout->addWidget(courseDropdown);
    QLabel *descriptionLabel = new QLabel("Description:", this);
    descriptionLabel->setStyleSheet("font-weight: bold;color:white; font-size: 20px;");
    formLayout->addWidget(descriptionLabel);

    descriptionInput = new QTextEdit(this);
    descriptionInput->setPlaceholderText("Enter Lecture Description");
    descriptionInput->setFixedHeight(80);
    descriptionInput->setStyleSheet(R"(
        QTextEdit { background-color: #212735; color: white; border-radius: 10px; padding: 8px; font-size: 14px; }
    )");
    formLayout->addWidget(descriptionInput);
    QWidget *formWidget = new QWidget(this);
    formWidget->setLayout(formLayout);
    functionalLayout->addWidget(formWidget);

    // functionalLayout->addLayout(formLayout);

    // Upload & Cancel Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);

    uploadButton = new QPushButton("Upload", this);
    cancelButton = new QPushButton("Reset", this);

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
    uploadButton->setVisible(false);
    // cancelButton->setFixedSize(150, 50);
    cancelButton->setStyleSheet(R"(
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
    buttonLayout->addWidget(cancelButton);
    functionalLayout->addLayout(buttonLayout);
    mainLayout->addWidget(functionalWindow, 0, Qt::AlignCenter);

    connect(uploadButton, &QPushButton::clicked, this, &UploadLecture::startUpload);
    connect(cancelButton, &QPushButton::clicked, this, &UploadLecture::resetScreen);

    mediaPlayer->pause();
    loadCourses();
}
void UploadLecture::loadCourses(){
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
void UploadLecture::selectFile() {
    filePath = QFileDialog::getOpenFileName(this, "Select MP4 File", "", "Videos (*.mp4)");
    if (!filePath.isEmpty()) {
        mediaPlayer->setSource(QUrl::fromLocalFile(filePath));
        mediaPlayer->pause();

        selectButton->setVisible(false);
        previewStack->setCurrentIndex(1);
        uploadButton->setVisible(true);
    }
}

void UploadLecture::resetScreen() {
    filePath.clear();
    mediaPlayer->stop();

    selectButton->setVisible(true);
    previewStack->setCurrentIndex(0);

    titleInput->clear();
    descriptionInput->clear();
    uploadButton->setVisible(false);
}

void UploadLecture::startUpload() {
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file selected!");
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Failed to open the file.");
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QString fileName = QFileInfo(filePath).fileName();
    QString uploadUrl = "https://xubfmxtcefkzpfjytavd.supabase.co/storage/v1/object/lectures/" + fileName;

    QNetworkRequest uploadRequest{QUrl(uploadUrl)};
    uploadRequest.setRawHeader("Authorization", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inh1YmZteHRjZWZrenBmanl0YXZkIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTc0MzY3NjI0NSwiZXhwIjoyMDU5MjUyMjQ1fQ.ujizd3now55PzNIbkEeH9xR3LOoz1JxWaJlUrrAnOTs");
    // uploadRequest.setRawHeader("Content-Type", "video/mp4");

    networkManager = new QNetworkAccessManager(this);
    QNetworkReply *uploadReply = networkManager->put(uploadRequest, fileData);
    connect(uploadReply, &QNetworkReply::finished, this, &UploadLecture::handleFileUploadReply);
}
void UploadLecture::handleFileUploadReply() {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Upload Failed", "Video file upload failed.");
        qDebug() << "File Upload Error: " << reply->errorString();
    } else {
        qDebug() << "Video uploaded. Proceeding to metadata...";

        // Now send metadata
        QJsonObject json;
        json["teacher_id"] = uniqueId;
        json["file_name"] = titleInput->text();
        json["file_url"] = "https://xubfmxtcefkzpfjytavd.supabase.co/storage/v1/object/public/lectures/" + QFileInfo(filePath).fileName();
        json["file_description"] = descriptionInput->toPlainText();
        json["file_type"] = "video";
        json["course_code"] = courseDropdown->currentText();

        QNetworkRequest request(QUrl("https://xubfmxtcefkzpfjytavd.supabase.co/rest/v1/files"));
        request.setRawHeader("Authorization", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inh1YmZteHRjZWZrenBmanl0YXZkIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTc0MzY3NjI0NSwiZXhwIjoyMDU5MjUyMjQ1fQ.ujizd3now55PzNIbkEeH9xR3LOoz1JxWaJlUrrAnOTs");
        request.setRawHeader("apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inh1YmZteHRjZWZrenBmanl0YXZkIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTc0MzY3NjI0NSwiZXhwIjoyMDU5MjUyMjQ1fQ.ujizd3now55PzNIbkEeH9xR3LOoz1JxWaJlUrrAnOTs");
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *metaReply = networkManager->post(request, QJsonDocument(json).toJson());
        connect(metaReply, &QNetworkReply::finished, this, &UploadLecture::handleUploadReply);
    }

    reply->deleteLater();
}


void UploadLecture::handleUploadReply() {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::warning(this, "Upload Failed", "Failed to upload metadata.");
        qDebug() << "Upload error: " << reply->errorString();
    } else {
        QMessageBox::information(this, "Success", "Lecture uploaded successfully!");
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
