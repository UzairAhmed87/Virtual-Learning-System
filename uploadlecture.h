#ifndef UPLOADLECTURE_H
#define UPLOADLECTURE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QFile>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QStackedWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QEvent> // Added to define QEvent fully
#include "DatabaseManager.h"
#include "mesgboxutil.h"
#include "SupabaseConfig.h"

class UploadLecture : public QWidget {
    Q_OBJECT

public:
    explicit UploadLecture(const QString &Id, QWidget *parent = nullptr, QWidget *topBar = nullptr);

private slots:
    void selectFile();
    void startUpload();
    void resetScreen();
    void handleFileUploadReply();
    void handleUploadReply();
    void togglePlayback();
    void loadCourses();

signals:
    void backButtonClicked();

private:
    bool eventFilter(QObject *obj, QEvent *event);

    QString filePath;
    QString uniqueId;
    QWidget *functionalWindow;
    QLabel *placeholderLabel;
    QPushButton *selectButton;
    QPushButton *uploadButton;
    QPushButton *cancelButton;
    QPushButton *pauseButton;
    QComboBox *courseDropdown;
    QLineEdit *titleInput;
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QStackedWidget *previewStack;
    QNetworkAccessManager *networkManager;
};

#endif // UPLOADLECTURE_H
