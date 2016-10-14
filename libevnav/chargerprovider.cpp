#include "chargerprovider.h"

#include <QDebug>
#include <QFile>

ChargerProvider::ChargerProvider()
{
}

ChargerProvider::ChargerProvider(QVector<Charger> &lst)
{
	m_chargers = lst; // shallow copy, implicitly shared
}

void ChargerProvider::read(const QJsonArray &json)
{
    m_chargers.clear();
	m_chargers.resize(json.size());
    for (int i = 0; i < json.size(); ++i) {
        QJsonObject obj = json[i].toObject();
		m_chargers[i].read(obj);
		m_chargers[i].m_id = i;
    }
}

void ChargerProvider::loadJson(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(data));
        read(doc.array());
    }
}

ChargerProvider ChargerProvider::filter(std::function<bool (const Charger &)> predicate)
{
	QVector<Charger> filtered;
	for (const Charger &c: m_chargers) {
        if (predicate(c)) {
			filtered.push_back(c);
        }
    }
    return ChargerProvider(filtered);
}
