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

#include "online_localizer/online_localizer.h"
#include <iostream>
#include <string>
#include "database/online_database.h"
#include "gtest/gtest.h"
#include "successor_manager/successor_manager.h"

TEST(onlineLocalizer, get_prominent_successor) {
  OnlineLocalizer localizer;
  std::vector<Node> v = {Node(1, 0, 2.0), Node(1, 1, 3.0), Node(1, 2, 1.5)};
  Node node = localizer.get_prominent_successor(v);
  EXPECT_EQ(node.quId, 1);
  EXPECT_EQ(node.refId, 2);
  EXPECT_NEAR(node.idvCost, 1.5, 1e-06);
}

TEST(onlineLocalizer, predExists) {
  std::string path2ref = "../test/test_data/ref_features/";
  std::string path2qu = "../test/test_data/query_features/";
  auto onlineDatabasePtr = OnlineDatabase::Ptr(new OnlineDatabase);
  onlineDatabasePtr->setRefFeaturesFolder(path2ref);
  onlineDatabasePtr->setQuFeaturesFolder(path2qu);
  onlineDatabasePtr->setBufferSize(10);

  if (!onlineDatabasePtr->isSet()) {
    printf("[ERROR] database is not set completely\n");
  }
  iDatabase::Ptr database = onlineDatabasePtr;
  auto successorManagerPtr = SuccessorManager::Ptr(new SuccessorManager);
  successorManagerPtr->setFanOut(1);
  successorManagerPtr->setDatabase(database);

  OnlineLocalizer localizer;
  localizer.setQuerySize(4);
  localizer.setSuccessorManager(successorManagerPtr);
  localizer.setExpansionRate(0.0);  // expand everything
  localizer.setNonMatchingCost(0.18);

  localizer.processImage(0);
  localizer.processImage(1);

  EXPECT_TRUE(localizer.predExists(Node(1, 1, 0.0)));
  EXPECT_FALSE(localizer.predExists(Node(2, 1, 0.0)));
}

TEST(onlineLocalizer, run) {
  std::string path2ref = "../test/test_data/ref_features/";
  std::string path2qu = "../test/test_data/query_features/";
  auto onlineDatabasePtr = OnlineDatabase::Ptr(new OnlineDatabase);

  onlineDatabasePtr->setRefFeaturesFolder(path2ref);
  onlineDatabasePtr->setQuFeaturesFolder(path2qu);
  onlineDatabasePtr->setBufferSize(10);

  if (!onlineDatabasePtr->isSet()) {
    printf("[ERROR] database is not set completely\n");
  }
  iDatabase::Ptr database = onlineDatabasePtr;
  auto successorManagerPtr = SuccessorManager::Ptr(new SuccessorManager);
  successorManagerPtr->setFanOut(1);
  successorManagerPtr->setDatabase(database);

  OnlineLocalizer localizer;
  localizer.setQuerySize(4);
  localizer.setSuccessorManager(successorManagerPtr);
  localizer.setExpansionRate(0.0);  // expand everything
  localizer.setNonMatchingCost(6.0);

  localizer.run();
  // // check the resulting path???
  std::vector<PathElement> path = localizer.getCurrentPath();
  // for (const auto &el : path) {
  //   el.print();
  // }
  EXPECT_TRUE(path[3].quId == 0 && path[3].refId == 0 &&
              path[3].state == HIDDEN);
  EXPECT_TRUE(path[2].quId == 1 && path[2].refId == 0 && path[2].state == REAL);
  EXPECT_TRUE(path[1].quId == 2 && path[1].refId == 1 && path[1].state == REAL);
  EXPECT_TRUE(path[0].quId == 3 && path[0].refId == 2 && path[0].state == REAL);
}

