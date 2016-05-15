#include "evnavserver.h"
#include <QUrl>
#include <Tufao/HttpServerRequest>
#include <Tufao/HttpServerResponse>
#include <Tufao/Headers>

#include <osrm/coordinate.hpp>

using namespace osrm;

EvnavServer::EvnavServer(QObject *parent) : QObject(parent)
{

}

void EvnavServer::setEngine(Evnav *engine)
{
    m_engine = engine;
}

bool EvnavServer::handleRequest(HttpServerRequest &req,
                                HttpServerResponse &res)
{
    // parse url for src,dst
    EvnavRequest evreq;
    QJsonObject json;

    int ret = evreq.parseUrl(req.url());
    if (ret < 0) {
        json["code"] = "error";
        json["message"] = "cannot parse url";
        goto end;
    }

    // compute the path
    m_engine->route(evreq, json);

end:
    QJsonDocument doc(json);
    res.writeHead(Tufao::HttpResponseStatus::OK);
    res.headers().replace("Content-Type", "application/json");
    res.end(doc.toJson());
    return true;
}

