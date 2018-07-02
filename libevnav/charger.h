#ifndef CHARGER_H
#define CHARGER_H

#include <QDebug>
#include <QObject>
#include <QVector>
#include <QAtomicInt>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <osrm/coordinate.hpp>

#include "edge.h"

/*
{
  "AvailableCount": null,
  "Count": 2,
  "Id": null,
  "Ids": [
    "35624178-370e-44b7-99b3-18260a970b97",
    "a497dc79-5832-43b7-988f-85d98dc159d8"
  ],
  "LatLng": {
    "Lat": 45.420233,
    "Lng": -72.724066
  },
  "Level": 0,
  "NetworkId": 2,
  "ParkName": "Zoo de Granby - Stationnement",
  "Status": 1
},
*/

using namespace osrm;

class Charger
{
public:
    Charger();
    QString name() { return m_name; }
    VertexId id() { return m_id; }
    osrm::util::Coordinate loc() { return m_loc; }
    int level() { return m_level; }
    void read(const QJsonObject &json);
    bool isValid();

private:
    VertexId m_id;
    QString m_name;
    osrm::util::Coordinate m_loc;
    int m_level;
    static QAtomicInt m_count;
};

#endif // CHARGER_H
