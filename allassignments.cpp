#include "AllAssignments.h"
#include "SupabaseConfig.h"
#include <QDebug>
#include <QEvent>
#include <QHoverEvent>
#include <QGridLayout>
#include <QFile>
#include <QDesktopServices>
#include <QIcon>
#include <QPixmap>
#include <QMessageBox>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QPushButton>
#include <QStandardPaths>
#include <QMouseEvent>
#include "mesgboxutil.h"

AllAssignments::AllAssignments(QWidget *parent, QWidget *topBar, const QString &Id)
    : QWidget(parent), studentUploadPage(nullptr) {
    setStyleSheet("background-color: #1B263B;");
    setWindowState(Qt::WindowMaximized);
    uniqueId = Id;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *topBarPlaceholder = new QWidget();
    topBarPlaceholder->setFixedHeight(topBar->height());
    mainLayout->addWidget(topBarPlaceholder, 0, Qt::AlignTop);

    // Header Layout
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(10);

    // Back Button
    QPushButton *backButton = new QPushButton(this);
    backButton->setIcon(QIcon("images/back_arrow.png"));
    backButton->setIconSize(QSize(30, 30));
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton { background: none; border: none; padding: 5px; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.2); border-radius: 5px; }"
        "QPushButton:pressed { background-color: rgba(255, 255, 255, 0.3); border-radius: 5px; }"
        );
    connect(backButton, &QPushButton::clicked, this, [=](){
        emit backButtonClicked();
    });

    QLabel *allAssignmentsLabel = new QLabel("All Assignments", this);
    allAssignmentsLabel->setStyleSheet("font-size: 24px; color: white; font-weight: bold;background:transparent;");

    // Filter components
    QHBoxLayout *filterLayout = new QHBoxLayout();
    QLabel *filterLabel = new QLabel("Filter by Subject:", this);
    filterLabel->setStyleSheet("color: white; font-size: 14px;background:transparent");

    filterComboBox = new QComboBox(this);
    filterComboBox->addItem("All Subjects");
    filterComboBox->setStyleSheet(
        "QComboBox { background-color: #415A77; color: white; border: 1px solid #778DA9; "
        "border-radius: 5px; padding: 5px; min-width: 150px; }"
        "QComboBox::drop-down { border: none; }"
        "QComboBox QAbstractItemView { background-color: #415A77; color: white; "
        "selection-background-color: #778DA9; }"
        );
    connect(filterComboBox, &QComboBox::currentTextChanged,
            this, &AllAssignments::filterAssignments);

    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterComboBox);

    headerLayout->addWidget(backButton, 0, Qt::AlignLeft);
    headerLayout->addWidget(allAssignmentsLabel);
    headerLayout->addStretch();
    headerLayout->addStretch();
    headerLayout->addLayout(filterLayout);

    mainLayout->addLayout(headerLayout);

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

    scrollContent = new QWidget(scrollArea);
    scrollLayout = new QGridLayout(scrollContent);
    scrollLayout->setAlignment(Qt::AlignTop);
    scrollLayout->setSpacing(20);
    scrollLayout->setContentsMargins(20, 20, 20, 20);
    scrollContent->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    // Initialize network manager
    networkManager = new QNetworkAccessManager(this);

    // Fetch assignments from Supabase
    fetchAssignments();
}

AllAssignments::~AllAssignments() {
    if (studentUploadPage) {
        studentUploadPage->deleteLater();
    }
    if (networkManager) {
        networkManager->deleteLater();
    }
}

