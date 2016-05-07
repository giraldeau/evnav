#ifndef EVNAV_H
#define EVNAV_H

#include <QObject>
#include <QString>
#include <osrm/osrm.hpp>
#include <osrm/status.hpp>
#include <osrm/engine_config.hpp>
#include <osrm/route_parameters.hpp>
#include <osrm/json_container.hpp>

#include "chargerprovider.h"

using namespace osrm;

class Evnav
{
public:
    Evnav(const QString &osrm);
    ~Evnav();

    void setChargerProvider(ChargerProvider &provider) { m_provider = provider; }

    void route(Coordinate );

signals:

public slots:

private:
    OSRM *m_osrm;
    ChargerProvider m_provider;
};

#endif // EVNAV_H
