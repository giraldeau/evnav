#include "evnav.h"

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
