#ifndef UPLOADLECTRE_H
#define UPLOADLECTRE_H

#include <QWidget>
#include <QEvent>
#include <QApplication>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QStackedWidget>
#include "TopBar.h"
#include <QNetworkAccessManager>
#include <QComboBox>
class UploadLecture : public QWidget {
    Q_OBJECT

public:
    explicit UploadLecture(const QString &uniqueId="",QWidget *parent = nullptr,QWidget *topBar=nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void selectFile();
    void startUpload();
    void resetScreen();
    void togglePlayback();
    void handleUploadReply();
    void handleFileUploadReply();
    void loadCourses();
private:
    QComboBox *courseDropdown;
    QPushButton *uploadButton, *cancelButton, *selectButton, *pauseButton;
    QLineEdit *titleInput;
    QTextEdit *descriptionInput;
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QStackedWidget *previewStack;
    QString uniqueId;
    QString filePath;
    // TopBar *topBar;
    QWidget *functionalWindow;
    QLabel *placeholderLabel;
     QNetworkAccessManager *networkManager;
signals:
    void backButtonClicked();
};

#endif // UPLOADLECTURE_H
