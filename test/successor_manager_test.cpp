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

#include "successor_manager/successor_manager.h"
#include <iostream>
#include <string>
#include "database/online_database.h"
#include "gtest/gtest.h"

TEST(successorManager, getSuccessors_source) {
  std::string path2ref = "../test/test_data/ref_features/";
  std::string path2qu = "../test/test_data/query_features/";

  auto onlineDatabasePtr = OnlineDatabase::Ptr(new OnlineDatabase);
  onlineDatabasePtr->setRefFeaturesFolder(path2ref);
  onlineDatabasePtr->setQuFeaturesFolder(path2qu);
  onlineDatabasePtr->setBufferSize(10);

  if (!onlineDatabasePtr->isSet()) {
    printf("[ERROR] database is not set completely\n");
  }
  // iDatabase::Ptr databasePtr = onlineDatabasePtr;
  SuccessorManager successorManager;
  successorManager.setDatabase(onlineDatabasePtr);
  std::vector<Node> succes = successorManager.getSuccessors(-1, -1);
  // for (const Node &node : succes) {
  //   node.print();
  // }
  EXPECT_EQ(succes[0].quId, 0);
  EXPECT_EQ(succes[0].refId, 0);
  EXPECT_NEAR(succes[0].idvCost, 6.68232, 1e-05);

  EXPECT_EQ(succes[1].quId, 0);
  EXPECT_EQ(succes[1].refId, 1);
  EXPECT_NEAR(succes[1].idvCost, 7.31119, 1e-05);

  EXPECT_EQ(succes[2].quId, 0);
  EXPECT_EQ(succes[2].refId, 2);
  EXPECT_NEAR(succes[2].idvCost, 9.22337, 1e-05);

  EXPECT_EQ(succes[3].quId, 0);
  EXPECT_EQ(succes[3].refId, 3);
  EXPECT_NEAR(succes[3].idvCost, 7.31119, 1e-05);
}


TEST(successorManager, getSuccessors) {
  std::string path2ref = "../test/test_data/ref_features/";
  std::string path2qu = "../test/test_data/query_features/";
  auto onlineDatabasePtr = OnlineDatabase::Ptr(new OnlineDatabase);
  onlineDatabasePtr->setRefFeaturesFolder(path2ref);
  onlineDatabasePtr->setQuFeaturesFolder(path2qu);
  onlineDatabasePtr->setBufferSize(10);

  printf("[TEST] Database size: %d\n", onlineDatabasePtr->refSize());

  if (!onlineDatabasePtr->isSet()) {
    printf("[ERROR] database is not set completely\n");
  }
  iDatabase::Ptr database = onlineDatabasePtr;
  SuccessorManager successorManager;
  successorManager.setDatabase(database);
  successorManager.setFanOut(1);
  std::vector<Node> succes = successorManager.getSuccessors(1, 2);
  for (const Node &node : succes) {
    node.print();
  }
  ASSERT_TRUE(succes.size() > 0);
  EXPECT_EQ(succes[0].quId, 2);
  EXPECT_EQ(succes[0].refId, 1);
  EXPECT_NEAR(succes[0].idvCost, 5.88258, 1e-05);

  EXPECT_EQ(succes[1].quId, 2);
  EXPECT_EQ(succes[1].refId, 2);
  EXPECT_NEAR(succes[1].idvCost, 9.01962, 1e-05);

  EXPECT_EQ(succes[2].quId, 2);
  EXPECT_EQ(succes[2].refId, 3);
  EXPECT_NEAR(succes[2].idvCost, 5.88258, 1e-05);

  succes = successorManager.getSuccessors(0, 0);
  for (const Node &node : succes) {
    node.print();
  }
  EXPECT_EQ(succes[0].quId, 1);
  EXPECT_EQ(succes[0].refId, 0);
  EXPECT_NEAR(succes[0].idvCost, 5.18384, 1e-05);

  EXPECT_EQ(succes[1].quId, 1);
  EXPECT_EQ(succes[1].refId, 1);
  EXPECT_NEAR(succes[1].idvCost, 6.78146, 1e-05);
}
