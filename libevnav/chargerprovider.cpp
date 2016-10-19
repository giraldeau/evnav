#include "chargerprovider.h"

#include <QDebug>
#include <QFile>

ChargerProvider::ChargerProvider()
{
}

ChargerProvider::ChargerProvider(QMap<VertexId, Charger> &lst)
{
    m_chargers = lst;
}

void ChargerProvider::read(const QJsonArray &json)
{
    m_chargers.clear();
    for (int i = 0; i < json.size(); ++i) {
        QJsonObject obj = json[i].toObject();
        Charger c;
        c.read(obj);
        if (c.isValid()) {
            m_chargers[c.id()] = c;
        }
    }
}

void ChargerProvider::loadJson(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject root = doc.object();
        read(root["list"].toArray());
    }
}

ChargerProvider ChargerProvider::filter(std::function<bool (Charger &)> predicate)
{
    QMap<VertexId, Charger> filtered;
    for (Charger &c: m_chargers.values()) {
        if (predicate(c)) {
            filtered[c.id()] = c;
        }
    }
    return ChargerProvider(filtered);
}
