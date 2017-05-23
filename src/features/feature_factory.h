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
#ifndef SRC_FEATURES_FEATURE_FACTORY_H_
#define SRC_FEATURES_FEATURE_FACTORY_H_

#include "features/feature.h"
#include "features/ifeature.h"

/**
 * @brief      creates shared pointers for the specified feature types.
 * Use this class, when applying own features.
 */
class FeatureFactory {
 public:
    // NEW_FEATURE: register your feature in enum
  enum FeatureType { CNN_Features};
  
  /**
   * @brief      Creates a shared pointer to a feature of previously specified FeatureType. By default generates a pointer to CNN_Features
   *
   * @return     { shared_pointer to a feature }
   */
  iFeature::Ptr createFeature();
  /**
   * @brief      Sets the feature type. Use this function, if implemented own features.
   *
   * @param[in]  type  The type
   */
  void setFeatureType(FeatureType type){ _type = type;}

 private:
  FeatureType _type = FeatureType::CNN_Features;
};

#endif  // SRC_FEATURES_FEATURE_FACTORY_H_
