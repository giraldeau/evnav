#include <QString>
#include <QDebug>
#include <QtTest>
#include <iostream>

#include <routingkit/geo_position_to_node.h>
#include <routingkit/geo_dist.h>
#include <routingkit/permutation.h>

using namespace RoutingKit;
using namespace std;

class VpTest : public QObject
{
    Q_OBJECT

public:
    VpTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testVantagePointIndex();
    void benchmarkVantagePoint();
    void benchmarkLinearSearch();
private:
    GeoPositionToNode *m_index;
    std::vector<float> m_lat;
    std::vector<float> m_lon;
};

VpTest::VpTest()
{
}

void VpTest::initTestCase()
{
    // test performance on one million points
    int width = 1000;
    int height = 1000;
    int len = width * height;
    m_lat.resize(len);
    m_lon.resize(len);

    // generate a position matrix
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = x + y * height;
            m_lat[idx] = 1.0 * x / width;
            m_lon[idx] = 1.0 * y / height;
        }
    }

    // randomise lat/lon vectors using the same permutation
    std::random_device rd;
    std::default_random_engine en(rd());

    auto permute = random_permutation(len, en);
    apply_permutation(permute, m_lat);
    apply_permutation(permute, m_lon);

    // build the index
    m_index = new GeoPositionToNode(m_lat, m_lon);
}

void VpTest::cleanupTestCase()
{
    delete m_index;
}

void VpTest::testVantagePointIndex()
{
    int width = 8;
    int height = 8;
    int len = width * height;
    std::vector<float> lat(len);
    std::vector<float> lon(len);

    // generate a position matrix
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = x + y * height;
            lat[idx] = 0.12 * x;
            lon[idx] = 0.12 * y;
            QString str = QString("(%1, %2, %3)")
                    .arg(idx, 2)
                    .arg(lat[idx], 4)
                    .arg(lon[idx], 4);
            std::cout << str.toStdString();
        }
        std::cout << endl;
    }

    GeoPositionToNode index(lat, lon);

    qDebug() << index.point_count();

    float radius = 0.12 * 120000;
    float query_lat = 0.4;
    float query_lon = 0.4;
    auto res = index.find_nearest_neighbors_within_radius(query_lat, query_lon, radius);
    std::sort(res.begin(), res.end());

    std::vector<unsigned> exp;
    for (int i = 0; i < len; i++) {
        auto dist = geo_dist(lat[i], lon[i], query_lat, query_lon);
        if (dist <= radius)
            exp.push_back(i);
    }

    QVERIFY2(res.size() == exp.size(), "wrong number of neighbors returned");
    for (uint i = 0; i < exp.size(); i++) {
        QVERIFY2(res[i] == exp[i], "wrong neighbor");
    }

    auto print_id = [](unsigned x){ cout << x << ","; };
    std::for_each(res.begin(), res.end(), print_id);
    cout << endl;
    std::for_each(exp.begin(), exp.end(), print_id);
    cout << endl;
}

void VpTest::benchmarkVantagePoint()
{
    std::random_device rd;
    std::default_random_engine en(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    QBENCHMARK {
        float query_lat = dist(en);
        float query_lon = dist(en);
        float query_radius = 1000;
        m_index->find_nearest_neighbors_within_radius(query_lat, query_lon, query_radius);
    }
}


void VpTest::benchmarkLinearSearch()
{
    std::random_device rd;
    std::default_random_engine en(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    QBENCHMARK {
        std::vector<unsigned> res;
        float query_lat = dist(en);
        float query_lon = dist(en);
        float query_radius = 1000;
        for (uint i = 0; i < m_lat.size(); i++) {
            auto dist = geo_dist(m_lat[i], m_lon[i], query_lat, query_lon);
            if (dist <= query_radius)
                res.push_back(i);
        }
    }
}

QTEST_APPLESS_MAIN(VpTest)

#include "tst_vptest.moc"
