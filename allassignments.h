#ifndef ALLASSIGNMENTS_H
#define ALLASSIGNMENTS_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QDesktopServices>
#include <QMouseEvent>
#include <QComboBox>
#include <QNetworkAccessManager>
#include "topbar.h"
#include "StudentUploadPDF.h"

class ClickableLabelPdf : public QLabel {
    Q_OBJECT
public:
    explicit ClickableLabelPdf(QWidget *parent = nullptr) : QLabel(parent) {
        setCursor(Qt::PointingHandCursor);
    }
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }
        QLabel::mousePressEvent(event);
    }
};

class AllAssignments : public QWidget {
    Q_OBJECT
public:
    explicit AllAssignments(QWidget *parent = nullptr, QWidget *topBar = nullptr,const QString &Id="");
    ~AllAssignments();
signals:
    void backButtonClicked();
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
private slots:
    void openStudentUploadPage(const QString& title, const QString& description, const QString& pdfPath);
    void filterAssignments(const QString& subject);
    void downloadFile(const QString& url, const QString& fileName);
private:
    QWidget* createPdfBox(const QString& title, const QString& description,
                          const QString& pdfPath, const QString& uploadDate,
                          const QString& deadline);
    void populateAssignments();
    void fetchAssignments();

    TopBar *topBar;
    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QGridLayout *scrollLayout;
    QComboBox *filterComboBox;
    StudentUploadPDF *studentUploadPage;
    QNetworkAccessManager *networkManager;
    QString uniqueId;
    struct Assignment {
        QString title;
        QString description;
        QString pdfPath;
        QString uploadDate;
        QString deadline;
        QString subject;
    };

    QVector<Assignment> allAssignments;
};
#endif // ALLASSIGNMENTS_H
