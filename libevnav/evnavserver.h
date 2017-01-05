#ifndef EVNAVSERVER_H
#define EVNAVSERVER_H

#include <QObject>
#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>

#include "evnav.h"

using namespace qhttp::server;

class EvnavServer : public QObject
{
    Q_OBJECT
public:
    explicit EvnavServer(Evnav *evnav = 0, QObject *parent = 0);

signals:

public slots:
    void handleRequest(QHttpRequest *req,
                       QHttpResponse *res);
private:
    Evnav* m_engine;
};

#endif // EVNAVSERVER_H
