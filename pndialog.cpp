#include "pndialog.h"
#include "ui_pndialog.h"

#include <QSystemTrayIcon>
#include <QDebug>
#include <QStringBuilder>
#include <QMessageBox>
#include <QSettings>

#include "poststatus.h"
#include "ui_about.h"

#define INTERVALL "intervall"
#define POSTFACH "postfach"
#define HOURS "hours"
#define MINS "mins"
#define SECS "secs"
#define DEFAULT_INTERVALL_HOURS 0
#define DEFAULT_INTERVALL_MINS 1
#define DEFAULT_INTERVALL_SECS 0

const char *PNDialog::STATUS_ = "status";

PNDialog::PNDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PNDialog)
{
    ui->setupUi(this);
    if(QSystemTrayIcon::isSystemTrayAvailable()){

        qaQuit = new QAction(tr("&Beenden"), this);
        connect(qaQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
        QAction *qaAbout = new QAction(tr("Ü&ber PostNot-Tray"),this);
        connect(qaAbout,SIGNAL(triggered()),this, SLOT(showAbout()));
        qaConfig = new QAction(tr("&Konfigurieren"),this);
        connect(qaConfig, SIGNAL(triggered()), this, SLOT(show()));

        qm = new QMenu(this);
        qm->addAction(qaConfig);
        qm->addAction(qaAbout);
        qm->addSeparator();
        qm->addAction(qaQuit);

        ti = new QSystemTrayIcon(this);
        ti->setContextMenu(qm);

        icnEMail = new QIcon(":/icon/email-small");
        icnEMailAktiv = new QIcon(":/icon/email-aktiv");
        icnEMailUnbekannt = new QIcon(":/icon/unbekannt");

        ti->setIcon(*icnEMailUnbekannt);
        ti->setToolTip("PostNot");
        ti->setProperty(STATUS_,QVariant(false));
        ti->show();

        ta = new Task(this);
        connect(ta,SIGNAL(aktualisierung(QString)),this,SLOT(updateIcon(QString)));
        t = new QTimer(this);
        connect(t, SIGNAL(timeout()),ta,SLOT(run()));
        QSettings settings;
        QTime ti(settings.value(HOURS, DEFAULT_INTERVALL_HOURS).toInt(),
                settings.value(MINS, DEFAULT_INTERVALL_MINS).toInt(),
                settings.value(SECS, DEFAULT_INTERVALL_SECS).toInt());
        t->start(1000 * ti.second() + 1000 * 60 * ti.minute() + 1000 * 60 * 60 *ti.hour());

        QTimer::singleShot(0, ta, SLOT(run()));

        ui->teIntervall->setTime(ti);
        ui->sbPostfach->setValue(settings.value(POSTFACH,0).toInt() + 1);

        connect(ui->pbOK,SIGNAL(clicked()),this,SLOT(updateConfig()));
        connect(ui->pbCancel,SIGNAL(clicked()),this, SLOT(hide()));
        connect(ui->pbQuit,SIGNAL(clicked()),qApp, SLOT(quit()));

        setWindowFlags((Qt::CustomizeWindowHint | Qt::WindowTitleHint) & ~Qt::WindowMaximizeButtonHint);
        setFixedSize(this->size());
        ui->lbStatus->setAlignment(Qt::AlignHCenter);
        qApp->setQuitOnLastWindowClosed(false);

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
    t->start( 1000 * 60 * 60 * interval.hour() + 1000 * 60 * interval.minute() + 1000 * interval.second());

    QSettings settings;
    settings.setValue(QString(POSTFACH), ui->sbPostfach->value()-1);
    settings.setValue(HOURS, interval.hour());
    settings.setValue(MINS,interval.minute());
    settings.setValue(SECS, interval.second());

    this->hide();
}

void PNDialog::showAbout(){
    QDialog *d = new QDialog(this);
    Ui_About *a = new Ui_About();
    a->setupUi(d);
    a->lbVersion->setText(QString("Version: ") % qApp->applicationVersion());
    a->lbText->setOpenExternalLinks(true);
    d->setFixedSize(d->size());
    d->exec();
}

PNDialog::~PNDialog()
{
    delete ui;
}
