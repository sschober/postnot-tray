#include <QUrl>
#include <QDebug>
#include <QXmlStreamReader>

#include "task.h"
#include "poststatus.h"

void Task::run() {
    mgr_->get(QNetworkRequest(QUrl("http://portal.uni-ulm.de/PortalNG/post.html")));
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
                emit aktualisierung(xmlStrRdr.text().toString());
            }
        }
    }
    else{
        qWarning() << nwr->error();
    }
}
