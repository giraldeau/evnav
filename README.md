# EvNav: Electric Vehicule Routing

EvNav computes a charging plan for long distance travel in electric vehicle.

![EvNav Screenshot](/../master/screenshot/screenshot01.png?raw=true "Travel from Montreal to Quebec in electric car")

[Demo](https://www.youtube.com/watch?v=zXEa5IlD1LQ)

# Compiling

Requirements:
 * [Recent libosrm](https://github.com/Project-OSRM/osrm-backend)
 * [Recent Qt](http://www.qt.io/)
 * [Tufao web framework](https://github.com/vinipsmaker/tufao)

QtCreator is recommended.
 
Shadow build steps:

```
qmake -qt=qt5 <evnav source directory>
make
```

# Running

To run the program, you need a charger list. The example uses the chargers from the [Circuit Electrique](https://lecircuitelectrique.com/) and therefore requires the [Quebec OSM data](http://download.geofabrik.de/north-america/canada/quebec.html).

Perform osrm-extract and osrm-prepare on the map, as documented [here](https://github.com/Project-OSRM/osrm-backend/wiki/Running-OSRM).

The evnav program shows the result directly on the command line. The following shows an example for a trip from Montreal to Quebec city:

```
./evnav <path to quebec-latest.osrm> <path to evnav/res/chargers.json> --src=-73.57225,45.53847 --dst=-71.28751,46.79206 --verbose

chargers loaded: 585
fast chargers: 26
done
energy required  :  43.92 kWh
energy start     :  18 kWh
energy on the way:  25.92 kWh
charging min_stops: 2
charging min_time : "0:34"
graph size: 88
{
    "charging_steps": [
        {
            "charging_cost": 1.8000000715255737,
            "charging_duration": 648.00002574920654,
            "energy": 8.1000003218650818,
            "location": [
                -73.004480000000001,
                46.195441000000002
            ],
            "name": "BRCC - Aire de service de la Baie-de-Maskinongé"
        },
        {
            "charging_cost": 2.3200000921885171,
            "charging_duration": 835.20003318786621,
            "energy": 10.440000414848328,
            "location": [
                -72.554479999999998,
                46.370328000000001
            ],
            "name": "MTQ - Trois-Rivières (Cap-de-la-Madeleine)"
        },
        {
            "charging_cost": 2.4400000969568887,
            "charging_duration": 878.40003490447998,
            "energy": 10.980000436306,
            "location": [
                -71.996449999999996,
                46.639682999999998
            ],
            "name": "MTQ - Restaurant Le Chavigny - Deschambault"
        }
    ],
    "code": "Ok",
    "message": "reachable",
    "route_summary": {
        "charging_cost": 6.5600002606709795,
        "charging_duration": 2361.6000938415527,
        "distance": 257657,
        "driving_duration": 10757,
        "duration": 13118.600093841553,
        "energy": 46.080001831054688
    }
}
```

There exists a web service backend for the [evnav branch of osrm-frontend](https://github.com/giraldeau/osrm-frontend-v2/tree/evnav). The parameters are encoded in the URL and the result is returned as JSON string.

```
# Start the server
./evnav-srv <path to quebec-latest.osrm> <path to evnav/res/chargers.json>

# Make request
wget -O result.json 'http://localhost:8080/route/v1/evnav/-73.487548828125,45.62172169252446;-71.35894775390625,46.803819640791566?battery=21&SOC_act=1.0&SOC_min=0.1&SOC_max=0.8&efficiency=0.190&power_avg=33.0'
```

Supported parameters:
 * battery: usable capacity in (kWh)
 * SOC_act: State Of Charge at start
 * SOC_min: Minimum State Of Charge 
 * SOC_max: Maximum State of Charge when fast-charging
 * efficiency: energy consumption of the vehicle (Wh/km)
 * power_avg: Fast chaging average power (kW)

# Limitations and disclaimer

The algorithm is based on a simple Dijkstra shortest path. The graph is a complete digraph between all chargers, the source, and the destination. The edge weight includes the travel time, the charging time and a small overhead for the stop at the charger, which tends to minimize the total number of stops. This naive prototype won't scale well to large number of chargers.

The tool does not take into account the elevation, the weather, the battery wear of the vehicle and your own driving habits. Adjust the parameters to take into account these factors. In particular, the trip computation assumes the driver respects speed limits at all time. Driving over the speed limit, particularly on the highway, will use much more energy and reduce the range of the vehicle.

If you know the terrain is hilly, increase the SOC_min battery reserve (for instance, 20% should be enough to climb any mount in Quebec ~1000m). While driving, if the real range is below the expected range, the most effective ways to reduce energy consumption is to slow down, turn off heating and air conditioning.

Always be on the safe side, have a plan B, such as a [CAA membership](https://www.caaquebec.com/) for towing to the nearest available charger.

**THE EVNAV PROJECT IS AN EXPERIMENTAL ALPHA PROTOTYPE PROVIDED WITHOUT ANY WARRANTY. USE THE RESULTING CHARGING PLAN AT YOUR OWN RISK. YOU ARE RESPONSIBLE FOR ADJUSTING THE PLAN ACCORDING TO THE ACTUAL CHANGING ROAD CONDITIONS. IN DOUBTS, BE SAFE AND USE ANOTHER MODE OF TRANSPORTATION.**

