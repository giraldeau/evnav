#include "evnav.h"
#include <QElapsedTimer>

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

#include <boost/graph/adjacency_list.hpp>
using namespace boost;
typedef adjacency_list<vecS, vecS, directedS,
    no_property, property<edge_index_t, std::size_t> > Graph;

void Evnav::route(Coordinate &src, Coordinate &dst)
{

}
