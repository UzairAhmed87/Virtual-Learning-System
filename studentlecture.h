#ifndef STUDENTLECTURE_H
#define STUDENTLECTURE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextBrowser>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDebug>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "TopBar.h"

class StudentLecture : public QWidget {
    Q_OBJECT

public:
    explicit StudentLecture(QWidget *parent = nullptr);
    void setLectureInfo(const QString& title, const QString& description, const QString& videoUrl);

private slots:
    void downloadLecture();
    void playLecture();

private:
    QString lectureFilePath;
    QString videoUrl;
    QLabel *assignmentDetails;
    QTextBrowser *lectureDescription;
    QPushButton *downloadButton;
    QPushButton *playButton;
    TopBar *topBar;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QNetworkAccessManager *networkManager;
};

#endif // STUDENTLECTURE_H
