#ifndef EDGE_H
#define EDGE_H

#include <QDebug>

typedef int VertexId;
typedef int EdgeId;

class Edge {
public:
    Edge() : m_from(-1), m_to(-1), m_weight(0) {}
    Edge(VertexId u, VertexId v, double weight);
    VertexId from() const { return m_from; }
    VertexId to() const { return m_to; }
    double weight() const { return m_weight; }
private:
    VertexId m_from;
    VertexId m_to;
    double m_weight;
};


QDebug operator<<(QDebug dbg, const Edge &point);


#endif // EDGE_H
