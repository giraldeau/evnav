#include <QString>
#include <QDebug>
#include <QtTest>

#include "graph.h"

class GraphTest : public QObject
{
    Q_OBJECT

public:
    GraphTest();

private Q_SLOTS:
    void testCase1();
};

GraphTest::GraphTest()
{
}

void GraphTest::testCase1()
{
    Graph g;
    g.addEdge(Edge{1, 2, 1.0});
    g.addEdge(Edge{1, 3, 2.0});
    g.addEdge(Edge{2, 3, 3.0});
    g.addEdge(Edge{2, 1, 4.0});

    QList<Edge> lstAll = g.edges();
    QList<Edge> lstV1 = g.adj(1);
    QVERIFY2(lstAll.size() == 4, "total edges");
    QVERIFY2(lstV1.size() == 2, "edges of vertex 1");
    QVERIFY2(g.E() == 4, "total edges");
    QVERIFY2(g.V() == 3, "total vertices");
}

QTEST_APPLESS_MAIN(GraphTest)

#include "tst_graphtest.moc"
