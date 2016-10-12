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
#include "evnavrequest.h"
#include "graph.h"

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

    void setChargerProvider(ChargerProvider *provider);
    ChargerProvider *getChargerProvider();
    void initGraph();

    void route(EvnavRequest &req, QJsonObject &result);
    void write(QVector<Edge> &path, QJsonObject &json);

    engine::Status computeTrip(const Coordinate &src, const Coordinate &dst, Trip &t);
    void chargerMatrix(std::function<void (Charger &, Charger &, Trip &)> cb);
    void computeDistanceHistogram(QVector<int> &hist, int bin);

    static util::Coordinate stringToCoordinates(const QString &str);
    static QString formatTime(int sec);

signals:

public slots:

private:
    void checkCachePerformance();
    void makeGraph(Graph &g, EvnavRequest &req, VertexId srcId, VertexId dstId);

    OSRM *m_osrm;
    ChargerProvider *m_provider; // FIXME: should use QSharedPointer, let's assume the object is always live
    QMap<int, QMap<int, Trip>> m_cache;

};

#endif // EVNAV_H
