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

#ifndef APPS_ESTIMATE_NON_MATCH_COST_NON_MATCH_COST_ESTIMATOR_H_
#define APPS_ESTIMATE_NON_MATCH_COST_NON_MATCH_COST_ESTIMATOR_H_

#include <string>
#include <vector>
#include <utility>
#include "features/feature_factory.h"
#include "features/ifeature.h"

class NonMatchCostEstimator {
 public:
  using FeaturePairs = std::vector<std::pair<iFeature::Ptr, iFeature::Ptr> >;
  void setFeatureType(FeatureFactory::FeatureType type);

  void process(const std::string &matchFile, const std::string &nonMatchFile);

 private:
  std::vector<double> readFromFile(const std::string &filename);
  FeatureFactory _factory;
};

#endif  // APPS_ESTIMATE_NON_MATCH_COST_NON_MATCH_COST_ESTIMATOR_H_
