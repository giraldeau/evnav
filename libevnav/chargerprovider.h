#ifndef CHARGERPROVIDER_H
#define CHARGERPROVIDER_H

#include <QObject>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <osrm/coordinate.hpp>

#include <functional>
#include "edge.h"
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
	QVector<Charger> chargers() { return m_chargers; }
    Charger charger(VertexId id) { return m_chargers[id]; }


	ChargerProvider filter(std::function<bool (const Charger &)> predicate);

    auto fastChargerFilter() {
		return [](const Charger &c){ return c.m_level == 3; };
    }

signals:

public slots:


private:
	QVector<Charger> m_chargers;
};

#endif // CHARGERPROVIDER_H
