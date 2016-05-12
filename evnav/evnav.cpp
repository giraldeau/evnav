#include <QElapsedTimer>
#include "evnav.h"
#include "graph.h"
#include "shortestpath.h"

#define _USE_MATH_DEFINES
#include <cmath>

Evnav::Evnav(const QString &osrm)
{
    Q_UNUSED(osrm);
    EngineConfig config;
    config.storage_config = {osrm.toStdString()};
    config.use_shared_memory = false;

    m_osrm = new OSRM{config};

}

Evnav::~Evnav()
{
    delete m_osrm;
}
/* Source:
 * http://stackoverflow.com/questions/27928/calculate-distance-between-two-latitude-longitude-points-haversine-formula
 *
 * I would prefer to use the implementation in OSRM, but it is not exported in the public API.
 */
double deg2rad(double deg)
{
  return deg * (M_PI/180);
}

constexpr const double PRECISION = 1e-6;
const constexpr long double EARTH_RADIUS_M = 6372797.560856;

double haversineDistance(double lat1, double lon1, double lat2, double lon2)
{
  double dLat = deg2rad(lat2 - lat1);  // deg2rad below
  double dLon = deg2rad(lon2 - lon1);
  double a =
    std::sin(dLat / 2) * std::sin(dLat / 2) +
    std::cos(deg2rad(lat1)) * std::cos(deg2rad(lat2)) *
    std::sin(dLon / 2) * std::sin(dLon / 2);
  double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
  double d = EARTH_RADIUS_M * c; // Distance in km
  return d;
}

double haversineDistance(Coordinate lhs, Coordinate rhs)
{
    double lat1 = static_cast<int>(lhs.lat) * PRECISION;
    double lon1 = static_cast<int>(lhs.lon) * PRECISION;
    double lat2 = static_cast<int>(rhs.lat) * PRECISION;
    double lon2 = static_cast<int>(rhs.lon) * PRECISION;
    return haversineDistance(lat1, lon1, lat2, lon2);
}

engine::Status Evnav::computeTrip(const Coordinate &src, const Coordinate &dst, Trip &t) {
    RouteParameters p;
    json::Object result;

    p.coordinates = { src, dst };
    engine::Status status = m_osrm->Route(p, result);
    if (status == Status::Ok) {
        json::Array &routes = result.values["routes"].get<json::Array>();
        json::Value &val = routes.values.at(0);
        auto &route = val.get<json::Object>();
        auto distance = route.values["distance"].get<json::Number>().value;
        auto duration = route.values["duration"].get<json::Number>().value;
        t.dist_m = distance;
        t.time_s = duration;
    }
    return status;
}

void Evnav::chargerMatrix(std::function<void (Charger &, Charger &, Trip &)> cb)
{
    for (Charger &a : m_provider.chargers()) {
        for (Charger &b : m_provider.chargers()) {
            if (a.id() == b.id()) {
                continue;
            }
            Trip res;
            if (m_cache.contains(a.id()) &&
                    m_cache.value(a.id()).contains(b.id())) {
                res = m_cache[a.id()][b.id()];
            } else {
                engine::Status s = computeTrip(a.loc(), b.loc(), res);
                if (s == Status::Ok) {
                    m_cache[a.id()][b.id()] = res;
                }
            }
            // FIXME: should we call an error callback?
            // or throw exception?
            // or pass the status as parameter?
            cb(a, b, res);
        }
    }
}

void Evnav::computeDistanceHistogram(QVector<int> &hist, int bin)
{
    chargerMatrix([&](Charger &a, Charger &b, Trip &t){
        Q_UNUSED(a); Q_UNUSED(b);
        int index = (t.dist_m / 1000) / bin;
        index = std::min(std::max(index, 0), hist.size() - 1);
        hist[index]++;
    });
}

void Evnav::checkCachePerformance()
{
    QVector<int> hist(50);
    int bin = 10;

    QElapsedTimer timer;
    timer.start();
    computeDistanceHistogram(hist, bin);
    auto e1 = timer.nsecsElapsed();
    timer.restart();
    computeDistanceHistogram(hist, bin);
    auto e2 = timer.nsecsElapsed();
    qDebug() << "e1:" << e1 << "ms" << " e2:" << e2 << "ms";
    qDebug() << "hist:" << hist;
}

double computeEnergy(Trip &trip, double eff)
{
    // FIXME: take into account the speed
    return (trip.dist_m / 1000) * eff;         // kwh
}

