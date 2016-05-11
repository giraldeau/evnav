#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>

typedef int VertexId;
typedef int EdgeId;

/* Source:
 * Algorithms 4th Edition, Robert Sedgewick and Kevin Wayne,
 * Addison-Wesley, 2014
 */

class Edge {
public:
    Edge(VertexId u, VertexId v, double weight) :
        m_from(u), m_to(v), m_weight(weight) { }
    VertexId from() { return m_from; }
    VertexId to() { return m_to; }
    double weight() { return m_weight; }
private:
    VertexId m_from;
    VertexId m_to;
    double m_weight;
};

class Graph
{
public:
    Graph();
    int V();
    int E();
    void addEdge(Edge &e);
    QVectorIterator<Edge> adj(VertexId v);
    QVectorIterator<Edge> edges();

private:
    int m_V;
    int m_E;
    QVector<QVector<Edge> > m_adj;
};

#endif // GRAPH_H
