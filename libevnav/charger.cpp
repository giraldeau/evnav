#include "charger.h"

QAtomicInt Charger::m_count(0);

Charger::Charger()
{
    m_id = m_count.fetchAndAddAcquire(1);
}

void Charger::read(const QJsonObject &json)
{
    m_name = json["ParkName"].toString();
    QJsonObject tmp = json["Ports"].toArray().at(0).toObject();
    m_level = tmp["Level"].toInt();
    double lat = json["Latitude"].toDouble();
    double lng = json["Longitude"].toDouble();
    m_loc = util::Coordinate{util::FloatLongitude{lng}, util::FloatLatitude{lat}};
}

bool Charger::isValid()
{
    return (!m_name.isEmpty() && m_level != 0 && m_loc.IsValid());
}
