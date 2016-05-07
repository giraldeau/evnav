#include "chargerprovider.h"

#include <QDebug>
#include <QFile>

ChargerProvider::ChargerProvider()
{
}

ChargerProvider::ChargerProvider(QVector<Charger> &lst)
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
        m_chargers.append(c);
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

ChargerProvider ChargerProvider::filter(std::function<bool (Charger &)> predicate)
{
    QVector<Charger> filtered;
    for (Charger &c: m_chargers) {
        if (predicate(c)) {
            filtered.append(c);
        }
    }
    return ChargerProvider(filtered);
}
