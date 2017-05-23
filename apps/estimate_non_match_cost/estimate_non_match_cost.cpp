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

#include <string>
#include "non_match_cost_estimator.h"
#include "features/feature_factory.h"

int main(int argc, char const *argv[]) {
  printf("=== Estimating the non-matching cost ====\n");
  if (argc < 3) {
    printf("[ERROR] Not enough input parameters.\n");
    printf(
        "======= Proper usage: ./estimate_non_match_cost matchFile(.txt) "
        "nonMatchFile(.txt)\n");
    return 0;
  }

  // file with corresponding features
  // file with features that should not match
  std::string matchFile, nonMatchFile;
  matchFile = argv[1];
  nonMatchFile = argv[2];

  NonMatchCostEstimator estimator;
  estimator.setFeatureType(FeatureFactory::FeatureType::CNN_Features);

  estimator.process(matchFile, nonMatchFile);
  printf("Done.\n");

  return 0;
}
