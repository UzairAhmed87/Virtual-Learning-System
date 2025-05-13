#include "AllLecture.h"
#include "DatabaseManager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QScrollBar>
#include "SupabaseConfig.h"
#include "mesgboxutil.h"

UploadPage::UploadPage(QWidget *parent, QWidget *topBar, const QString &Id) : QWidget(parent) {
    studentLecture = nullptr;  // Initialize to nullptr
    uniqueId = Id;
    videoWidget = new QVideoWidget(this);
    videoWidget->setMouseTracking(true);
    videoWidget->setAttribute(Qt::WA_Hover, true);
    videoWidget->installEventFilter(this);

    setStyleSheet("background-color: #1B263B;");
    setWindowState(Qt::WindowMaximized);

    // Clear any existing layout to prevent duplication
    if (layout()) {
        QLayoutItem *item;
        while ((item = layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout();
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Add topBar and ensure it stays at the top
    QWidget *topBarPlaceholder = new QWidget();
    topBarPlaceholder->setFixedHeight(topBar->height());
    mainLayout->addWidget(topBarPlaceholder, 0, Qt::AlignTop);

    // Scroll Area
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea { border: none; }"
        "QScrollBar:vertical {"
        "    border: none; background: #212121; width: 8px; margin: 4px 0px 4px 0px; }"
        "QScrollBar::handle:vertical { background: #888; min-height: 20px; border-radius: 4px; }"
        "QScrollBar::handle:vertical:hover { background: #666; }"
        );

    QWidget *scrollContent = new QWidget(scrollArea);
    scrollContent->setStyleSheet("background-color: #1B263B;");

    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setAlignment(Qt::AlignTop);
    scrollLayout->setContentsMargins(20, 20, 20, 20);
    scrollLayout->setSpacing(20);

    // Header Layout with back button, title, and filter
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(10);

    // Back Button
    QPushButton *backButton = new QPushButton(scrollContent);
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

    // Heading
    QLabel *headingLabel = new QLabel("All Lectures", this);
    headingLabel->setAlignment(Qt::AlignCenter);
    headingLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");

    // Filter components
    QHBoxLayout *filterLayout = new QHBoxLayout();
    QLabel *filterLabel = new QLabel("Filter by Subject:", this);
    filterLabel->setStyleSheet("color: white; font-size: 14px;");

    filterComboBox = new QComboBox(this);
    filterComboBox->addItem("All Subjects");
    filterComboBox->setStyleSheet(R"(
        QComboBox {
            background-color: #415A77;
            color: white;
            border: 1px solid #778DA9;
            border-radius: 5px;
            padding: 5px;
            min-width: 150px;
        }
        QComboBox::drop-down {
            border: none;
        }
        QComboBox QAbstractItemView {
            background-color: #415A77;
            color: white;
            selection-background-color: #778DA9;
        }
    )");

    connect(filterComboBox, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this, &UploadPage::filterLectures);

    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterComboBox);

    // Add items to header
    headerLayout->addWidget(backButton, 0, Qt::AlignLeft);
    headerLayout->addWidget(headingLabel, 0, Qt::AlignCenter);
    headerLayout->addStretch();
    headerLayout->addStretch();
    headerLayout->addLayout(filterLayout);

    scrollLayout->addLayout(headerLayout);

    // Functional Window
    functionalWindow = new QWidget(scrollContent);
    functionalWindow->setStyleSheet("background-color: #283044; border-radius: 20px;");
    functionalWindow->setMinimumWidth(600);
    functionalWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QVBoxLayout *functionalLayout = new QVBoxLayout(functionalWindow);
    functionalLayout->setAlignment(Qt::AlignTop);
    functionalLayout->setContentsMargins(20, 20, 20, 20);
    functionalLayout->setSpacing(20);

    // Video Layout
    videoLayout = new QGridLayout();
    videoLayout->setAlignment(Qt::AlignCenter);
    videoLayout->setSpacing(20);

    // Initialize network manager
    networkManager = new QNetworkAccessManager(this);

    // Fetch lectures from Supabase
    fetchLectures();

    functionalLayout->addLayout(videoLayout);
    functionalWindow->setLayout(functionalLayout);
    scrollLayout->addWidget(functionalWindow, 0, Qt::AlignCenter);

    scrollContent->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea, 1); // Stretch factor to fill remaining space
    setLayout(mainLayout);
}

