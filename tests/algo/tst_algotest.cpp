#include <QString>
#include <QtTest>
#include <QDebug>

#include <iostream>
#include <random>

#include <routingkit/permutation.h>
#include <routingkit/id_queue.h>

#include "chargerprovider.h"

using namespace RoutingKit;

class AlgoTest : public QObject
{
	Q_OBJECT

public:
	AlgoTest();

private Q_SLOTS:
	void testPriorityQueue();
};

AlgoTest::AlgoTest()
{
}

void AlgoTest::testPriorityQueue()
{
	std::vector<unsigned> data(100);
	MinIDQueue queue(data.size());
	std::random_device rd;
	std::default_random_engine en(rd());


	for (uint i = 0; i < data.size(); i++) {
		data[i] = i;
	}

	auto permute = random_permutation(data.size(), en);

	for (uint i = 0; i < data.size(); i++) {
		queue.push({permute[i], data[permute[i]]});
	}

	while (!queue.empty()) {
		const IDKeyPair &item = queue.pop();
	}

}

QTEST_APPLESS_MAIN(AlgoTest)

#include "tst_algotest.moc"
