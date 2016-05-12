#ifndef GRAPH_H
#define GRAPH_H

#include <QMultiMap>
#include <QSet>
#include "edge.h"

/* Source:
 * Algorithms 4th Edition, Robert Sedgewick and Kevin Wayne,
 * Addison-Wesley, 2014
 */

class Graph
{
public:
    Graph();
    int V();
    int E();
    void addEdge(const Edge &e);
    QList<Edge> adj(VertexId v);
    QList<Edge> edges();
    QSet<VertexId> vertices();

private:
    QMultiMap<VertexId, Edge> m_adj; // (from, to)
    QSet<VertexId> m_set;
};

#endif // GRAPH_H