void UploadPage::fetchLectures() {
    // Step 1: Get database from DatabaseManager
    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        MessageBoxUtil::showCustomMessage(this, "Database connection is not open.",
                                          "Error", "OK");
        return;
    }

    QString role;
    QStringList enrolledCourses;

    // Query to get user role
    QSqlQuery roleQuery(db);
    roleQuery.prepare("SELECT role FROM vls_schema.users WHERE unique_id = :unique_id");
    roleQuery.bindValue(":unique_id", uniqueId);

    if (roleQuery.exec() && roleQuery.next()) {
        role = roleQuery.value("role").toString();
    } else {
        MessageBoxUtil::showCustomMessage(this, "Failed to fetch user role: " + roleQuery.lastError().text(),
                                          "Error", "OK");
        return;
    }

    // If student, get enrolled courses
    if (role.toLower() == "student") {
        QSqlQuery enrollmentQuery(db);
        enrollmentQuery.prepare("SELECT course_code FROM vls_schema.enrollments WHERE student_id = :student_id AND status = 'active'");
        enrollmentQuery.bindValue(":student_id", uniqueId);

        if (enrollmentQuery.exec()) {
            while (enrollmentQuery.next()) {
                enrolledCourses << enrollmentQuery.value("course_code").toString();
            }
        } else {
            MessageBoxUtil::showCustomMessage(this, "Failed to fetch enrollments: " + enrollmentQuery.lastError().text(),
                                              "Error", "OK");
            return;
        }
    }

    // Step 2: Construct Supabase query based on role
    QString queryFilter;
    if (role.toLower() == "teacher") {
        // Filter by teacher_id
        queryFilter = QString("?select=*&file_type=eq.video&teacher_id=eq.%1").arg(uniqueId);
    } else if (role.toLower() == "student" && !enrolledCourses.isEmpty()) {
        // Filter by course_code in enrolled courses
        QString courseFilter = enrolledCourses.join(",");
        queryFilter = QString("?select=*&file_type=eq.video&course_code=in.(%1)").arg(courseFilter);
    } else {
        // No lectures for students not enrolled or invalid role
        allLectures.clear();
        filterComboBox->clear();
        filterComboBox->addItem("All Subjects");
        populateLectures();
        return;
    }

    // Step 3: Fetch lectures from Supabase
    QNetworkRequest request(QUrl(SupabaseConfig::getFilesApiUrl() + queryFilter));
    request.setRawHeader("apikey", SupabaseConfig::getApiKey().toUtf8());
    request.setRawHeader("Authorization", "Bearer " + SupabaseConfig::getServiceRoleKey().toUtf8());

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            QJsonArray jsonArray = jsonDoc.array();

            allLectures.clear();
            QSet<QString> subjects;

            for (const QJsonValue &value : jsonArray) {
                QJsonObject obj = value.toObject();
                if (obj["file_type"].toString() == "video") {
                    Lecture lecture;
                    lecture.title = obj["file_name"].toString();
                    lecture.description = obj["file_description"].toString();
                    lecture.videoPath = obj["file_url"].toString();
                    lecture.subject = obj["course_code"].toString();

                    allLectures.append(lecture);

                    // Add unique subjects to filter combo box
                    if (!subjects.contains(lecture.subject)) {
                        subjects.insert(lecture.subject);
                        filterComboBox->addItem(lecture.subject);
                    }
                }
            }

            populateLectures();
        } else {
            MessageBoxUtil::showCustomMessage(this, "Failed to fetch lectures: " + reply->errorString(),
                                              "Error", "OK");
        }
        reply->deleteLater();
    });
}

