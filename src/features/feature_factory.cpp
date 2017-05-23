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

#include "feature_factory.h"
#include "feature.h"
#include <cstdlib>

/**
 * @brief      Creates a feature, based on the specified type.
 *
 * @return     shared pointer to a feature
 */
// TODO(me): should return a unique_pointer!
iFeature::Ptr FeatureFactory::createFeature() {
  iFeature::Ptr featurePtr = nullptr;
  switch (_type) {
    case CNN_Features: {
      featurePtr = Feature::Ptr(new Feature);
      break;
    }
    // NEW_FEATURE:
    // case New_feature: {
      // featurePtr = New_feature::Ptr(new New_Feature);
    //   break;
    // }
    default: {
      printf("[ERROR][FeatureFactory] Unknown feature type\n");
      exit(EXIT_FAILURE);
    }
  }
  return featurePtr;
}
