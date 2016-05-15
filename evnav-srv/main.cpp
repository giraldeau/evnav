#include <QCoreApplication>
#include <QCommandLineParser>
#include <QUrl>

#include <Tufao/HttpServer>
#include <Tufao/HttpServerRequest>
#include <Tufao/HttpServerRequestRouter>
#include <Tufao/HttpFileServer>
#include <Tufao/NotFoundHandler>
#include <Tufao/Headers>

#include "evnav.h"
#include "evnavserver.h"
#include "chargerprovider.h"

using namespace Tufao;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

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

    HttpServer httpd;
    EvnavServer handler;
    handler.setEngine(&evnav);

    HttpServerRequestRouter router {
        { QRegularExpression{"^/hello"}, handler },
        { QRegularExpression{",*"}, NotFoundHandler::handler() },
    };

    QObject::connect(&httpd, &HttpServer::requestReady,
        &router, &HttpServerRequestRouter::handleRequest);

    httpd.listen(QHostAddress::Any, 8080);

    return a.exec();
}