void UploadPage::populateLectures() {
    // Clear existing widgets
    QLayoutItem* item;
    while ((item = videoLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Add lectures to the layout
    int columns = 3;
    for (int i = 0; i < allLectures.size(); ++i) {
        QWidget *videoColumn = createVideoColumn(allLectures[i].title,
                                                 allLectures[i].description,
                                                 allLectures[i].videoPath);
        int row = i / columns;
        int col = i % columns;
        videoLayout->addWidget(videoColumn, row, col);
    }
}

void UploadPage::filterLectures(const QString& subject) {
    // Clear existing widgets
    QLayoutItem* item;
    while ((item = videoLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Add filtered lectures to the layout
    int columns = 3;
    int counter = 0;
    for (const Lecture& lecture : allLectures) {
        if (subject == "All Subjects" || lecture.subject == subject) {
            QWidget *videoColumn = createVideoColumn(lecture.title,
                                                     lecture.description,
                                                     lecture.videoPath);
            int row = counter / columns;
            int col = counter % columns;
            videoLayout->addWidget(videoColumn, row, col);
            counter++;
        }
    }
}

QWidget* UploadPage::createVideoColumn(const QString& title, const QString& description, const QString& videoPath) {
    QWidget *column = new QWidget(this);
    QVBoxLayout *columnLayout = new QVBoxLayout(column);
    columnLayout->setAlignment(Qt::AlignCenter);
    columnLayout->setSpacing(10);

    // QMediaPlayer and QVideoWidget
    QMediaPlayer *mediaPlayer = new QMediaPlayer(this);
    QVideoWidget *videoWidget = new QVideoWidget(this);
    mediaPlayer->setVideoOutput(videoWidget);

    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    videoWidget->setFixedSize(350, 200);
    videoWidget->installEventFilter(this);

    mediaPlayer->setSource(QUrl(videoPath));
    mediaPlayer->pause();

    // Title Label (always visible below the video)
    QLabel *titleLabel = new QLabel(title, column);
    titleLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedWidth(250);
    titleLabel->setWordWrap(true);

    // Watch Label (shown on hover)
    QLabel *watchLabel = new QLabel("Watch this video", column);
    watchLabel->setStyleSheet("font-size: 14px; color: #E0E1DD; background-color: rgba(0, 0, 0, 0.7); padding: 5px; border-radius: 5px;");
    watchLabel->setAlignment(Qt::AlignCenter);
    watchLabel->setVisible(false); // Hidden by default

    // Store properties for event handling
    videoWidget->setProperty("title", title);
    videoWidget->setProperty("description", description);
    videoWidget->setProperty("videoPath", videoPath);
    videoWidget->setProperty("watchLabel", QVariant::fromValue(watchLabel));

    // Add widgets to layout
    columnLayout->addWidget(videoWidget, 0, Qt::AlignCenter);
    columnLayout->addWidget(watchLabel, 0, Qt::AlignCenter);
    columnLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    column->setLayout(columnLayout);
    return column;
}

bool UploadPage::eventFilter(QObject *obj, QEvent *event) {
    if (QVideoWidget *videoWidget = qobject_cast<QVideoWidget*>(obj)) {
        QLabel *watchLabel = qobject_cast<QLabel*>(videoWidget->property("watchLabel").value<QObject*>());
        if (!watchLabel) {
            qDebug() << "⚠️ WatchLabel is NULL! Check createVideoColumn().";
            return false;
        }

        if (event->type() == QEvent::Enter) {
            qDebug() << "Mouse entered video!";
            watchLabel->setVisible(true);
        }
        else if (event->type() == QEvent::Leave) {
            qDebug() << "Mouse left video!";
            watchLabel->setVisible(false);
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                QString title = videoWidget->property("title").toString();
                QString description = videoWidget->property("description").toString();
                QString videoPath = videoWidget->property("videoPath").toString();

                // Open the student lecture page
                openVideoScreen(title, description, videoPath);
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void UploadPage::resetScreen() {
    // Reset the scroll area to the top
    scrollArea->verticalScrollBar()->setValue(0);
    // Refetch lectures to ensure content is up-to-date
    fetchLectures();
}

void UploadPage::openVideoScreen(const QString& title, const QString& description, const QString& videoPath) {
    qDebug() << "Opening video: " << title << " at " << videoPath;

    // Create or reuse the StudentLecture window
    if (!studentLecture) {
        studentLecture = new StudentLecture();
    }

    // Update the lecture information
    studentLecture->setLectureInfo(title, description, videoPath);

    // Show the window
    studentLecture->show();
    studentLecture->raise();
    studentLecture->activateWindow();
}
