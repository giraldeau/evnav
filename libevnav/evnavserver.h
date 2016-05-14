#ifndef EVNAVSERVER_H
#define EVNAVSERVER_H

#include <QObject>

#include <Tufao/AbstractHttpServerRequestHandler>

using namespace Tufao;

class EvnavServer : public QObject,
        public AbstractHttpServerRequestHandler
{
    Q_OBJECT
public:
    explicit EvnavServer(QObject *parent = 0);
signals:

public slots:
    bool handleRequest(HttpServerRequest &req,
                       HttpServerResponse &res) override;

};

#endif // EVNAVSERVER_H
