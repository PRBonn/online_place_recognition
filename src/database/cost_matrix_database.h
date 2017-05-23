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
#ifndef SRC_DATABASE_COST_MATRIX_DATABASE_H_
#define SRC_DATABASE_COST_MATRIX_DATABASE_H_

#include <string>
#include "idatabase.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief      Class for cost matrix database. Stores costs as matrix.
 */
class CostMatrixDatabase : public iDatabase {
 public:
  using Ptr = std::shared_ptr<CostMatrixDatabase>;
  using ConstPtr = std::shared_ptr<const CostMatrixDatabase>;

  CostMatrixDatabase();
  ~CostMatrixDatabase() {}

  int refSize() override;
  /** gets the original cost and transforms it 1/cost **/
  double getCost(int quId, int refId) override;

  /**
   * @brief      Loads a from txt. Expects specific format. First line should
   * contain number of rows and cols
   *
   * @param[in]  filename  The filename
   */
  void loadFromTxt(const std::string &filename);
  /**
   * @brief      Loads a from txt. Expects specific format. Pure matrix values.
   *
   * @param[in]  filename  The filename
   * @param[in]  rows      The rows
   * @param[in]  cols      The cols
   */
  void loadFromTxt(const std::string &filename, int rows, int cols);

  /** used for test purposes **/
  void setCosts(const cv::Mat &costs) { costs.copyTo(_costs); }
  /** returns original costs. Use for visualization and testing only **/
  cv::Mat getCosts() const { return _costs; }

 private:
  cv::Mat _costs;
};

/** \class CostMatrixDatabase
  *can store only those matrix that fit in the RAM
**/

#endif  // SRC_DATABASE_COST_MATRIX_DATABASE_H_
