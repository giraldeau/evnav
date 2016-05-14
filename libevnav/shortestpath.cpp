#include "shortestpath.h"

ShortestPath::ShortestPath(Graph &g, VertexId src) :
    m_graph(g), m_src(src)
{
    m_distTo[src] = 0;
    QList<VertexId> queue;
    QSet<VertexId> marked;

    marked.insert(src);
    queue.append(src);
    while(!queue.isEmpty()) {
        int v = queue.takeFirst();
        for (Edge &e : g.adj(v)) {
            relax(e);
            if (!marked.contains(e.to())) {
                marked.insert(e.to());
                queue.append(e.to());
            }
        }
    }
}

double ShortestPath::distTo(VertexId dst)
{
    if (m_distTo.contains(dst))
        return m_distTo[dst];
    return INFINITY;
}

bool ShortestPath::hasPathTo(VertexId dst)
{
    return m_distTo.contains(dst);
}

QList<Edge> ShortestPath::pathTo(VertexId dst)
{
    QList<Edge> path;
    if (hasPathTo(dst)) {
        Edge nullEdge{};
        for (Edge &e = m_edgeTo[dst];
             e != nullEdge;
             e = m_edgeTo[e.from()]) {
            path.push_front(e);
        }
    }
    return path;
}

void ShortestPath::relax(Edge &e)
{
    double old_dist = INFINITY;
    double new_dist = INFINITY;
    VertexId v = e.from();
    VertexId w = e.to();
    if (m_distTo.contains(v))
        new_dist = m_distTo[v] + e.weight();
    if (m_distTo.contains(w))
        old_dist = m_distTo[w];
    if (old_dist > new_dist) {
        m_distTo[w] = new_dist;
        m_edgeTo[w] = e;
    }
}
