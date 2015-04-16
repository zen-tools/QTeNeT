#include "parsexml.h"

ParseXML::ParseXML(QObject *parent) :
    QObject(parent) {

}


ParseXML::ParseXML(QStringList vars, QObject *parent) : QObject(parent) {
    params.addQueryItem("login", vars[0]);
    params.addQueryItem("md5pass", vars[1]);
    params.addQueryItem("t", QString::number(QDateTime::currentDateTime().toTime_t()));

    request.setUrl(QUrl(vars[2]));

    data.append(params.toString());
    data.remove(0, 1);

    serverRequest(data);
}

void ParseXML::serverRequest(QByteArray param) {
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
        SLOT(slotReply(QNetworkReply*)));

    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", "QTeNeT 0.0.3");

    manager->post(request, param);
}

void ParseXML::slotReply(QNetworkReply *reply) {
    QString replyString;
    replyString = reply->readAll();
    serverReplyParse(replyString);
}

void ParseXML::serverReplyParse(QString reply) {
    QDomDocument xmlDoc;
    xmlDoc.setContent(reply);

    if(xmlDoc.elementsByTagName("result").count() > 0) {
        QString str = xmlDoc.elementsByTagName("result").at(0).toElement().text();
        emit result(tr("Response from the server: ")+str);
    } else {
        emit result(tr("Could not connect to server!"));
    }

    if(xmlDoc.elementsByTagName("saldo").count() > 0) {
        QString str = xmlDoc.elementsByTagName("saldo").at(0).toElement().text();
        QLocale::setDefault(QLocale::German);
        emit saldo(str.toDouble());
    } else {
        emit saldo(0);
    }

    if(xmlDoc.elementsByTagName("LS").count() > 0) {
        QString str = xmlDoc.elementsByTagName("LS").at(0).toElement().text();
        emit id(str);
    } else {
        emit id("-----");
    }
}

void ParseXML::updateVars(QStringList vars) {
    params.clear();
    data.clear();

    params.addQueryItem("login", vars[0]);
    params.addQueryItem("md5pass", vars[1]);
    params.addQueryItem("t", QString::number(QDateTime::currentDateTime().toTime_t()));

    request.setUrl(QUrl(vars[2]));

    data.append(params.toString());
    data.remove(0, 1);
}

void ParseXML::replayRequest() {
    emit result(tr("Connecting to server..."));
    QString login = params.queryItemValue("login");
    QString passw = params.queryItemValue("md5pass");

    params.clear();

    params.addQueryItem("login", login);
    params.addQueryItem("md5pass", passw);
    params.addQueryItem("t", QString::number(QDateTime::currentDateTime().toTime_t()));
    serverRequest(data);
}
