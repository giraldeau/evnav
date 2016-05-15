#ifndef EVNAVSERVER_H
#define EVNAVSERVER_H

#include <QObject>

#include <Tufao/AbstractHttpServerRequestHandler>

#include "evnav.h"

using namespace Tufao;

class EvnavServer : public QObject,
        public AbstractHttpServerRequestHandler
{
    Q_OBJECT
public:
    explicit EvnavServer(QObject *parent = 0);
    void setEngine(Evnav *engine);

signals:

public slots:
    bool handleRequest(HttpServerRequest &req,
                       HttpServerResponse &res) override;
private:
    Evnav* m_engine;
};

#endif // EVNAVSERVER_H
