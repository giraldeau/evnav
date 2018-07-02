#include <QDebug>
#include <QCoreApplication>
#include <QCommandLineParser>

#include <iostream>

#include <functional>
#include "evnav.h"
#include "chargerprovider.h"


#include <osrm/coordinate.hpp>

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

    QCommandLineOption srcOption("src", "source lon,lat", "src");
    parser.addOption(srcOption);

    QCommandLineOption dstOption("dst", "destination lon,lat", "dst");
    parser.addOption(dstOption);

    QCommandLineOption verboseOption("verbose", "verbose output");
    parser.addOption(verboseOption);

    parser.process(app);
    const QStringList args = parser.positionalArguments();
    if (!(parser.isSet(srcOption) && parser.isSet(dstOption))) {
        std::cout << qPrintable(parser.helpText());
        qFatal("error: requires src and dst");
    }

    Evnav evnav(args.at(0));

    ChargerProvider provider;
    provider.loadJson(args.at(1));

    ChargerProvider dcfc = provider.filter(ChargerProvider::fastChargerFilter);

    qDebug() << "chargers loaded:" << provider.size();
    qDebug() << "fast chargers:" << dcfc.size();

    if (parser.isSet(verboseOption)) {
        for (Charger &charger: dcfc.chargers()) {
            qDebug() << charger.id() << charger.name();
        }
    }

    evnav.setChargerProvider(&dcfc);
    evnav.initGraph();

    EvnavRequest req;
    QJsonObject res;
    req.m_src = evnav.stringToCoordinates(parser.value(srcOption));
    req.m_dst = evnav.stringToCoordinates(parser.value(dstOption));

    evnav.route(req, res);

    QJsonDocument doc(res);
    std::cout << doc.toJson().toStdString() << std::endl;

    return 0;
}

