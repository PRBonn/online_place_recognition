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
#include "features/feature_buffer.h"
#include "features/feature.h"
#include <iostream>
#include <string>
#include <vector>
#include "gtest/gtest.h"

TEST(featureBuffer, addFeature) {
  FeatureBuffer buffer;
  buffer.setBufferSize(2);
  std::vector<double> v0 = {1, 2, 3};
  std::vector<double> v1 = {4, 5, 6};
  std::vector<double> v3 = {7, 8, 9};

  Feature f0, f1, f3;
  f0.dim = v0;
  f1.dim = v1;
  f3.dim = v3;
  buffer.addFeature(0, std::make_shared<Feature>(f0));
  buffer.addFeature(1, std::make_shared<Feature>(f1));
  EXPECT_EQ(buffer.featureMap.size(), 2);
  EXPECT_EQ(buffer.ids[0], 0);
  EXPECT_EQ(buffer.ids[1], 1);

  buffer.addFeature(3, std::make_shared<Feature>(f3));
  EXPECT_EQ(buffer.featureMap.size(), 2);
  EXPECT_EQ(buffer.ids[0], 1);
  EXPECT_EQ(buffer.ids[1], 3);
}

TEST(featureBuffer, inBuffer) {
  FeatureBuffer buffer;
  buffer.setBufferSize(4);
  EXPECT_FALSE(buffer.inBuffer(4));
  std::vector<double> v1 = {4, 5, 6};
  Feature f1;
  f1.dim = v1;
  buffer.addFeature(1, std::make_shared<Feature>(f1));
  EXPECT_FALSE(buffer.inBuffer(4));
  EXPECT_TRUE(buffer.inBuffer(1));
}

TEST(featureBuffer, getFeature) {
  FeatureBuffer buffer;
  buffer.setBufferSize(4);

  std::vector<double> v1 = {4, 5, 6};
  Feature f1;
  f1.dim = v1;
  buffer.addFeature(1, std::make_shared<Feature>(f1));
  iFeature::Ptr res_feature = buffer.getFeature(1);
  Feature::Ptr resPtr = std::static_pointer_cast<Feature>(res_feature);
  EXPECT_NEAR(resPtr->dim[0], 4, 1e-09);
  EXPECT_NEAR(resPtr->dim[1], 5, 1e-09);
  EXPECT_NEAR(resPtr->dim[2], 6, 1e-09);
}
