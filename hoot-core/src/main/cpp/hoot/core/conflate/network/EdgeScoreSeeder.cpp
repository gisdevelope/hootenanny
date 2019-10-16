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
#include "EdgeScoreSeeder.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QUuid>
#include <QMutexLocker>
#include <QElapsedTimer>

namespace hoot
{

EdgeScoreSeeder::EdgeScoreSeeder() :
_startingInputSize(0),
_numEdgesProcesed(0),
_numIntersectionsProcesed(0),
_maxEdgeMatchSetFinderSteps(20)
{
}

void EdgeScoreSeeder::run()
{
  QElapsedTimer timer;
  timer.start();

  _id = QUuid::createUuid().toString();
  LOG_TRACE("Starting thread: " << _id << "...");

  _matchFinder.reset(new EdgeMatchSetFinder2(_details, _output, _n1, _n2));
  // our stubs don't need to be bidirectional since they don't create new nodes.
  _matchFinder->setAddStubsInBothDirections(false);
  _matchFinder->setIncludePartialMatches(true);
  _matchFinder->setEdgeMatchSimilarities(_edgeMatchSimilarities);
  _matchFinder->setMaxSteps(_maxEdgeMatchSetFinderSteps);

  _startingInputSize = _input->size();
  while (!_input->empty())
  {
    _inputMutex->lock();
    ConstNetworkEdgePtr edge = _input->dequeue();
    if (_input->size() % 10 == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " <<
        StringUtils::formatLargeNumber(_startingInputSize - _input->size()) <<
        " / " << StringUtils::formatLargeNumber(_startingInputSize) << " edges in: " <<
        StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
    }
    _inputMutex->unlock();

    _processEdge(edge);
    _numEdgesProcesed++;
    LOG_VART(_input->size());
  }

  LOG_VARD(_id);
  LOG_VARD(_output->getSize());
  LOG_VARD(_numEdgesProcesed);
  LOG_VARD(_numIntersectionsProcesed);
  LOG_VARD(_matchFinder->getNumSimilarEdgeMatches());
}

void EdgeScoreSeeder::_processEdge(ConstNetworkEdgePtr edge1)
{
  // find all the n2 edges that are in range of this one
  geos::geom::Envelope env = _details->getEnvelope(edge1);
  env.expandBy(_details->getSearchRadius(edge1));
  LOG_TRACE("Search Radius: " << _details->getSearchRadius(edge1));

  Tgs::IntersectionIterator iit = _createIterator(env, _edge2Index);
  while (iit.next())
  {
    ConstNetworkEdgePtr e2 = _index2Edge[iit.getId()];
    LOG_VART(edge1);
    LOG_VART(e2);

    //_schemaMutex->lock();
    const double score = _details->getPartialEdgeMatchScore(edge1, e2);
    //_schemaMutex->unlock();
    LOG_TRACE("partial edge match score:" << score);
    if (score > 0.0)
    {
      // Add all the EdgeMatches that are seeded with this edge pair.
      _outputMutex->lock();
      //_schemaMutex->lock();
      _matchFinder->addEdgeMatches(edge1, e2);
      //_schemaMutex->unlock();
      _outputMutex->unlock();
    }

    _numIntersectionsProcesed++;
  }
}

Tgs::IntersectionIterator EdgeScoreSeeder::_createIterator(geos::geom::Envelope env,
                                                           Tgs::HilbertRTreePtr tree)
{
  std::vector<double> min(2), max(2);
  min[0] = env.getMinX();
  min[1] = env.getMinY();
  max[0] = env.getMaxX();
  max[1] = env.getMaxY();
  Tgs::IntersectionIterator it(tree.get(), min, max);
  return it;
}

}
