#include "charger.h"

Charger::Charger()
{
}

void Charger::read(const QJsonObject &json)
{
    m_name = json["ParkName"].toString();
    m_level = json["Level"].toInt();
    m_network_id = json["NetworkId"].toInt();

    QJsonObject loc = json["LatLng"].toObject();
    double lat = loc["Lat"].toDouble();
    double lng = loc["Lng"].toDouble();
    m_loc = util::Coordinate{util::FloatLongitude{lng},
            util::FloatLatitude{lat}};
}
