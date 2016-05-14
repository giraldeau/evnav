#ifndef EVNAV_H
#define EVNAV_H

#include <QObject>
#include <QString>
#include <QMap>
#include <osrm/osrm.hpp>
#include <osrm/status.hpp>
#include <osrm/engine_config.hpp>
#include <osrm/route_parameters.hpp>
#include <osrm/json_container.hpp>

#include "chargerprovider.h"

using namespace osrm;

class Trip {
public:
    int dist_m;
    int time_s;
};

class Evnav
{
public:
    Evnav(const QString &osrm);
    ~Evnav();

    void setChargerProvider(ChargerProvider &provider) { m_provider = provider; }

    void route(Coordinate &src, Coordinate &dst);

    engine::Status computeTrip(const Coordinate &src, const Coordinate &dst, Trip &t);
    void chargerMatrix(std::function<void (Charger &, Charger &, Trip &)> cb);
    void computeDistanceHistogram(QVector<int> &hist, int bin);

    Coordinate stringToCoordinates(QString str) {
        QStringList items = str.split(",");
        double lng = items.at(0).toDouble();
        double lat = items.at(1).toDouble();
        return Coordinate{util::FloatLongitude(lng), util::FloatLatitude(lat)};
    }

signals:

public slots:

private:
    void checkCachePerformance();

    OSRM *m_osrm;
    ChargerProvider m_provider;
    QMap<int, QMap<int, Trip>> m_cache;

};

#endif // EVNAV_H
