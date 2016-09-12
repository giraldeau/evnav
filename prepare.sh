#!/bin/sh

git submodule init
git submodule update

cd 3rdparty/RoutingKit
make
cd -

wget http://download.geofabrik.de/north-america/canada/quebec-latest.osm.pbf
