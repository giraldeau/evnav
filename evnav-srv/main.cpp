#include <QCoreApplication>
#include <QCommandLineParser>
#include <QUrl>
#include <QDateTime>


#include <Tufao/HttpServer>
#include <Tufao/HttpServerRequest>
#include <Tufao/HttpServerRequestRouter>
#include <Tufao/HttpFileServer>
#include <Tufao/NotFoundHandler>
#include <Tufao/Headers>


#include "evnav.h"
#include "evnavserver.h"
#include "chargerprovider.h"

#include <qhttpserver.hpp>
#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>

using namespace Tufao;
using namespace qhttp::server;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setApplicationName("evnav-cli");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("electric vehicule trip planner");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("osrm", "path to osrm file");
    parser.addPositionalArgument("charger", "path to json charger file");

    parser.process(app);
    const QStringList args = parser.positionalArguments();

    if (args.size() < 2) {
        qDebug() << "missing arguments";
        parser.showHelp(1);
    }

    qDebug() << "loading street data...";
    Evnav evnav(args.at(0));

    qDebug() << "loading chargers...";
    ChargerProvider provider;
    provider.loadJson(args.at(1));
    ChargerProvider dcfc = provider.filter(provider.fastChargerFilter());
    qDebug() << "fast chargers found:" << dcfc.size();

    evnav.setChargerProvider(&dcfc);
    evnav.initGraph();

    EvnavServer handler(&evnav);
    EvnavServer *handlerPtr = &handler;

    QRegularExpression evnavUrl{"^/route/v1/evnav/.*"};
    evnavUrl.optimize();

    QHttpServer srv;
    srv.listen(QHostAddress::Any, 8080, [&](QHttpRequest *req, QHttpResponse *res) {
        QRegularExpressionMatch match = evnavUrl.match(req->url().path());
        if (match.hasMatch()) {
            req->onEnd([res, req, handlerPtr]() {
                handlerPtr->handleRequest(req, res);
            });
        } else {
            res->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
            res->end();
        }
        return;

    });

    if (!srv.isListening()) {
        fprintf(stderr, "error spawning the server\n");
        return -1;
    }

    fprintf(stdout, "server listening\n");
    return app.exec();
}

