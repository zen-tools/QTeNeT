#ifndef PARSEXML_H
#define PARSEXML_H

#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QUrl>
#include <QLocale>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>

class ParseXML : public QObject {
    Q_OBJECT
    QUrl params;
    QByteArray data;
    QNetworkRequest request;
public:
    explicit ParseXML(QObject *parent = 0);
    ParseXML(QStringList, QObject *parent = 0);
private:
    void serverRequest(QByteArray);
    void serverReplyParse(QString);

public:
    void updateVars(QStringList);
    void replayRequest();

public slots:
    void slotReply(QNetworkReply *);

signals:
    void saldo(double);
    void result(QString);
    void id(QString);
    //void finish();
public slots:

};

#endif // PARSEXML_H