double computeChargingTime(double energy, double power)
{
    return (energy / power) * 3600.0;  // s
}

double computeSetupTime()
{
    return 5 * 60.0; // 5 minutes to s
}

double computeTripTimeWithCharging(Trip &trip, double energy, double power)
{
    return trip.time_s +
            computeSetupTime() +
            computeChargingTime(energy, power);
}

void Evnav::route(Coordinate &src, Coordinate &dst)
{
    Trip trip;
    Graph g;

    double eff = 0.150;     // kWh/km
    double power = 50.0;    // kW
    double batt = 18.0;     // kWh
    double SOC_act = 0.95;
    double SOC_min = 0.05;
    double SOC_max = 0.80;
    double SOC_dyn = SOC_max - SOC_min;
    double batt_avail = batt * SOC_act; // kWh

    if (computeTrip(src, dst, trip) == Status::Ok) {
        double e = computeEnergy(trip, eff);
        double e_otw = 0; // kWh
        if (e > batt_avail) {
            e_otw = e - batt_avail;
        }
        qDebug() << "energy required  : " << e << "kWh";
        qDebug() << "energy start     : " << batt_avail << "kWh";
        qDebug() << "energy on the way: " << e_otw << "kWh";
        if (e < batt_avail) {
            qDebug() << "reaching destination without charging";
            return;
        } else {
            int min_stops = std::ceil(e_otw / (batt * SOC_dyn));
            qDebug() << "charging min_stops:" << min_stops;
            qDebug() << "charging min_time :" << computeChargingTime(e_otw, power);
        }
    }

    // Add edge from the source to all chargers
    // FIXME: create waypoint class with chargers that extends it
    VertexId srcId = -1;
    VertexId dstId = -2;

    qDebug() << "source to chargers:";
    for (Charger &a : m_provider.chargers()) {
        if (computeTrip(src, a.loc(), trip) == Status::Ok) {
            double e = computeEnergy(trip, eff);
            if (e < batt_avail) {
                qDebug() << "can reach charger:" << a.name();
                g.addEdge(Edge{srcId, a.id(), (double)trip.time_s});
            }
        }
    }

    // Add all intermediate chargers
    qDebug() << "intermediate chargers";
    chargerMatrix([&](Charger &a, Charger &b, Trip &t) {
        // do not add edges between chargers that are too close
        if (t.dist_m < 1000) {
            return;
        }
        double e = computeEnergy(t, eff);
        if (e < (batt * SOC_dyn)) {
            double charge_time = computeChargingTime(e, power);
            double total_time = computeTripTimeWithCharging(t, e, power);
            Edge edge{a.id(), b.id(), total_time};
            qDebug() << a.name() << " -> " << b.name()
                     << "distance:" << (t.dist_m / 1000.0)
                     << "travel time:" << (t.time_s / 3600.0)
                     << "charge time:" << (charge_time / 3600.0)
                     << "total time :" << (total_time / 3600.0);
            g.addEdge(edge);
        }
    });

    // Add edge from all chargers to the destination
    qDebug() << "chargers to destination";
    for (Charger &a : m_provider.chargers()) {
        if (computeTrip(a.loc(), dst, trip) == Status::Ok) {
            double e = computeEnergy(trip, eff);
            if (e < (batt * SOC_dyn)) {
                qDebug() << "can reach charger:" << a.name();
                double total_time = computeTripTimeWithCharging(trip, e, power);
                g.addEdge(Edge{a.id(), dstId, total_time});
            }
        }
    }

    qDebug() << "graph size:" << g.E();
    // TODO: write the graph as Json
    // TODO: write the path as Json

    ShortestPath sp(g, srcId);
    if (sp.hasPathTo(dstId)) {
        QVector<Edge> path = sp.pathTo(dstId).toVector();
        if (path.isEmpty()) {
            qDebug() << "error: found path, but path is empty";
            return;
        }
        qDebug() << "Awesome: charging path found!";
        qDebug() << "total time, including charging:" << sp.distTo(dstId) / 3600;
        for (int i = 0; i < path.size() - 1; i++) {
            Edge &e = path[i];
            qDebug() << "go to:" << m_provider.charger(e.to()).name() << (e.weight() / 3600);
        }
        qDebug() << "go to destination" << (path.last().weight() / 3600);

    } else {
        qDebug() << "cannot reach destination with this electric car";
    }

}
