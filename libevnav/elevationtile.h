#ifndef ELEVATIONTILE_H
#define ELEVATIONTILE_H

#include <QObject>
#include <QFile>

class ElevationTile : public QObject
{
    Q_OBJECT
public:
    explicit ElevationTile(QObject *parent = 0);
    ~ElevationTile();
    void load(const QString &path);
    double query(double lon, double lat);
    int width();
    int height();
    int valueAt(int x, int y);

signals:

public slots:

private:
    uchar *m_buf;
    QFile m_file;
    double m_lon;
    double m_lat;
};

#endif // ELEVATIONTILE_H
