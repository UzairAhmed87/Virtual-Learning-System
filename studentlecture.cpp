#include "studentlecture.h"
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QNetworkRequest>
#include "mesgboxutil.h"

StudentLecture::StudentLecture(QWidget *parent) : QWidget(parent) {
    // Initialize network manager
    networkManager = new QNetworkAccessManager(this);
    this->setMinimumSize(800, 700);
    // Set Window Properties
    setStyleSheet("background-color: #0D1B2A;");
    // setWindowState(Qt::WindowMaximized);

    // Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Top Bar
    // topBar = new TopBar(this);
    // mainLayout->addWidget(topBar);

    // Functional Window
    QWidget *functionalWindow = new QWidget(this);
    functionalWindow->setStyleSheet("background-color: #283044; border-radius: 20px;");
    functionalWindow->setFixedSize(1000, 700);

    QVBoxLayout *functionalLayout = new QVBoxLayout(functionalWindow);
    functionalLayout->setAlignment(Qt::AlignCenter);

    // Lecture Details
    assignmentDetails = new QLabel("Lecture Details will appear here", this);
    assignmentDetails->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    assignmentDetails->setAlignment(Qt::AlignCenter);
    assignmentDetails->setFixedSize(950, 50);
    functionalLayout->addWidget(assignmentDetails, 0, Qt::AlignCenter);

    // Add Vertical Space
    functionalLayout->addSpacing(20);

    // Video Preview
    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    player->setVideoOutput(videoWidget);
    videoWidget->setFixedSize(950, 300);
    videoWidget->setStyleSheet("background-color: black; border-radius: 10px;");
    functionalLayout->addWidget(videoWidget, 0, Qt::AlignCenter);

    // Add Vertical Space
    functionalLayout->addSpacing(20);

    // Lecture Description
    lectureDescription = new QTextBrowser(this);
    lectureDescription->setText("Lecture description will appear here");
    lectureDescription->setStyleSheet(R"(
        QTextBrowser { background-color: #212735; color: white; border-radius: 10px; padding: 8px; font-size: 14px; }
    )");
    lectureDescription->setFixedSize(950, 100);
    lectureDescription->setReadOnly(true);
    functionalLayout->addWidget(lectureDescription, 0, Qt::AlignCenter);

    // Add Vertical Space
    functionalLayout->addSpacing(20);

    // Buttons Layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);

    // Download Button
    downloadButton = new QPushButton("Download Lecture", this);
    downloadButton->setCursor(Qt::PointingHandCursor);
    downloadButton->setStyleSheet(R"(
        QPushButton { padding: 12px; font-size: 16px; background-color: #212735; color: white; border-radius: 20px; }
        QPushButton:hover { background-color: #2E3542; }
        QPushButton:pressed { background-color: #3A4252; }
    )");
    connect(downloadButton, &QPushButton::clicked, this, &StudentLecture::downloadLecture);
    buttonLayout->addWidget(downloadButton);

    // Play Button
    playButton = new QPushButton("Play Lecture", this);
    playButton->setCursor(Qt::PointingHandCursor);
    playButton->setStyleSheet(R"(
        QPushButton { padding: 12px; font-size: 16px; background-color: #0078D4; color: white; border-radius: 20px; }
        QPushButton:hover { background-color: #0067B8; }
        QPushButton:pressed { background-color: #0056A3; }
    )");
    connect(playButton, &QPushButton::clicked, this, &StudentLecture::playLecture);
    buttonLayout->addWidget(playButton);

    functionalLayout->addLayout(buttonLayout);
    mainLayout->addWidget(functionalWindow, 0, Qt::AlignCenter);
}

void StudentLecture::setLectureInfo(const QString& title, const QString& description, const QString& videoUrl) {
    this->videoUrl = videoUrl;
    assignmentDetails->setText(QString("Lecture: %1").arg(title));
    lectureDescription->setText(description);

    // Set up the video preview
    player->setSource(QUrl(videoUrl));
    player->play();
}

void StudentLecture::downloadLecture() {
    if (videoUrl.isEmpty()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "No lecture URL available!",
            "Download Error",
            "OK"
            );
        return;
    }

    QString defaultFileName = QUrl(videoUrl).fileName();
    if (defaultFileName.isEmpty()) {
        defaultFileName = "lecture.mp4";
    }

    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString savePath = QFileDialog::getSaveFileName(
        this,
        "Save Lecture Video",
        downloadPath + "/" + defaultFileName,
        "Video Files (*.mp4)"
        );

    if (savePath.isEmpty()) {
        return;
    }

    QNetworkRequest request(videoUrl);
    // Corrected authorization headers for Supabase
    request.setRawHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Im1jcHNkdnh2eW1kcm9mbWdoZ2hwIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTc0NjQyMzc5MCwiZXhwIjoyMDYxOTk5NzkwfQ.u1UHFatQIc1kKAlCv019iU-QxUoq1r430R0wGiWsOrM");
    request.setRawHeader("apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Im1jcHNkdnh2eW1kcm9mbWdoZ2hwIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTc0NjQyMzc5MCwiZXhwIjoyMDYxOTk5NzkwfQ.u1UHFatQIc1kKAlCv019iU-QxUoq1r430R0wGiWsOrM");

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, savePath]() {
        if (reply->error() == QNetworkReply::NoError) {
            QFile file(savePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(reply->readAll());
                file.close();
                lectureFilePath = savePath;
                MessageBoxUtil::showCustomMessage(
                    this,
                    "Lecture downloaded successfully!",
                    "Download Complete",
                    "OK"
                    );

            } else {
                MessageBoxUtil::showCustomMessage(
                    this,
                    "Failed to save lecture file!",
                    "Download Error",
                    "OK"
                    );
            }
        } else {
            QString errorMsg = QString("Failed to download: %1\nHTTP Status: %2")
            .arg(reply->errorString())
                .arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());

            MessageBoxUtil::showCustomMessage(
                this,
                errorMsg,
                "Download Error",
                "OK"
                );
        }
        reply->deleteLater();
    });
    connect(reply, &QNetworkReply::downloadProgress, this, [this](qint64 bytesReceived, qint64 bytesTotal) {
        if (bytesTotal > 0) {
            int progress = (bytesReceived * 100) / bytesTotal;
            // You could update a progress bar here if you add one
        }
    });
}

void StudentLecture::playLecture() {
    if (videoUrl.isEmpty()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "No lecture available to play!",
            "Playback Error",
            "OK"
            );
        return;
    }

    // First check if we have a downloaded local copy
    if (!lectureFilePath.isEmpty() && QFile::exists(lectureFilePath)) {
        // Play the local file
        if (!QDesktopServices::openUrl(QUrl::fromLocalFile(lectureFilePath))) {
            QMessageBox::warning(this, "Playback Error", "Could not open default media player!");
        }
    } else {
        // If no local copy exists, try to play directly from URL
        player->setSource(QUrl(videoUrl));
        player->play();
    }
}
