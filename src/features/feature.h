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
#ifndef SRC_FEATURES_FEATURE_H_
#define SRC_FEATURES_FEATURE_H_
#include <string>
#include <vector>
#include "features/ifeature.h"

/**
 * @brief      Class for feature, computed using OverFeat CNN.
 */
class Feature : public iFeature {
 public:
  using Ptr = std::shared_ptr<Feature>;
  using ConstPtr = std::shared_ptr<const Feature>;

/**
 * @brief      Loads a feature from a file. Expects feature to be stored in predefined format.
 * If you would like other format. Consider writing your own class, derived from ifeature.h
 *
 * @param[in]  filename  The filename
 */
  void loadFromFile(const std::string &filename) override;
  /**
   * @brief      computes the cosine distance between two vectors
   *
   * @param[in]  rhs   The right hand side
   *
   * @return     The similarity score.
   */
  double computeSimilarityScore(iFeature::Ptr rhs) override;
  /**
   * @brief      weight/cost is an inverse of a score.
   *
   * @param[in]  score  The score
   *
   * @return    weight/cost. If cost is near to 0, returns the
   * std::numeric_limits<double>::max()
   */
  double score2cost(double score) const override;


  /**
   * { feature vector}
   */
  std::vector<double> dim;
};

#endif  // SRC_FEATURES_FEATURE_H_