void AllAssignments::fetchAssignments() {
    QNetworkRequest request(QUrl(SupabaseConfig::getFilesApiUrl() + "?select=*"));
    request.setRawHeader("apikey", SupabaseConfig::getServiceRoleKey().toUtf8());
    request.setRawHeader("Authorization", ("Bearer " + SupabaseConfig::getServiceRoleKey()).toUtf8());

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            qDebug() << "Supabase Response:" << response;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            if (jsonDoc.isNull()) {
                qDebug() << "Invalid JSON response";
                MessageBoxUtil::showCustomMessage(this, "Invalid JSON response from Supabase",
                                                  "Error", "OK");
                reply->deleteLater();
                return;
            }
            QJsonArray jsonArray = jsonDoc.array();
            qDebug() << "Number of records:" << jsonArray.size();

            allAssignments.clear();
            QSet<QString> subjects;

            for (const QJsonValue &value : jsonArray) {
                QJsonObject obj = value.toObject();
                qDebug() << "Record:" << obj;
                if (obj["file_url"].toString().contains("/assignments/") || obj["file_type"].toString() == "pdf") {
                    Assignment assignment;
                    assignment.title = obj["file_name"].toString();
                    assignment.description = obj["file_description"].toString();
                    assignment.pdfPath = obj["file_url"].toString();
                    assignment.uploadDate = obj["uploaded_at"].toString().left(10);
                    assignment.deadline = obj.contains("deadline") ? obj["deadline"].toString() : "Not specified";
                    assignment.subject = obj["course_code"].toString().isEmpty() ? "Unknown" : obj["course_code"].toString();

                    allAssignments.append(assignment);
                    qDebug() << "Added assignment:" << assignment.title;

                    if (!subjects.contains(assignment.subject)) {
                        subjects.insert(assignment.subject);
                        filterComboBox->addItem(assignment.subject);
                    }
                }
            }

            qDebug() << "Total assignments loaded:" << allAssignments.size();
            populateAssignments();
        } else {
            qDebug() << "Network Error:" << reply->errorString();
            MessageBoxUtil::showCustomMessage(this, "Failed to fetch assignments: " + reply->errorString(),
                                              "Error", "OK");
        }
        reply->deleteLater();
    });
}

