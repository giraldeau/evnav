#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include "graph.h"

class ShortestPath
{
public:
    // FIXME: the graph must exists for the lifespan of this instance.
    // convert graph to refcount for safety
    ShortestPath(Graph &graph, VertexId srcId);
    double distTo(VertexId dst);
    bool hasPathTo(VertexId dst);
    QList<Edge> pathTo(VertexId dst);

private:
    void relax(Edge &e);

    Graph& m_graph;
    VertexId m_src;
    QMap<VertexId, double> m_distTo;
    QMap<VertexId, Edge> m_edgeTo;
};

#endif // SHORTESTPATH_H
