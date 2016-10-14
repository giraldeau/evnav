#include <QString>
#include <QtTest>
#include <QDebug>

#include <iostream>
#include <random>
#include <queue>

#include "chargerprovider.h"

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
	void algoDevel();
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

uint find_node(GeoPositionToNode &index, pair<double, double> &coord)
{
	int radius = 1000;
	auto res = index.find_nearest_neighbor_within_radius(coord.first, coord.second, radius);
	assert(res.id != invalid_id);
	return res.id;
}

enum NodeIdType {
	GRAPH_ID = 42,
	CHARGER_ID = 43,
};

struct QueueItem {
	NodeIdType id_type;
	uint id;
	uint weight;
};

uint get_actual_distance(SimpleOSMCarRoutingGraph &graph, vector<uint> arc_path)
{
	uint dist = 0;
	for (auto a: arc_path) {
		dist += graph.geo_distance[a];
	}
	return dist;
}

void Routingkit2Test::algoDevel()
{
	ChargerProvider chargerProvider;
	QString charger_file(TOPSRCDIR "/res/chargers.json");
	chargerProvider.loadJson(charger_file);

	// map the fast chargers to ids starting from zero
	QVector<Charger> chargers = chargerProvider.chargers();
	vector<Charger> dcfc;
	auto filter = chargerProvider.fastChargerFilter();
	for (Charger &c: chargers) {
		if (filter(c)) {
			dcfc.push_back(c);
		}
	}
	for (uint i = 0; i < dcfc.size(); i++) {
		dcfc[i].m_id = i;
	}

	// TODO: use an array of ids instead of a full copy of objects
	// TODO: make a cache of the closest graph node id

	vector<float> chargersLat(dcfc.size());
	vector<float> chargersLon(dcfc.size());
	for (uint i = 0; i < dcfc.size(); i++) {
		util::FloatCoordinate loc(dcfc[i].m_loc);
		double lat = static_cast<double>(loc.lat);
		double lon = static_cast<double>(loc.lon);
		chargersLat[i] = lat;
		chargersLon[i] = lon;
	};

	GeoPositionToNode chargersIndex(chargersLat, chargersLon);

	// Load graph: m_graph
	// Make contraction: m_ch
	// Geo Position index
	GeoPositionToNode index(m_graph.latitude, m_graph.longitude);
	vector<uint> chargerToGraphNode(chargerProvider.size()); // ids are filtered

	// some chargers may be outside of the map
	for (const auto charger: dcfc) {
		util::FloatCoordinate loc(charger.m_loc);
		double lat = static_cast<double>(loc.lat);
		double lon = static_cast<double>(loc.lon);
		auto result = index.find_nearest_neighbor_within_radius(lat, lon, 1000);
		chargerToGraphNode[charger.m_id] = result.id;
	}

	// Priority queue of chargers, sorted by weight + heuristic
	auto cmp = [](QueueItem left, QueueItem right) { return left.weight < right.weight; };
	priority_queue<QueueItem, vector<QueueItem>, decltype(cmp)> queue(cmp);
	std::vector<uint> predecessor(dcfc.size());

	// Visited chargers
	QMap<bool> visited(dcfc.size());

	// Montreal-Quebec
	vector<pair<double, double>> coords = { { 45.53847, -73.57225 }, { 46.79206, -71.28751 } };
	uint src_id = find_node(index, coords[0]);
	uint dst_id = find_node(index, coords[1]);

	//push the start node in the priority queue with cost geo_dist(src, dst);
	ContractionHierarchyQuery query(m_ch);

	int range = 100000; // 100km
	query.reset().add_source(src_id).add_target(dst_id).run();
	{
		auto time = query.get_distance(); // this is travel time
		auto arc_path = query.get_arc_path();
		auto dist = get_actual_distance(m_graph, arc_path);

		qDebug() << "time:" << time;
		qDebug() << "dist:" << dist;
		queue.push({GRAPH_ID, src_id, time});
	}

	while(!queue.empty()) {
		auto item = queue.top();
		queue.pop();


		double lat, lon;
		uint current_graph_node_id;
		if (item.id_type == GRAPH_ID) {
			lat = m_graph.latitude[item.id];
			lon = m_graph.longitude[item.id];
			current_graph_node_id = item.id;
		} else if (item.id_type == CHARGER_ID) {
			Charger &c = dcfc[item.id];
			util::FloatCoordinate loc(c.m_loc);
			lat = static_cast<double>(loc.lat);
			lon = static_cast<double>(loc.lon);
			current_graph_node_id = chargerToGraphNode[c.m_id];
		}

		// add all reachable chargers in the priority queue
		auto res = chargersIndex.find_all_nodes_within_radius(lat, lon, range);
		query.reset().add_source(current_graph_node_id);
		for (const auto &r: res) {
			query.reset_target().add_target(chargerToGraphNode[r.id]).run();
			auto arc_path = query.get_arc_path();
			auto dist = get_actual_distance(m_graph, arc_path);
			if (dist > range) {
				// not reachable
				continue;
			}
			auto time = query.get_distance();
			auto f = ;
			auto g = ;
			auto cost = f + g;
			queue.push({r.id, cost});
		}


		//dist = query(current, dst);
		//if(dist < range)
		//	return path;
		// do not revisit this node
		//closedSet.add(current);

		// query neighbor chargers within optimal radius range AND not in the closedSet
		// dist_a = current-to-all chargers query
		// for each charger
			// if (dist_a > range)
				continue; // unreachable charger
			// if (not start node)
				// dist_a += charging cost + stop overhead
			// dist_b = geo_dist(charger, dist) * max_speed // heuristic
			// cost = dist_a + dist_b
			// if (charger not in pq)
				// pq.push({charger, cost})
			// else
				// peek item in pq
				// if (cost < pq.cost)
				//		update item in pq
	}
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
