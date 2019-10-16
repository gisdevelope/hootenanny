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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef EDGE_SCORE_SEEDER_H
#define EDGE_SCORE_SEEDER_H

// Hoot
#include <hoot/core/conflate/network/NetworkDetails.h>
#include <hoot/core/conflate/network/IndexedEdgeMatchSet.h>
#include <hoot/core/conflate/network/EdgeMatchSetFinder2.h>

// Tgs
#include <tgs/RStarTree/IntersectionIterator.h>

// Qt
#include <QRunnable>
#include <QQueue>
#include <QMutex>

namespace hoot
{

/**
 * TODO
 */
class EdgeScoreSeeder : public QRunnable
{

public:

  EdgeScoreSeeder();

  /**
   * TODO
   *
   * @see QRunnable
   */
  void run() override;

  void setNetworkDetails(NetworkDetailsPtr networkDetails) { _details = networkDetails; }
  void setNetworks(OsmNetworkPtr network1, OsmNetworkPtr network2)
  { _n1 = network1; _n2 = network2; }
  void setInput(QQueue<ConstNetworkEdgePtr>* input) { _input = input; }
  void setInputMutex(QMutex* mutex) { _inputMutex = mutex; }
  void setOutput(IndexedEdgeMatchSetPtr output) { _output = output; }
  void setOutputMutex(QMutex* mutex) { _outputMutex = mutex; }
  void setEdge2Index(Tgs::HilbertRTreePtr edge2Index) { _edge2Index = edge2Index; }
  void setIndex2Edge(const std::deque<ConstNetworkEdgePtr>& index2Edge) { _index2Edge = index2Edge; }
  void setEdgeMatchSimilarities(
    std::shared_ptr<QMap<QString, EdgeMatchSimilarity>> edgeMatchSimilarities)
  { _edgeMatchSimilarities = edgeMatchSimilarities; }
  void setMaxEdgeMatchSetFinderSteps(const int max) { _maxEdgeMatchSetFinderSteps = max; }
  void setSchemaMutex(QMutex* mutex) { _schemaMutex = mutex; }

private:

  NetworkDetailsPtr _details;
  IndexedEdgeMatchSetPtr _output;
  OsmNetworkPtr _n1, _n2;
  EdgeMatchSetFinder2Ptr _matchFinder;
  Tgs::HilbertRTreePtr _edge2Index;
  std::deque<ConstNetworkEdgePtr> _index2Edge;
  std::shared_ptr<QMap<QString, EdgeMatchSimilarity>> _edgeMatchSimilarities;

  QMutex* _inputMutex;
  QMutex* _outputMutex;
  QMutex* _schemaMutex;
  QQueue<ConstNetworkEdgePtr>* _input;

  QString _id;
  int _startingInputSize;
  int _numEdgesProcesed;
  int _numIntersectionsProcesed;
  int _maxEdgeMatchSetFinderSteps;

  void _processEdge(ConstNetworkEdgePtr edge);

  Tgs::IntersectionIterator _createIterator(geos::geom::Envelope env, Tgs::HilbertRTreePtr tree);
};

}

#endif // EDGE_SCORE_SEEDER_H
