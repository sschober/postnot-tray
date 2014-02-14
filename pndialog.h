#ifndef PNDIALOG_H
#define PNDIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include "task.h"

namespace Ui {
class PNDialog;
}

class PNDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PNDialog(QWidget *parent = 0);
    ~PNDialog();

public slots:
    void updateIcon(QString);
    void updateConfig();
    void showAbout();
    void cbProxyClicked(bool);
    void connectionProblem(QString);

private:
    Ui::PNDialog *ui;
    QSystemTrayIcon *ti;
    QMenu *qm;
    QAction *qaQuit;
    QAction *qaConfig;
    QTimer *t;
    Task *ta;
    QIcon *icnEMail;
    QIcon *icnEMailAktiv;
    QIcon *icnEMailUnbekannt;
    const static char *STATUS_;
};
#endif // PNDIALOG_H
