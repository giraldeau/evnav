#include <routingkit/osm_simple.h>
#include <routingkit/contraction_hierarchy.h>
#include <routingkit/inverse_vector.h>
#include <routingkit/timer.h>
#include <iostream>
#include <QtGlobal>
#include <QDebug>
#include <QFile>
#include <QElapsedTimer>
#include <QTime>

using namespace std;
using namespace RoutingKit;

void computePath(ContractionHierarchyQuery& ch_query, int from, int to,
                 function<void (unsigned /* distance */,
                                vector<unsigned>& /* path */,
                                qint64 /* elapsed */)> cb)
{
    QElapsedTimer timer;
    timer.start();
    ch_query.reset().add_source(from).add_target(to).run();
    auto distance = ch_query.get_distance();
    auto path = ch_query.get_node_path();
    qint64 elapsed = timer.nsecsElapsed() / 1000;

    cb(distance, path, elapsed);
}

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    // Load a car routing graph from OpenStreetMap-based data
    if (argc <= 1) {
        qDebug() << "specify input file";
        return -1;
    }

    auto graph = simple_load_osm_car_routing_graph_from_pbf(argv[1], [](const std::string& msg) {
        cout << msg << endl;
    });
    auto tail = invert_inverse_vector(graph.first_out);

    ContractionHierarchy ch;
    QFile ch_file(QString(argv[1]).append(".rk"));

    if (ch_file.exists() && ch_file.open(QIODevice::ReadOnly)) {
        cout << "Loading ContractionHierarchy from cache" << endl;
        ch = ContractionHierarchy::read([&](char* data, unsigned long long len) {
            ch_file.read(data, len);
        });
    } else {
        // Build the index
        ch = ContractionHierarchy::build(
            graph.node_count(),
            tail, graph.head,
            graph.travel_time
        );
        if (ch_file.open(QIODevice::WriteOnly)) {
            ch.write([&](const char* data, unsigned long long len) {
                ch_file.write(data, len);
            });
        }
    }


    // Besides the CH itself we need a query object.
    ContractionHierarchyQuery ch_query(ch);

    // Use the query object to answer queries from stdin to stdout
    //GeoPositionToNode index(graph.latitude, graph.longitude);

    int verbose = false;
    //int verbose = true;
    int samples = 1E6;
    //int samples = 1;
    qint64 sumElapsed = 0;
    qint64 sumDistance = 0;


    for (int i = 0; i < samples; i++) {
        int from = qrand() % (graph.node_count() - 1);
        int to = qrand() % (graph.node_count() - 1);
        computePath(ch_query, from, to,
            [&](unsigned distance, vector<unsigned>& path, qint64 elapsed) {
            if (verbose) {
                cout << "To get from "<< from << " to "<< to << " one needs " << distance << " seconds." << endl;
                cout << "This query was answered in " << elapsed << " microseconds." << endl;
                cout << "The path is";
                for(auto x:path)
                    cout << " " << x;
                cout << endl;
            }
            sumElapsed += elapsed;
            sumDistance += distance;
        });
    }

    cout << "Average query time: " << sumElapsed / samples << " us" << endl;
    cout << "Average distance  : " << sumDistance / (3600.0 * samples) << " h" << endl;

    return 0;
}

