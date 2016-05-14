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

bool EvnavServer::handleRequest(HttpServerRequest &req,
                                HttpServerResponse &res)
{



    // parse url for src,dst
    // compute the path
    // build json response

    res.writeHead(Tufao::HttpResponseStatus::OK);
    res.headers().replace("Content-Type", "text/plain");
    res.end("Hello" + req.url().path().toUtf8());
    return true;
}