TEST(onlineLocalizer, processImage) {
  std::string path2ref = "../test/test_data/ref_features/";
  std::string path2qu = "../test/test_data/query_features/";
  auto onlineDatabasePtr = OnlineDatabase::Ptr(new OnlineDatabase);
  onlineDatabasePtr->setRefFeaturesFolder(path2ref);
  onlineDatabasePtr->setQuFeaturesFolder(path2qu);
  onlineDatabasePtr->setBufferSize(10);

  if (!onlineDatabasePtr->isSet()) {
    printf("[ERROR] database is not set completely\n");
  }

  iDatabase::Ptr database = onlineDatabasePtr;
  auto successorManagerPtr = SuccessorManager::Ptr(new SuccessorManager);
  successorManagerPtr->setFanOut(1);
  successorManagerPtr->setDatabase(database);
  // successorManagerPtr->setRelocStrategy(INDEX);

  OnlineLocalizer localizer;
  localizer.setQuerySize(4);
  localizer.setSuccessorManager(successorManagerPtr);
  localizer.setExpansionRate(0.0);  // expand everything
  localizer.setNonMatchingCost(6.0);

  localizer.processImage(0);
  // check the resulting path???
  std::vector<PathElement> path = localizer.getCurrentPath();
  for (const auto &el : path) {
    el.print();
  }
  EXPECT_TRUE(path[0].quId == 0 && path[0].refId == 0 &&
              path[0].state == HIDDEN);
  localizer.processImage(1);
  // check the resulting path???
  path = localizer.getCurrentPath();
  for (const auto &el : path) {
    el.print();
  }
  EXPECT_TRUE(path[0].quId == 1 && path[0].refId == 0 && path[0].state == REAL);
  EXPECT_TRUE(path[1].quId == 0 && path[1].refId == 0 &&
              path[1].state == HIDDEN);
}

TEST(onlineLocalizer, compute_average_path_cost) {
  std::string path2ref = "../test/test_data/ref_features/";
  std::string path2qu = "../test/test_data/query_features/";
  auto onlineDatabasePtr = OnlineDatabase::Ptr(new OnlineDatabase);
  onlineDatabasePtr->setRefFeaturesFolder(path2ref);
  onlineDatabasePtr->setQuFeaturesFolder(path2qu);
  onlineDatabasePtr->setBufferSize(10);

  if (!onlineDatabasePtr->isSet()) {
    printf("[ERROR] database is not set completely\n");
  }

  iDatabase::Ptr database = onlineDatabasePtr;
  auto successorManagerPtr = SuccessorManager::Ptr(new SuccessorManager);
  successorManagerPtr->setFanOut(1);
  successorManagerPtr->setDatabase(database);

  OnlineLocalizer localizer;
  localizer.setQuerySize(4);
  localizer.setSuccessorManager(successorManagerPtr);
  localizer.setExpansionRate(0.0);  // expand everything
  localizer.setNonMatchingCost(6.0);

  localizer.processImage(0);
  localizer.processImage(1);
  localizer.processImage(2);

  double ave_cost = localizer.compute_average_path_cost();
  EXPECT_NEAR(ave_cost, 5.91625, 1e-05);

  localizer.processImage(3);

  ave_cost = localizer.compute_average_path_cost();
  EXPECT_NEAR(ave_cost, 5.88489, 1e-05);
}

TEST(onlineLocalizer, node_worth_expanding) {
  std::string path2ref = "../test/test_data/ref_features/";
  std::string path2qu = "../test/test_data/query_features/";
  auto onlineDatabasePtr = OnlineDatabase::Ptr(new OnlineDatabase);
  onlineDatabasePtr->setRefFeaturesFolder(path2ref);
  onlineDatabasePtr->setQuFeaturesFolder(path2qu);
  onlineDatabasePtr->setBufferSize(10);

  if (!onlineDatabasePtr->isSet()) {
    printf("[ERROR] database is not set completely\n");
  }
  iDatabase::Ptr database = onlineDatabasePtr;
  auto successorManagerPtr = SuccessorManager::Ptr(new SuccessorManager);
  successorManagerPtr->setFanOut(1);
  successorManagerPtr->setDatabase(database);

  OnlineLocalizer localizer;
  localizer.setQuerySize(4);
  localizer.setSuccessorManager(successorManagerPtr);
  localizer.setExpansionRate(0.6);  // expand everything
  localizer.setNonMatchingCost(6.0);

  localizer.processImage(0);
  EXPECT_FALSE(localizer.node_worth_expanding(0, 1, 7.31119));
  EXPECT_TRUE(localizer.node_worth_expanding(0, 0, 6.68232));
}
