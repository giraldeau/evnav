
/*
 *  Demo of electric vehicule routing using osrm
 */

#include <QDebug>

#include <iostream>

#include <osrm/osrm.hpp>
#include <osrm/status.hpp>
#include <osrm/engine_config.hpp>
#include <osrm/route_parameters.hpp>
#include <osrm/json_container.hpp>

using namespace osrm;
using namespace std;

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    EngineConfig config;
    config.storage_config = {argv[1]};
    config.use_shared_memory = false;

    qDebug() << config.storage_config.core_data_path.c_str();
    qDebug() << config.storage_config.file_index_path.c_str();
    qDebug() << config.storage_config.IsValid();
    qDebug() << config.IsValid();

    OSRM osrm{config};

    vector<pair<double, double>> coords = { { -73.57225, 45.53847 }, { -71.28751, 46.79206 } };

    RouteParameters params;
    for (std::pair<double, double> &pair: coords) {
        params.coordinates.push_back({util::FloatLongitude{pair.first}, util::FloatLatitude{pair.second}});
    }

    json::Object result;

    engine::Status status = osrm.Route(params, result);

    if (status == Status::Ok) {
        qDebug() << "ok";
        json::Array &routes = result.values["routes"].get<json::Array>();
        for (json::Value &val: routes.values) {
            auto &route = val.get<json::Object>();
            auto distance = route.values["distance"].get<json::Number>().value;
            auto duration = route.values["duration"].get<json::Number>().value;
            cout << "distance: " << distance << " meters" << endl;
            cout << "duration: " << duration << " seconds" << endl;
        }
    }

    qDebug() << "done";
    return 0;
}
