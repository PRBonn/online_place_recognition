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

#include <fstream>
#include <algorithm>
#include "non_match_cost_estimator.h"

void NonMatchCostEstimator::setFeatureType(FeatureFactory::FeatureType type) {
  _factory.setFeatureType(type);
}

/**
 * @brief      reads feature pairs from file and computes respective similarity
 * scores
 *
 * @param[in]  filename  The filename
 *
 * @return     vector of similarity scores.
 */
std::vector<double> NonMatchCostEstimator::readFromFile(
    const std::string &filename) {
  std::ifstream in(filename.c_str());
  if (!in) {
    printf("[ERROR] File %s can not be opened.\n", filename.c_str());
    exit(EXIT_FAILURE);
  }
  std::vector<double> scores;
  while (!in.eof()) {
    std::string firstMatch, secondMatch;
    in >> firstMatch >> secondMatch;
    Feature firstFeature, secondFeature;
    iFeature::Ptr firstFeaturePtr = _factory.createFeature();
    iFeature::Ptr secondFeaturePtr = _factory.createFeature();

    firstFeaturePtr->loadFromFile(firstMatch);
    secondFeaturePtr->loadFromFile(secondMatch);

    double simScore = firstFeaturePtr->computeSimilarityScore(secondFeaturePtr);
    scores.push_back(simScore);
  }
  in.close();
  return scores;
}

void NonMatchCostEstimator::process(const std::string &matchFile,
                                    const std::string &nonMatchFile) {
  std::vector<double> mScores = readFromFile(matchFile);
  std::vector<double> nmScores = readFromFile(nonMatchFile);
  printf("Match cost\n");
  for (const auto &cost : mScores) {
    printf("%f ", cost);
  }
  printf("\n");

  printf("Non match cost\n");
  for (const auto &cost : nmScores) {
    printf("%f ", cost);
  }
  printf("\n");
  double max = *(std::max_element(mScores.begin(), mScores.end()));
  double min = *(std::min_element(nmScores.begin(), nmScores.end()));
  double suggValue_low = min + 0.5 * (max - min);
  double suggValue_high = min + 0.9 * (max - min);

  printf("=== The maximum similarity value is: %f\n", max);
  printf("=== The minimum similarity value is: %f\n", min);
  printf("=== Suggest similarity value from %f to %f\n", suggValue_low,
         suggValue_high);

  iFeature::Ptr feature = _factory.createFeature();
  printf("=== Suggest non-matching cost from %f to %f\n",
         feature->score2cost(suggValue_high),
         feature->score2cost(suggValue_low));
}
