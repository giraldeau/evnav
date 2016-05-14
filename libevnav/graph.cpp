#include "graph.h"


Graph::Graph()
{
}

int Graph::V()
{
    return m_set.size();
}

int Graph::E()
{
    return m_adj.values().size();
}

void Graph::addEdge(const Edge &e)
{
    m_adj.insert(e.from(), e);
    m_set.insert(e.from());
    m_set.insert(e.to());
}

QList<Edge> Graph::adj(VertexId v)
{
    if (m_adj.contains(v)) {
        return m_adj.values(v);
    }
    return QList<Edge>{};
}

QList<Edge> Graph::edges()
{
    return m_adj.values();
}

QSet<VertexId> Graph::vertices()
{
    return m_set;
}
