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

#include "cost_matrix_database.h"
#include <fstream>
#include <limits>

CostMatrixDatabase::CostMatrixDatabase() {}

void CostMatrixDatabase::loadFromTxt(const std::string &filename) {
  std::ifstream in(filename.c_str());
  if (!in) {
    printf("[ERROR][CostMatrixDatabase] The file cannot be opened %s\n",
           filename.c_str());
    return;
  }
  int rows, cols;
  in >> rows >> cols;
  printf("[INFO][CostMatrixDatabase] The matrix has %d rows and %d cols\n",
         rows, cols);
  // the only solution that works to reserve space for the Mat. If you know
  // better working way please let me know.
  cv::Mat tmp(rows, cols, CV_32FC1);
  _costs = tmp;

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      float value;
      in >> value;
      _costs.at<float>(r, c) = value;
    }
  }
  printf("[INFO][CostMatrixDatabase] Matrix was read\n");
  in.close();
}

void CostMatrixDatabase::loadFromTxt(const std::string &filename, int rows,
                                     int cols) {
  std::ifstream in(filename.c_str());
  if (!in) {
    printf("[ERROR][CostMatrixDatabase] The file cannot be opened %s\n",
           filename.c_str());
    return;
  }
  printf("[INFO][CostMatrixDatabase] The matrix has %d rows and %d cols\n",
         rows, cols);
  // the only solution that works to reserve space for the Mat. If you know
  // better working way please let me know.
  cv::Mat tmp(rows, cols, CV_32FC1);
  _costs = tmp;

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      float value;
      in >> value;
      _costs.at<float>(r, c) = value;
    }
  }
  printf("[INFO][CostMatrixDatabase] Matrix was read\n");
  in.close();
}

int CostMatrixDatabase::refSize() { return _costs.cols; }
double CostMatrixDatabase::getCost(int quId, int refId) {
  if (quId >= _costs.rows || quId < 0) {
    printf("[ERROR][CostMatrixDatabase] Invalid query index %d\n", quId);
    return -1;
  }
  if (refId >= _costs.cols || refId < 0) {
    printf("[ERROR][CostMatrixDatabase] Invalid query index %d\n", refId);
    return -1;
  }
  double value = _costs.at<float>(quId, refId);
  if (value < 1e-09) {
    return std::numeric_limits<double>::max();
  }
  return 1. / value;
}
