/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/algorithms/perty/PertyOp.h>

// tbs
#include <tbs/stats/SampleStats.h>

// Qt
#include <QSet>

using namespace std;

namespace hoot
{

class PertyOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PertyOpTest);
  CPPUNIT_TEST(runDirectSequentialSimulationTest);
  //CPPUNIT_TEST(runDebugTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PertyOpTest()
    : HootTestFixture(UNUSED_PATH,
                      "test-output/rnd/perty/")
  {
  }

  void runDirectSequentialSimulationTest()
  {
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    map->setProjection(MapProjector::createAeacProjection(env));
    //OsmXmlReader reader;
    //reader.read("test-files/ToyTestA.osm", map);
    // force the map bounds.
    NodePtr n1(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 10));
    NodePtr n2(new Node(Status::Unknown1, map->createNextNodeId(), 500, 500, 10));
    map->addNode(n1);
    map->addNode(n2);

    for (double x = 0.0; x <= 500.0; x += 220)
    {
      for (double y = 0.0; y <= 500.0; y += 260)
      {
        NodePtr n(new Node(Status::Unknown1, map->createNextNodeId(), x, y, 10));
        map->addNode(n);
      }
    }

    PertyOp uut;
    uut.setSeed(1);
    uut.setCsmParameters(100);
    uut.setNamedOps(QStringList());
    uut.apply(map);

    ////
    // Handy bit for regenerating the test values, _AFTER_ it has been visually verified.
    ////
    QSet<long> nids;
    const NodeMap& nodes = map->getNodes();
    for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
      nids.insert(it->first);
    QList<long> keys = QList<long>::fromSet(nids);
    qSort(keys);

//    OsmXmlWriter writer;
//    MapProjector::reprojectToWgs84(map);
//    writer.write(map, _outputPath + "BasicTest.osm");
//    QString result = "";
//    for (int i = 0; i < keys.size(); i++)
//    {
//      const NodePtr& n = map->getNode(keys[i]);
//      result += QString("n = map->getNode(keys[%1]);\n").arg(i);
//      result += QString("CPPUNIT_ASSERT_DOUBLES_EQUAL(%1, n->getX(), 1e-3);\n").arg(n->getX());
//      result += QString("CPPUNIT_ASSERT_DOUBLES_EQUAL(%1, n->getY(), 1e-3);\n").arg(n->getY());
//    }
//    LOG_INFO(result);

    NodePtr n;
    n = map->getNode(keys[0]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(418.5, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(210.61, n->getY(), 1e-3);
    n = map->getNode(keys[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(417.754, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-5.31176, n->getY(), 1e-3);
    n = map->getNode(keys[2]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(203.892, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(179.617, n->getY(), 1e-3);
    n = map->getNode(keys[3]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(168.138, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-28.9846, n->getY(), 1e-3);
    n = map->getNode(keys[4]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-12.5047, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(257.038, n->getY(), 1e-3);
    n = map->getNode(keys[5]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-14.5567, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.34252, n->getY(), 1e-3);
    n = map->getNode(keys[6]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(534.813, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(475.291, n->getY(), 1e-3);
    n = map->getNode(keys[7]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-14.5567, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.34252, n->getY(), 1e-3);
  }

  /**
   * Runs a debug version of perty for visualizing the results. Not necessary for normal testing.
   */
  void runDebugTest()
  {
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    map->setProjection(MapProjector::createAeacProjection(env));
    //OsmXmlReader reader;
    //reader.read("test-files/ToyTestA.osm", map);
    // force the map bounds.
    NodePtr n1(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 10));
    NodePtr n2(new Node(Status::Unknown1, map->createNextNodeId(), 100, 100, 10));
    map->addNode(n1);
    map->addNode(n2);

    double gridSpacing = 2.5;

    for (double x = 0.0; x < 200.0; x += gridSpacing / 4)
    {
      for (double y = 0.0; y < 100.0; y += gridSpacing / 4)
      {
        NodePtr n(new Node(Status::Unknown1, map->createNextNodeId(), x, y, 10));
        //n->setTag("note", QString::number(n->getId()));
        map->addNode(n);
      }
    }

    OsmXmlWriter writer;

    OsmMapPtr original(new OsmMap(map));
    MapProjector::projectToWgs84(original);
    writer.write(original, _outputPath + "Original.osm");

    PertyOp uut;
    uut.setGridSpacing(gridSpacing);
    //uut.setRandomError(0.0, 0.0);
    uut.setSeed(1);
    //uut.setSystematicError(10.0, 10.0);
    //uut.setCsmParameters(9, 10);
    //uut.apply(map);
//    for (int i = 0; i < 100; i++)
//    {
//      OsmMapPtr tmp(new OsmMap(map));
//      uut.permute(tmp);
//    }
    //tbs::SampleStats ss(uut._x);
    //LOG_TRACE("sd: " << ss.calculateUnbiasedStandardDeviation());
    OsmMapPtr debug = uut.generateDebugMap(map);
    //    for (int i = 0; i < 100; i++)
    //    {
    //      OsmMapPtr tmp(new OsmMap(map));
    //      uut.permute(tmp);
    //    }
        //tbs::SampleStats ss(uut._x);
        //LOG_TRACE("sd: " << ss.calculateUnbiasedStandardDeviation


    MapProjector::projectToWgs84(debug);
    writer.write(debug, _outputPath + "Debug.osm");
    MapProjector::projectToWgs84(map);
    writer.write(map, _outputPath + "Permuted.osm");
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PertyOpTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PertyOpTest, "quick");

}