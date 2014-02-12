#include "pndialog.h"
#include "ui_pndialog.h"

#include <QSystemTrayIcon>
#include <QDebug>
#include <QStringBuilder>
#include <QMessageBox>
#include "poststatus.h"

const char *PNDialog::STATUS_ = "status";

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

        icnEMail = new QIcon(":/icon/email-small");
        icnEMailAktiv = new QIcon(":/icon/email-aktiv");

        ti->setIcon(*icnEMail);
        ti->setToolTip("PostNot");
        ti->setProperty(STATUS_,QVariant(false));
        ti->show();

        ta = new Task(this);
        connect(ta,SIGNAL(aktualisierung(QString)),this,SLOT(updateIcon(QString)));
        t = new QTimer(this);
        connect(t, SIGNAL(timeout()),ta,SLOT(run()));
        t->start(1000);

        connect(ui->pbOK,SIGNAL(clicked()),this,SLOT(updateConfig()));
        connect(ui->pbCancel,SIGNAL(clicked()),this, SLOT(hide()));
        connect(ui->pbQuit,SIGNAL(clicked()),qApp, SLOT(quit()));

        setWindowFlags((Qt::CustomizeWindowHint | Qt::WindowTitleHint) & ~Qt::WindowMaximizeButtonHint);
        this->setFixedSize(this->size());
    }
    else {
        QMessageBox::critical(this, "Keine Task-Leiste verfuegbar","Keine Task-Leiste verfuegbar.");
        qApp->quit();
    }
}

void PNDialog::updateIcon(QString status){
    Poststatus p(status);
    ui->lbStatus->setText(status);
    if(p.hatPost(ui->sbPostfach->value()-1)){
        // Flanken getriggerte Notifikation
        // TODO: Eigentlich muss ich mir dazu auch noch das Postfach merken
        if(ti->property(STATUS_).isValid() &&
                !ti->property(STATUS_).toBool()){
            // status war vorher inaktiv
            ti->showMessage(  "Neue Post",
                              QString("Es befindet sich Post in Fach ")
                                % QString().setNum(ui->sbPostfach->value())
                                % QString("!"));
        }
        ti->setProperty(STATUS_,QVariant(true));
        ti->setIcon(*icnEMailAktiv);
        ti->setToolTip( QString("Post in Postfach: ") % QString().setNum(ui->sbPostfach->value()));
    }
    else{
        if(ti->property(STATUS_).isValid()
                && ti->property(STATUS_).toBool()){
            // status war vorher aktiv
            ti->showMessage( "Post entfernt",
                             QString("Es befindet sich keine Post in Fach ")
                                % QString().setNum(ui->sbPostfach->value())
                                % QString("!"));
        }
        ti->setProperty(STATUS_,QVariant(false));
        ti->setIcon(*icnEMail);
        ti->setToolTip( QString("Keine Post in Postfach: ") % QString().setNum(ui->sbPostfach->value()));
    }
    ti->show();
}

void PNDialog::updateConfig() {
    t->stop();
    delete ta;
    ta = new Task(this);
    connect(ta,SIGNAL(aktualisierung(QString)),this,SLOT(updateIcon(QString)));
    connect(t, SIGNAL(timeout()),ta,SLOT(run()));
    QTime interval(ui->teIntervall->time());
    t->start( 1000 * 60 * interval.minute()+ 1000 * interval.second());
    this->hide();
}

PNDialog::~PNDialog()
{
    delete ui;
}
