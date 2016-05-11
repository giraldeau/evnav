#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>

class Graph
{
public:
    Graph();

    void addNode();
    void addEdge();

private:
    QVector<QVector<int>> m_adj;
};

#endif // GRAPH_H
