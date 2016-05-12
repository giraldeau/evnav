#include "edge.h"

Edge::Edge(VertexId u, VertexId v, double weight):
    m_from(u), m_to(v), m_weight(weight)
{

}

QDebug operator<<(QDebug debug, const Edge &c)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << c.from() << ", " << c.to() << ", " << c.weight() << ')';

    return debug;
}
