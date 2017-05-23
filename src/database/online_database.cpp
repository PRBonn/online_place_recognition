/** online_place_recognition: a library for online matching of image sequences
** Copyright (c) 2017 O. Vysotska, C. Stachniss, University of Bonn
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
**/

#include "database/online_database.h"
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include "database/list_dir.h"
#include "features/feature.h"
#include "tools/timer/timer.h"

using std::vector;
using std::string;

/**
 * @brief      Gets the match cost.
 *
 * @param[in]  quId   The qu identifier
 * @param[in]  refId  The reference identifier
 *
 * @return     The match cost. return -1 if cost is not found
 */
double MatchMap::getMatchCost(int quId, int refId) {
  auto row_iter = _matches.find(quId);
  if (row_iter != _matches.end()) {
    // found
    auto el_iter = row_iter->second.find(refId);
    if (el_iter != row_iter->second.end()) {
      return el_iter->second;
    }
  }
  return -1.0;
}

bool OnlineDatabase::isSet() const {
  if (_quFeaturesNames.empty()) {
    printf("[ERROR][OnlineDatabase] Query features are not set\n");
    return false;
  }
  if (_refFeaturesNames.empty()) {
    printf("[ERROR][OnlineDatabase] Reference features are not set\n");
    return false;
  }
  return true;
}

double OnlineDatabase::getCost(int quId, int refId) {
  double cost = _matchMap.getMatchCost(quId, refId);
  if (cost > -1.0) {
    // cost was found
    return cost;
  }
  // printf(
  //     "[DEBUG][OnlineDatabase] Matching costs for features %d - %d will be "
  //     "computed.\n",
  //     quId, refId);
  cost = computeMatchCost(quId, refId);
  _matchMap.addMatchCost(quId, refId, cost);
  return cost;
}

void OnlineDatabase::setQuFeaturesFolder(const std::string &path2folder) {
  _quFeaturesFolder = path2folder;
  _quFeaturesNames = listDir(path2folder);
}
void OnlineDatabase::setRefFeaturesFolder(const std::string &path2folder) {
  _refFeaturesFolder = path2folder;
  _refFeaturesNames = listDir(path2folder);
}

void OnlineDatabase::setBufferSize(int size) {
  _refBuff.setBufferSize(size);
  _quBuff.setBufferSize(size);
}

void OnlineDatabase::setFeatureType(FeatureFactory::FeatureType type) {
  _featureFactory.setFeatureType(type);
}

// use for tests / visualization only
const MatchMap &OnlineDatabase::getMatchMap() const { return _matchMap; }

double OnlineDatabase::computeMatchCost(int quId, int refId) {
  // The next 2 lines are the same. Just wanted to get rid of the int-size_t
  // comparison warning
  // if (quId < 0 || quId >= _quFeaturesNames.size()) {
  if (quId < 0 || quId >= (int)_quFeaturesNames.size()) {
    printf("[ERROR][OnlineDatabase] Feature %d is out of range\n", quId);
    exit(EXIT_FAILURE);
  }
  if (refId < 0 || refId >= (int)_refFeaturesNames.size()) {
    printf("[ERROR][OnlineDatabase] Feature %d is out of range\n", refId);
    exit(EXIT_FAILURE);
  }

  iFeature::Ptr quFeaturePtr = nullptr, refFeaturePtr = nullptr;
  if (_quBuff.inBuffer(quId)) {
    quFeaturePtr = _quBuff.getFeature(quId);
  } else {
    quFeaturePtr = _featureFactory.createFeature();
    quFeaturePtr->loadFromFile(_quFeaturesFolder + _quFeaturesNames[quId]);
    _quBuff.addFeature(quId, quFeaturePtr);
  }

  if (_refBuff.inBuffer(refId)) {
    refFeaturePtr = _refBuff.getFeature(refId);
  } else {
    refFeaturePtr = _featureFactory.createFeature();
    refFeaturePtr->loadFromFile(_refFeaturesFolder + _refFeaturesNames[refId]);
    _refBuff.addFeature(refId, refFeaturePtr);
  }

  double score = quFeaturePtr->computeSimilarityScore(refFeaturePtr);
  return quFeaturePtr->score2cost(score);
}

std::string OnlineDatabase::getQuFeatureName(int id) const {
  if (id < 0 || id >= (int)_quFeaturesNames.size()) {
    printf("[WARNING][OnlineDatabase] No such feature exists\n");
    return "";
  }
  return _quFeaturesNames[id];
}

std::string OnlineDatabase::getRefFeatureName(int id) const {
  if (id < 0 || id >= (int)_refFeaturesNames.size()) {
    printf("[WARNING][OnlineDatabase] No such feature exists\n");
    return "";
  }
  return _refFeaturesNames[id];
}
