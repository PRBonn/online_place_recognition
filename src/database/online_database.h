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

#ifndef SRC_DATABASE_ONLINE_DATABASE_H_
#define SRC_DATABASE_ONLINE_DATABASE_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "database/idatabase.h"
#include "features/feature_buffer.h"
#include "features/feature_factory.h"

/**
 * @brief      Container for storing computed feature matches
 */
class MatchMap {
 public:
  MatchMap() {}
  double getMatchCost(int quId, int refId);
  void addMatchCost(int quId, int refId, double cost) {
    _matches[quId][refId] = cost;
  }
  std::unordered_map<int, std::unordered_map<int, double> > _matches;
};

/**
 * @brief      Database for loading and matching features. Saves the computed
 * matching costs.
 */
class OnlineDatabase : public iDatabase {
 public:
  using Ptr = std::shared_ptr<OnlineDatabase>;
  using ConstPtr = std::shared_ptr<const OnlineDatabase>;

  int refSize() override { return _refFeaturesNames.size(); }
  double getCost(int quId, int refId) override;

  void setQuFeaturesFolder(const std::string &path2folder);
  void setRefFeaturesFolder(const std::string &path2folder);
  void setBufferSize(int size);
  /**
   * @brief      Sets the feature type the online database should work with. If
   * implementing your own feature, it is MANDATORY to use this function.
   * Default feature type is CNN_Features
   *
   * @param[in]  type  The type
   */
  void setFeatureType(FeatureFactory::FeatureType type);

  // use for tests / visualization only
  const MatchMap &getMatchMap() const;
  /**
   * @brief      used to test purposes.
   *
   * @param[in]  matchMap  The match map
   */
  void setMatchMap(const MatchMap &matchMap) { _matchMap = matchMap; }
  /**
   * @brief      checks if all the needed parameters are set.
   *
   * @return     True if set, False otherwise.
   */
  bool isSet() const;

  /**
   * @brief      Checks if features are in the buffer already. If not loads hte
   * features and computes the matching cost.
   *
   * @param[in]  quId   The qu identifier
   * @param[in]  refId  The reference identifier
   *
   * @return     The matching cost
   */
  double computeMatchCost(int quId, int refId);

  std::string getQuFeatureName(int id) const;
  std::string getRefFeatureName(int id) const;

 protected:
  MatchMap _matchMap;
  std::string _quFeaturesFolder = "";
  std::string _refFeaturesFolder = "";
  std::vector<std::string> _quFeaturesNames, _refFeaturesNames;
  FeatureFactory _featureFactory;

 private:
  FeatureBuffer _refBuff, _quBuff;
};

#endif  // SRC_DATABASE_ONLINE_DATABASE_H_
