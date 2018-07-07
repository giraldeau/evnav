FROM ubuntu:bionic as buildstage

RUN apt-get update && apt-get install -y \
    curl \
    build-essential \
    git \
    qt5-default \
    cmake \
    pkg-config \
    libbz2-dev \
    libxml2-dev \
    libzip-dev \
    libboost-all-dev \
    lua5.3 \
    liblua5.3-dev \
    libtbb-dev

ARG DOCKER_TAG
RUN mkdir -p /src  && mkdir -p /opt
COPY . /src

# Build OSRM first
# The last step must not delete the sources
WORKDIR /src/3rdparty/osrm-backend
RUN NPROC=$(grep -c ^processor /proc/cpuinfo 2>/dev/null || 1) && \
    echo "Building OSRM ${DOCKER_TAG}" && \
    git show --format="%H" | head -n1 > /opt/OSRM_GITSHA && \
    echo "Building OSRM gitsha $(cat /opt/OSRM_GITSHA)" && \
    mkdir -p build && \
    cd build && \
    BUILD_TYPE="Release" && \
    ENABLE_ASSERTIONS="Off" && \
    BUILD_TOOLS="Off" && \
    case ${DOCKER_TAG} in *"-debug"*) BUILD_TYPE="Debug";; esac && \
    case ${DOCKER_TAG} in *"-assertions"*) BUILD_TYPE="RelWithDebInfo" && ENABLE_ASSERTIONS="On" && BUILD_TOOLS="On";; esac && \
    echo "Building ${BUILD_TYPE} with ENABLE_ASSERTIONS=${ENABLE_ASSERTIONS} BUILD_TOOLS=${BUILD_TOOLS}" && \
    cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DENABLE_ASSERTIONS=${ENABLE_ASSERTIONS} -DBUILD_TOOLS=${BUILD_TOOLS} -DENABLE_LTO=On && \
    make -j${NPROC} install && \
    cd ../profiles && \
    cp -r * /opt && \
    strip /usr/local/bin/*

# Build evnav
WORKDIR /src
RUN NPROC=$(grep -c ^processor /proc/cpuinfo 2>/dev/null || 1) && \
    git show --format="%H" | head -n1 > /opt/EVNAV_GITSHA && \
    echo "Building EVNAV gitsha $(cat /opt/EVNAV_GITSHA)" && \
    mkdir -p build && \
    cd build && \
    BUILD_TYPE="Release" && \
    case ${DOCKER_TAG} in *"-debug"*) BUILD_TYPE="Debug";; esac && \
    cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} && \
    make -j${NPROC} install && \
    strip /usr/local/bin/*

# Multistage build to reduce image size - https://docs.docker.com/engine/userguide/eng-image/multistage-build/#use-multi-stage-builds
# Only the content below ends up in the image, this helps remove /src from the image (which is large)
FROM ubuntu:bionic as runstage
RUN apt-get update && apt-get install -y \
    curl \
    libboost-filesystem1.65.1 \
    libboost-iostreams1.65.1 \
    libboost-program-options1.65.1 \
    libboost-regex1.65.1 \
    libboost-system1.65.1 \
    libboost-thread1.65.1 \
    libbz2-1.0 \
    libpcre3 \
    libqt5core5a \
    libqt5network5 \
    libtbb2 \
    zlib1g \
    lua5.3
RUN mkdir -p /opt
COPY --from=buildstage /usr/local /usr/local
COPY --from=buildstage /opt /opt
WORKDIR /opt

EXPOSE 5000
EXPOSE 8080
