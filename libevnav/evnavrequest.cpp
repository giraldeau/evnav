#include "evnavrequest.h"

#include <QDebug>
#include <QUrlQuery>
#include <QRegularExpression>

#include "evnav.h"

EvnavRequest::EvnavRequest() :
    m_src(util::FloatCoordinate{}),
    m_dst(util::FloatCoordinate{}),
    m_battery(18.0),
    m_SOC_act(1.0),
    m_SOC_min(0.05),
    m_SOC_max(0.8),
    m_efficiency(0.180),
    m_power_avg(45.0),
    m_weight(1500.0)
{
}

int EvnavRequest::parseUrl(const QUrl &url)
{
    // parsing coordinates
    QRegularExpression re("/route/v1/evnav/(?<src>([\\w.-]+),([\\w.-]+));(?<dst>([\\w.-]+),([\\w.-]+))[.]*");
    qDebug() << url;
    QRegularExpressionMatch m = re.match(url.toString());
    if (m.hasMatch()) {
        m_src = Evnav::stringToCoordinates(m.captured("src"));
        m_dst = Evnav::stringToCoordinates(m.captured("dst"));
    } else {
        return -1;
    }

    // parsing options
    // FIXME: with properties, we could loop over items and set them with a single loop
    QUrlQuery query(url);
    if (query.hasQueryItem("battery")) {
        m_battery = query.queryItemValue("battery").toDouble();
    }
    if (query.hasQueryItem("SOC_act")) {
        m_SOC_act = query.queryItemValue("SOC_act").toDouble();
    }
    if (query.hasQueryItem("SOC_min")) {
        m_SOC_min = query.queryItemValue("SOC_min").toDouble();
    }
    if (query.hasQueryItem("SOC_max")) {
        m_SOC_max = query.queryItemValue("SOC_max").toDouble();
    }
    if (query.hasQueryItem("efficiency")) {
        m_efficiency = query.queryItemValue("efficiency").toDouble();
    }
    if (query.hasQueryItem("power_avg")) {
        m_power_avg = query.queryItemValue("power_avg").toDouble();
    }
    return 0;
}
