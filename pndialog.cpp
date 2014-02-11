#include "pndialog.h"
#include "ui_pndialog.h"

#include <QSystemTrayIcon>
#include <QDebug>

PNDialog::PNDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PNDialog)
{
    ui->setupUi(this);
    if(QSystemTrayIcon::isSystemTrayAvailable()){

        qaQuit = new QAction(tr("&Quit"), this);
        connect(qaQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
        qaConfig = new QAction(tr("&Configure"),this);
        connect(qaConfig, SIGNAL(triggered()), this, SLOT(show()));

        qm = new QMenu(this);
        qm->addAction(qaConfig);
        qm->addSeparator();
        qm->addAction(qaQuit);

        ti = new QSystemTrayIcon(this);
        ti->setContextMenu(qm);

        QIcon icon(":/icon/email-small");
        if(icon.isNull()){
            qDebug() << "Icon was null!";
            return;
        }
        ti->setIcon(icon);
        ti->setToolTip("PostNot");
        ti->show();

        ta = new Task(this);
        connect(ta,SIGNAL(aktualisierung(bool)),this,SLOT(updateIcon(bool)));
        t = new QTimer(this);
        connect(t, SIGNAL(timeout()),ta,SLOT(run()));
        t->start(1000);
        connect(ui->pbOK,SIGNAL(clicked()),this,SLOT(updateConfig()));
    }
}

void PNDialog::updateIcon(bool status){
    if(status){
        QIcon icon(":/icon/email-aktiv");
        ti->setIcon(icon);
    }
    else{
        QIcon icon(":/icon/email-small");
        ti->setIcon(icon);
    }
    ti->show();
}

void PNDialog::updateConfig() {
    t->stop();
    delete ta;
    ta = new Task(this,ui->sbPostfach->value()-1);
    connect(ta,SIGNAL(aktualisierung(bool)),this,SLOT(updateIcon(bool)));
    connect(t, SIGNAL(timeout()),ta,SLOT(run()));
    t->start(1000);
    this->hide();
}

PNDialog::~PNDialog()
{
    delete ui;
}
