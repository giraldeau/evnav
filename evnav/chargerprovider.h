#ifndef CHARGERPROVIDER_H
#define CHARGERPROVIDER_H

#include <QObject>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <osrm/coordinate.hpp>

#include <functional>
#include "charger.h"

using namespace osrm;

class ChargerProvider
{
public:
    ChargerProvider();
    ChargerProvider(QVector<Charger> &lst);

    void read(const QJsonArray &json);
    void loadJson(QString path);
    int size() { return m_chargers.size(); }
    QVector<Charger>& chargers() { return m_chargers; }


    ChargerProvider filter(std::function<bool (Charger &)> predicate);

    auto fastChargerFilter() {
        return [](Charger &c){ return c.level() == 3; };
    }

signals:

public slots:


private:
    QVector<Charger> m_chargers;
};

#endif // CHARGERPROVIDER_H
