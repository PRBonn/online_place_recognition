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
#include <iostream>
#include <string>
#include "database/cost_matrix_database.h"
#include "database/online_database.h"
#include "gtest/gtest.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

TEST(MatchMap, getCost) {
  MatchMap matchMap;
  matchMap.addMatchCost(1, 2, 3.0);
  double cost = matchMap.getMatchCost(1, 2);
  EXPECT_NEAR(cost, 3.0, 1e-09);
  cost = matchMap.getMatchCost(1, 3);
  EXPECT_NEAR(cost, -1.0, 1e-09);
}

TEST(OnlineDatabase, refSize) {
  OnlineDatabase database;
  std::string path2folder = "../test/test_data/ref_features/";
  database.setRefFeaturesFolder(path2folder);
  EXPECT_EQ(4, database.refSize());
}

TEST(OnlineDatabase, getCost) {
  OnlineDatabase database;
  std::string path2ref = "../test/test_data/ref_features/";
  std::string path2qu = "../test/test_data/query_features/";
  database.setRefFeaturesFolder(path2ref);
  database.setQuFeaturesFolder(path2qu);
  database.setBufferSize(10);

  if (!database.isSet()) {
    printf("[ERROR] database is not set completely\n");
  }

  printf("Costs for %d %d is %2.5f\n", 0, 0, database.getCost(0, 0));
  printf("Costs for %d %d is %2.5f\n", 0, 1, database.getCost(0, 1));
  printf("Costs for %d %d is %2.5f\n", 0, 2, database.getCost(0, 2));
  printf("Costs for %d %d is %2.5f\n\n", 0, 3, database.getCost(0, 3));

  printf("Costs for %d %d is %2.5f\n", 1, 0, database.getCost(1, 0));
  printf("Costs for %d %d is %2.5f\n", 1, 1, database.getCost(1, 1));
  printf("Costs for %d %d is %2.5f\n", 1, 2, database.getCost(1, 2));
  printf("Costs for %d %d is %2.5f\n\n", 1, 3, database.getCost(1, 3));

  printf("Costs for %d %d is %2.5f\n", 2, 0, database.getCost(2, 0));
  printf("Costs for %d %d is %2.5f\n", 2, 1, database.getCost(2, 1));
  printf("Costs for %d %d is %2.5f\n", 2, 2, database.getCost(2, 2));
  printf("Costs for %d %d is %2.5f\n\n", 2, 3, database.getCost(2, 3));

  printf("Costs for %d %d is %2.5f\n", 3, 0, database.getCost(3, 0));
  printf("Costs for %d %d is %2.5f\n", 3, 1, database.getCost(3, 1));
  printf("Costs for %d %d is %2.5f\n", 3, 2, database.getCost(3, 2));
  printf("Costs for %d %d is %2.5f\n", 3, 3, database.getCost(3, 3));
  EXPECT_NEAR(database.getCost(0, 0), 6.68232, 1e-05);
  EXPECT_NEAR(database.getCost(0, 1), 7.31119, 1e-05);
  EXPECT_NEAR(database.getCost(0, 2), 9.22337, 1e-05);
  EXPECT_NEAR(database.getCost(0, 3), 7.31119, 1e-05);

  EXPECT_NEAR(database.getCost(2, 1), 5.88258, 1e-05);
  EXPECT_NEAR(database.getCost(3, 2), 5.79083, 1e-05);
}

class CostMatrixDatabase_TEST : public CostMatrixDatabase {
 public:
  void loadFromTxt(const std::string &filename);
  void loadFromTxt(const std::string &filename, int rows, int cols);
  // double getCost(int quId, int refId) const;
};

void CostMatrixDatabase_TEST::loadFromTxt(const std::string &filename) {
  CostMatrixDatabase::loadFromTxt(filename);
  cv::Mat costs = getCosts();
  EXPECT_EQ(costs.rows, 3);
  EXPECT_EQ(costs.cols, 5);

  cv::Mat res = (cv::Mat_<float>(3, 5) << 0.2, 0.8, 0.14, 0.9, 0.8, 0.9, 0.8,
                 0.5, 0.25, 0.7, 0.9, 0.05, 0.4, 0.4, 0.2);
  ASSERT_EQ(costs.rows, res.rows);
  ASSERT_EQ(costs.cols, res.cols);
  for (int r = 0; r < res.rows; ++r) {
    for (int c = 0; c < res.cols; ++c) {
      EXPECT_NEAR(res.at<float>(r, c), costs.at<float>(r, c), 1e-09);
    }
  }
}

void CostMatrixDatabase_TEST::loadFromTxt(const std::string &filename, int rows,
                                          int cols) {
  CostMatrixDatabase::loadFromTxt(filename, rows, cols);
  cv::Mat costs = getCosts();
  EXPECT_EQ(costs.rows, 3);
  EXPECT_EQ(costs.cols, 5);

  cv::Mat res = (cv::Mat_<float>(3, 5) << 0.2, 0.8, 0.14, 0.9, 0.8, 0.9, 0.8,
                 0.5, 0.25, 0.7, 0.0, 0.05, 0.4, 0.4, 0.2);
  ASSERT_EQ(costs.rows, res.rows);
  ASSERT_EQ(costs.cols, res.cols);
  for (int r = 0; r < res.rows; ++r) {
    for (int c = 0; c < res.cols; ++c) {
      EXPECT_NEAR(res.at<float>(r, c), costs.at<float>(r, c), 1e-09);
    }
  }
}

TEST(CostMatrixDatabase, loadFromTxt) {
  CostMatrixDatabase_TEST database;
  std::string database_file = "../test/test_data/cost_matrix.txt";
  database.loadFromTxt(database_file);
}

TEST(CostMatrixDatabase, loadFromTxtRowsCols) {
  CostMatrixDatabase_TEST database;
  std::string database_file = "../test/test_data/cost_matrix_3_5.txt";
  database.loadFromTxt(database_file, 3, 5);
}

TEST(CostMatrixDatabase, getCost) {
  CostMatrixDatabase_TEST database;
  std::string database_file = "../test/test_data/cost_matrix_3_5.txt";
  database.loadFromTxt(database_file, 3, 5);

  EXPECT_EQ(-1, database.getCost(0, -1));
  EXPECT_NEAR(5.0, database.getCost(0, 0), 1e-06);
  EXPECT_NEAR(4.0, database.getCost(1, 3), 1e-06);
  EXPECT_NEAR(std::numeric_limits<double>::max(), database.getCost(2, 0),
              1e-06);
}
