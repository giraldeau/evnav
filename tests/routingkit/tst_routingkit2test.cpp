#include <QString>
#include <QtTest>
#include <QDebug>

#include <iostream>
#include <random>

#include <routingkit/osm_simple.h>
#include <routingkit/contraction_hierarchy.h>
#include <routingkit/inverse_vector.h>
#include <routingkit/geo_position_to_node.h>
#include <routingkit/permutation.h>

using namespace RoutingKit;
using namespace std;

class Routingkit2Test : public QObject
{
    Q_OBJECT

public:
    Routingkit2Test();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void benchmarkQuery();
    void benchmarkQueryExpandNode();
    void benchmarkQueryExpandEdge();
	void testNeighborQuery();

private:
    void doQuery(ContractionHierarchyQuery &query, bool expand_node, bool expand_edge);
    SimpleOSMCarRoutingGraph m_graph;
    ContractionHierarchy m_ch;
};

Routingkit2Test::Routingkit2Test()
{
}

void Routingkit2Test::initTestCase()
{
    qDebug() << "init";
    QString pbf(TOPSRCDIR "/quebec-latest.osm.pbf");
    QFile pbfFile(pbf);
    QVERIFY2(pbfFile.exists(), "Error: input file not found, run prepare.sh to download geographic data");

    m_graph = simple_load_osm_car_routing_graph_from_pbf(pbf.toStdString(), [](const std::string& msg) {
        qDebug() << msg.c_str();
    });

    QFile ch_file(pbf.append(".rk"));
    if (ch_file.exists() && ch_file.open(QIODevice::ReadOnly)) {
        qDebug() << "Loading ContractionHierarchy from cache";
        m_ch = ContractionHierarchy::read([&](char* data, unsigned long long len) {
            ch_file.read(data, len);
        });
    } else {
        // Build the index
        auto tail = invert_inverse_vector(m_graph.first_out);
        m_ch = ContractionHierarchy::build(
            m_graph.node_count(),
            tail, m_graph.head,
            m_graph.travel_time
        );
        if (ch_file.open(QIODevice::WriteOnly)) {
            m_ch.write([&](const char* data, unsigned long long len) {
                ch_file.write(data, len);
            });
        }
    }
}

void Routingkit2Test::cleanupTestCase()
{
    qDebug() << "done";
}

void Routingkit2Test::doQuery(ContractionHierarchyQuery &query, bool expand_node, bool expand_edge)
{
    int from = qrand() % (m_graph.node_count() - 1);
    int to = qrand() % (m_graph.node_count() - 1);
    query.reset().add_source(from).add_target(to).run();
    volatile auto distance = query.get_distance();
    Q_UNUSED(distance);

    if (expand_node) {
        volatile auto path = query.get_node_path();
        Q_UNUSED(path);
    }

    if (expand_edge) {
        volatile auto path = query.get_arc_path();
        Q_UNUSED(path);
    }
}

void Routingkit2Test::benchmarkQuery()
{
    ContractionHierarchyQuery query(m_ch);
    QBENCHMARK {
        doQuery(query, false, false);
    }
}

void Routingkit2Test::benchmarkQueryExpandNode()
{
    ContractionHierarchyQuery query(m_ch);
    QBENCHMARK {
        doQuery(query, true, false);
    }
}

void Routingkit2Test::benchmarkQueryExpandEdge()
{
    ContractionHierarchyQuery query(m_ch);
    QBENCHMARK {
        doQuery(query, false, true);
    }
}

void Routingkit2Test::testNeighborQuery()
{
    GeoPositionToNode index(m_graph.latitude, m_graph.longitude);
    std::random_device rd;
    std::default_random_engine en(rd());
    std::uniform_real_distribution<> dist(-0.008, 0.008); // at most ~1km

    QBENCHMARK {
        int node = qrand() % (m_graph.node_count() - 1);
        float lat = m_graph.latitude[node] + dist(en);
        float lon = m_graph.longitude[node] + dist(en);
		auto res = index.find_nearest_neighbor_within_radius(lat, lon, 1100);
        QVERIFY(res.id != invalid_id);
    }
}

QTEST_APPLESS_MAIN(Routingkit2Test)

#include "tst_routingkit2test.moc"