void AllAssignments::populateAssignments() {
    QLayoutItem* item;
    while ((item = scrollLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    int row = 0, col = 0;
    qDebug() << "Populating" << allAssignments.size() << "assignments";
    for (const Assignment& assignment : allAssignments) {
        qDebug() << "Creating box for:" << assignment.title;
        QWidget *pdfBox = createPdfBox(assignment.title, assignment.description,
                                       assignment.pdfPath, assignment.uploadDate,
                                       assignment.deadline);
        scrollLayout->addWidget(pdfBox, row, col);
        col++;
        if (col > 2) {
            col = 0;
            row++;
        }
    }
}

void AllAssignments::filterAssignments(const QString& subject) {
    QLayoutItem* item;
    while ((item = scrollLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    int row = 0, col = 0;
    for (const Assignment& assignment : allAssignments) {
        if (subject == "All Subjects" || assignment.subject == subject) {
            QWidget *pdfBox = createPdfBox(assignment.title, assignment.description,
                                           assignment.pdfPath, assignment.uploadDate,
                                           assignment.deadline);
            scrollLayout->addWidget(pdfBox, row, col);
            col++;
            if (col > 2) {
                col = 0;
                row++;
            }
        }
    }
}

QWidget* AllAssignments::createPdfBox(const QString& title, const QString& description,
                                      const QString& pdfPath, const QString& uploadDate,
                                      const QString& deadline) {
    QWidget *pdfBox = new QWidget(this);
    pdfBox->setStyleSheet(
        "background-color: #2C3A4F; border-radius: 10px; padding: 20px; border: 1px solid #415A77;"
        );
    pdfBox->setFixedSize(350, 150);

    QVBoxLayout *boxLayout = new QVBoxLayout(pdfBox);
    boxLayout->setAlignment(Qt::AlignCenter);

    // Download Button
    QPushButton *downloadButton = new QPushButton("Download", pdfBox);
    downloadButton->setStyleSheet(
        "QPushButton { background-color: #778DA9; color: white; border-radius: 5px; padding: 5px; font-size: 14px; }"
        "QPushButton:hover { background-color: #415A77; }"
        "QPushButton:pressed { background-color: #2C3A4F; }"
        );
    downloadButton->setFixedSize(100, 30);
    downloadButton->setVisible(false); // Initially hidden
    boxLayout->addWidget(downloadButton, 0, Qt::AlignCenter);

    // Connect download button
    connect(downloadButton, &QPushButton::clicked, this, [=]() {
        downloadFile(pdfPath, title.isEmpty() ? "assignment.pdf" : title);
    });

    QLabel *titleLabel = new QLabel(title.isEmpty() ? "Untitled" : title, this);
    titleLabel->setStyleSheet("font-size: 16px; color: white; font-weight: bold; margin-top: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *deadlineLabel = new QLabel("Deadline: " + deadline, this);
    deadlineLabel->setStyleSheet("font-size: 14px; color: #A0AEC0; margin-top: 1px;");
    deadlineLabel->setAlignment(Qt::AlignCenter);

    QLabel *descriptionLabel = new QLabel(this);
    descriptionLabel->setStyleSheet(
        "font-size: 14px; color: #F0F0F0; background-color: rgba(44, 58, 79, 0.95); "
        "padding: 10px; border-radius: 10px; border: 1px solid #415A77;"
        );
    descriptionLabel->setAlignment(Qt::AlignCenter);
    descriptionLabel->setText("Upload Date: " + uploadDate + "\n" + description);
    descriptionLabel->setVisible(false);
    descriptionLabel->setFixedWidth(350);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setMinimumHeight(70);

    pdfBox->setAttribute(Qt::WA_Hover, true);
    pdfBox->installEventFilter(this);
    pdfBox->setProperty("descriptionLabel", QVariant::fromValue(descriptionLabel));
    pdfBox->setProperty("downloadButton", QVariant::fromValue(downloadButton));
    pdfBox->setProperty("title", title);
    pdfBox->setProperty("description", description);
    pdfBox->setProperty("pdfPath", pdfPath);

    // Enable mouse tracking for clicking
    pdfBox->setMouseTracking(true);

    QVBoxLayout *outerLayout = new QVBoxLayout();
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(5);
    outerLayout->addWidget(pdfBox);
    outerLayout->addWidget(titleLabel);
    outerLayout->addWidget(deadlineLabel);
    outerLayout->addWidget(descriptionLabel);
    outerLayout->setAlignment(Qt::AlignCenter);

    QWidget *container = new QWidget(this);
    container->setLayout(outerLayout);

    return container;
}

void AllAssignments::downloadFile(const QString& url, const QString& fileName) {
    QNetworkRequest request((QUrl(url)));
    QNetworkReply *reply = networkManager->get(QNetworkRequest(QUrl(url)));

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
            QString savePath = downloadsPath + "/" + fileName;

            QFile file(savePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(data);
                file.close();
                MessageBoxUtil::showCustomMessage(this, "File downloaded to: " + savePath,
                                                  "Success", "OK");
            } else {
                MessageBoxUtil::showCustomMessage(this, "Failed to save file: " + file.errorString(),
                                                  "Error", "OK");
            }
        } else {
            MessageBoxUtil::showCustomMessage(this, "Failed to download file: " + reply->errorString(),
                                              "Error", "OK");
        }
        reply->deleteLater();
    });
}

void AllAssignments::openStudentUploadPage(const QString& title, const QString& description, const QString& pdfPath) {
    if (!studentUploadPage) {
        studentUploadPage = new StudentUploadPDF();
        studentUploadPage->setWindowModality(Qt::ApplicationModal);
        connect(studentUploadPage, &QObject::destroyed, [this]() {
            studentUploadPage = nullptr;
        });
    }
    studentUploadPage->setAssignmentDetails(title, description, pdfPath);
    studentUploadPage->show();
    studentUploadPage->raise();
    studentUploadPage->activateWindow();
}

bool AllAssignments::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::HoverEnter) {
        QWidget *pdfBox = qobject_cast<QWidget*>(obj);
        if (pdfBox) {
            QLabel *descriptionLabel = pdfBox->property("descriptionLabel").value<QLabel*>();
            QPushButton *downloadButton = pdfBox->property("downloadButton").value<QPushButton*>();
            if (descriptionLabel && downloadButton) {
                int x = pdfBox->mapToGlobal(QPoint(0, 0)).x() - scrollArea->mapToGlobal(QPoint(0, 0)).x() +
                        (pdfBox->width() - descriptionLabel->width()) / 2;
                int y = pdfBox->mapToGlobal(QPoint(0, 0)).y() - scrollArea->mapToGlobal(QPoint(0, 0)).y() +
                        pdfBox->height() + 10;

                descriptionLabel->move(x, y);
                descriptionLabel->setVisible(true);
                descriptionLabel->raise();

                downloadButton->setVisible(true);
            }
        }
    } else if (event->type() == QEvent::HoverLeave) {
        QWidget *pdfBox = qobject_cast<QWidget*>(obj);
        if (pdfBox) {
            QLabel *descriptionLabel = pdfBox->property("descriptionLabel").value<QLabel*>();
            QPushButton *downloadButton = pdfBox->property("downloadButton").value<QPushButton*>();
            if (descriptionLabel && downloadButton) {
                descriptionLabel->setVisible(false);
                downloadButton->setVisible(false);
            }
        }
    } else if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            QWidget *pdfBox = qobject_cast<QWidget*>(obj);
            if (pdfBox) {
                QString title = pdfBox->property("title").toString();
                QString description = pdfBox->property("description").toString();
                QString pdfPath = pdfBox->property("pdfPath").toString();
                openStudentUploadPage(title, description, pdfPath);
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}
