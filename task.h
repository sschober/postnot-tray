#ifndef TASK_H
#define TASK_H

#include <QTimer>
#include <QNetworkReply>

class Task : public QObject {
    Q_OBJECT
public:
    Task(QObject *parent = 0, int postfach = 0 ) : QObject(parent), postfach_(postfach) {}
public slots:
    void run();
    void replyFinished(QNetworkReply *nwr);

signals:
    void aktualisierung(bool status);
private:
    int postfach_;
};

#endif // TASK_H
