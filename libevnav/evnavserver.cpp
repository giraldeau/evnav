#include "evnavserver.h"
#include <QUrl>

#include <osrm/coordinate.hpp>

using namespace osrm;

EvnavServer::EvnavServer(Evnav *evnav, QObject *parent) :
    QObject(parent), m_engine(evnav)
{
}

void EvnavServer::handleRequest(QHttpRequest *req,
                                QHttpResponse *res)
{
    // parse url for src,dst
    EvnavRequest evreq;
    QJsonObject json;

    int ret = evreq.parseUrl(req->url());
    if (ret < 0) {
        json["code"] = "error";
        json["message"] = "cannot parse url";
        goto end;
    }

    // compute the path
    if (m_engine)
        m_engine->route(evreq, json);

end:
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    res->setStatusCode(qhttp::ESTATUS_OK);
    res->addHeaderValue("content-length", data.size());
    res->addHeader("Access-Control-Allow-Headers", "X-Requested-With, Content-Type");
    res->addHeader("Access-Control-Allow-Methods", "GET");
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->addHeader("Content-Type", "application/json; charset=UTF-8");
    res->end(data);
}
