#ifndef ALLLECTURE_H
#define ALLLECTURE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QMouseEvent>
#include "SupabaseConfig.h"
#include "studentlecture.h" // Assumed to exist; comment out if not available

class UploadPage : public QWidget {
    Q_OBJECT

public:
    explicit UploadPage(QWidget *parent = nullptr, QWidget *topBar = nullptr,const QString &Id="");

private slots:
    void fetchLectures();
    void populateLectures();
    void filterLectures(const QString &subject);
    void resetScreen();
    void openVideoScreen(const QString &title, const QString &description, const QString &videoPath);

signals:
    void backButtonClicked();

private:
    struct Lecture {
        QString title;
        QString description;
        QString videoPath;
        QString subject;
    };

    QWidget* createVideoColumn(const QString &title, const QString &description, const QString &videoPath);
    bool eventFilter(QObject *obj, QEvent *event);

    QScrollArea *scrollArea;
    QGridLayout *videoLayout;
    QComboBox *filterComboBox;
    QWidget *functionalWindow;
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QString uniqueId;
    QNetworkAccessManager *networkManager;
    StudentLecture *studentLecture;
    QList<Lecture> allLectures;
};

#endif // ALLLECTURE_H
