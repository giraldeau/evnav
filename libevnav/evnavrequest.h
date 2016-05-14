#ifndef EVNAVREQUEST_H
#define EVNAVREQUEST_H

#include <QUrl>

#include <osrm/coordinate.hpp>

using namespace osrm;

class EvnavRequest
{
public:
    EvnavRequest();
    int parseUrl(QUrl &url);

signals:

public slots:

// FIXME: convert to properties http://doc.qt.io/qt-5/properties.html
public:
    Coordinate m_src;
    Coordinate m_dst;
    float m_battery;
    float m_SOC_act;
    float m_SOC_min;
    float m_SOC_max;
    float m_efficiency;
    float m_power_avg;
    float m_weight;
};

#endif // EVNAVREQUEST_H
