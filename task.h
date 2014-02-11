#ifndef TASK_H
#define TASK_H

#include <QTimer>
#include <QNetworkReply>

class Task : public QObject {
    Q_OBJECT
public:
    Task(QObject *parent = 0, int postfach = 0 ) :
        QObject(parent),
        postfach_(postfach),
        mgr_(new QNetworkAccessManager(this)) {
        connect(mgr_, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
    }
public slots:
    void run();
    void replyFinished(QNetworkReply *nwr);

signals:
    void aktualisierung(bool status);
private:
    int postfach_;
    QNetworkAccessManager *mgr_;
};

#endif // TASK_H
