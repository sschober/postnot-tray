#include <QNetworkAccessManager>
#include <QUrl>
#include <QDebug>
#include <QXmlStreamReader>

#include "task.h"
#include "poststatus.h"

void Task::run() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://portal.uni-ulm.de/PortalNG/post.html")));
}

void Task::replyFinished(QNetworkReply *nwr) {
    bool post = false;
    QVariant statusCode = nwr->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(QNetworkReply::NoError == nwr->error()){
        QXmlStreamReader xmlStrRdr(nwr);
        while(!xmlStrRdr.atEnd() && !xmlStrRdr.hasError()){
            QXmlStreamReader::TokenType t = xmlStrRdr.readNext();
            if(QXmlStreamReader::StartElement == t && xmlStrRdr.name() == "status"){
                xmlStrRdr.readNext();
//                qDebug() << xmlStrRdr.text();
                Poststatus st(xmlStrRdr.text().toString());
                if(st.hatPost(postfach_)){
//                    qDebug() << "post in fach " << (postfach_ + 1);
                }
                post = st.hatPost(postfach_);
            }
        }
    }
    else{
        qWarning() << nwr->error();
    }
    emit aktualisierung(post);
}
