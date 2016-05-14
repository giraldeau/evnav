#include <QCoreApplication>
#include <QUrl>

#include <Tufao/HttpServer>
#include <Tufao/HttpServerRequest>
#include <Tufao/HttpServerRequestRouter>
#include <Tufao/HttpFileServer>
#include <Tufao/NotFoundHandler>
#include <Tufao/Headers>

#include "evnavserver.h"

using namespace Tufao;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    HttpServer httpd;
    EvnavServer handler;

    HttpServerRequestRouter router {
        { QRegularExpression{"^/hello"}, handler },
        { QRegularExpression{",*"}, NotFoundHandler::handler() },
    };

    QObject::connect(&httpd, &HttpServer::requestReady,
        &router, &HttpServerRequestRouter::handleRequest);

    httpd.listen(QHostAddress::Any, 8080);

    return a.exec();
}

