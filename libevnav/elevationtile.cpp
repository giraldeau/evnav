#include "elevationtile.h"
#include <QFile>
#include <QFileDevice>
#include <QDebug>
#include <cmath>

ElevationTile::ElevationTile(QObject *parent) : QObject(parent)
{
}

ElevationTile::~ElevationTile()
{
    if (m_buf) {
        m_file.unmap(m_buf);
    }
}

void
ElevationTile::load(QString &path, double lon, double lat)
{
    m_file.setFileName(path);
    if (m_file.open(QFile::ReadOnly)) {
        qDebug() << "file size:" << m_file.size();
        qDebug() << "theoric file" << (2 * 1201 * 1201);
        m_buf = m_file.map(0, m_file.size());
        m_lon = lon;
        m_lat = lat;
    }
}

double
ElevationTile::query(double lon, double lat)
{
    // need to check bounds!
    // 3600 / 3 = 1200

    double temp;
    double lon_dec = std::abs(std::modf(lon, &temp));
    double lat_dec = std::abs(std::modf(lat, &temp));

    qDebug() << lon_dec << lat_dec;
    qDebug() << round(lon_dec * 1200);
    qDebug() << round(lat_dec * 1200);

    int row = 1201 - round(lat_dec * 1200) - 1;
    int col = round(lon_dec * 1200) - 1;

    qDebug() << row << col;
    int offset = (row * 1201 + col) * 2;
    int alt = m_buf[offset + 1] | m_buf[offset] << 8;
    qDebug() << "alt" << alt;
    return alt;
}
