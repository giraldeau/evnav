#include <QElapsedTimer>
#include <QTime>

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

// http://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf
// FIXME: that should be a proper progress bar
void progress(float progress) {
    int barWidth = 20;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " \% \r";
    std::cout.flush();
}

void Evnav::initGraph()
{
    float count = 0;
    float total = m_provider.size() * m_provider.size();
    qDebug() << "pre-computing distances between chargers... " << total;
    chargerMatrix([&](Charger &a, Charger &b, Trip &t) {
        Q_UNUSED(a); Q_UNUSED(b); Q_UNUSED(t);
        count += 1;
        progress(count / total);
    });
    progress(1.0);
    std::cout << std::endl;
    qDebug() << "done";
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

QString formatTime(int sec)
{
    static const QString timeFmt("h:mm");
    QTime time(0, 0, 0);
    return time.addSecs(sec).toString(timeFmt);
}

void Evnav::route(EvnavRequest &req)
{

    Trip trip;
    Graph g;

    double SOC_dyn = req.m_SOC_max - req.m_SOC_min;
    double batt_act = req.m_battery * req.m_SOC_act; // kWh
    double batt_dyn = req.m_battery * SOC_dyn;

    if (computeTrip(req.m_src, req.m_dst, trip) == Status::Ok) {
        double e = computeEnergy(trip, req.m_efficiency);
        double e_otw = 0; // kWh
        if (e > batt_act) {
            e_otw = e - batt_act;
        }
        qDebug() << "energy required  : " << e << "kWh";
        qDebug() << "energy start     : " << batt_act << "kWh";
        qDebug() << "energy on the way: " << e_otw << "kWh";
        if (e < batt_act) {
            qDebug() << "reaching destination without charging";
            return;
        } else {
            int min_stops = std::ceil(e_otw / batt_dyn);
            double charging_time = computeChargingTime(e_otw, req.m_power_avg);
            qDebug() << "charging min_stops:" << min_stops;
            qDebug() << "charging min_time :" << formatTime(charging_time);
        }
    }

    // Add edge from the source to all chargers
    // FIXME: create waypoint class with chargers that extends it
    VertexId srcId = -1;
    VertexId dstId = -2;

    qDebug() << "source to chargers:";
    for (Charger &a : m_provider.chargers()) {
        if (computeTrip(req.m_src, a.loc(), trip) == Status::Ok) {
            double e = computeEnergy(trip, req.m_efficiency);
            if (e < batt_act) {
                //qDebug() << "can reach charger:" << a.name();
                Edge edge(srcId, a.id(), (double)trip.time_s);
                edge.m_travel_time = trip.time_s;
                edge.m_dist = trip.dist_m;
                edge.m_energy = e;
                g.addEdge(edge);
            }
        }
    }

    // Add all intermediate chargers
    //qDebug() << "intermediate chargers";
    chargerMatrix([&](Charger &a, Charger &b, Trip &t) {
        // do not add edges between chargers that are too close
        if (t.dist_m < 1000) {
            return;
        }
        double e = computeEnergy(t, req.m_efficiency);
        if (e < batt_dyn) {
            //qDebug() << "reachable charger:" << a.name() << " -> " << b.name();
            double total_time = computeTripTimeWithCharging(t, e, req.m_power_avg);
            Edge edge(a.id(), b.id(), total_time);
            edge.m_energy = e;
            edge.m_charge_time = computeChargingTime(e, req.m_power_avg);
            edge.m_dist = t.dist_m;
            edge.m_travel_time = t.time_s;
            g.addEdge(edge);
        }
    });

    // Add edge from all chargers to the destination
    //qDebug() << "chargers to destination";
    for (Charger &a : m_provider.chargers()) {
        if (computeTrip(a.loc(), req.m_dst, trip) == Status::Ok) {
            double e = computeEnergy(trip, req.m_efficiency);
            if (e < batt_dyn) {
                //qDebug() << "can reach charger:" << a.name();
                double total_time = computeTripTimeWithCharging(trip, e, req.m_power_avg);
                Edge edge(a.id(), dstId, total_time);
                edge.m_energy = e;
                edge.m_charge_time = computeChargingTime(e, req.m_power_avg);
                edge.m_dist = trip.dist_m;
                edge.m_travel_time = trip.time_s;
                g.addEdge(edge);
            }
        }
    }

    qDebug() << "graph size:" << g.E();
    // TODO: write the graph as Json

    ShortestPath sp(g, srcId);
    if (sp.hasPathTo(dstId)) {
        QVector<Edge> path = sp.pathTo(dstId).toVector();
        if (path.isEmpty()) {
            qDebug() << "error: found path, but path is empty";
            return;
        }
        qDebug() << "charging path found!";
        double energy = 0;
        double charge_time = 0;
        double travel_time = 0;
        double distance = 0;
        for (int i = 0; i < path.size(); i++) {
            Edge &e = path[i];
            QString name("---");
            double next_charge = 0;
            if (i < path.size() - 1) {
                next_charge = path[i + 1].m_charge_time;
                name = m_provider.charger(e.to()).name();
            }
            qDebug() << "step:" << i <<
                        "charge at:" << name  <<
                        "travel time:" << formatTime(e.m_travel_time) <<
                        "charge time:" << formatTime(next_charge) <<
                        "energy:" << e.m_energy;
            energy += e.m_energy;
            charge_time += e.m_charge_time;
            travel_time += e.m_travel_time;
            distance += e.m_dist;
        }
        double cost = ((charge_time / 3600.0) * 10.0);

        qDebug() << "SUMMARY:";
        qDebug() << "travel time:" << formatTime(travel_time);
        qDebug() << "charge time:" << formatTime(charge_time);
        qDebug() << "setup time:" << formatTime(computeSetupTime() * path.size() - 1);
        qDebug() << "total time:" << formatTime(sp.distTo(dstId));
        qDebug() << "number of charge:" << path.size() - 1;
        qDebug() << "energy:" << energy;
        qDebug() << "fast charger cost:" << QString("%1").arg(cost, 0, 'f', 2);

    } else {
        qDebug() << "cannot reach destination with this electric car";
    }

}
