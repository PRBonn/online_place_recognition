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
#include <limits>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include "database/online_database.h"
#include "tools/config_parser/config_parser.h"

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("[ERROR] Not enough input parameters.\n");
    printf("Proper usage: ./create_cost_matrix config.yaml\n");
    exit(0);
  }
  printf("[INFO] Computing full cost matrix\n");
  ConfigParser config;
  config.parseYaml(argv[1]);
  config.print();

  OnlineDatabase online_database;
  online_database.setRefFeaturesFolder(config.path2ref);
  online_database.setQuFeaturesFolder(config.path2qu);

  int querySize = config.querySize;
  int refSize = online_database.refSize();
  online_database.setBufferSize(refSize);

  cv::Mat img(querySize, refSize, CV_32FC1);
  for (int qu = 0; qu < querySize; ++qu) {
    printf("Processing query img %d\n", qu);
    for (int ref = 0; ref < refSize; ++ref) {
      double cost = online_database.getCost(qu, ref);
      if (cost == std::numeric_limits<double>::max()) {
        img.at<float>(qu, ref) = cost;
      } else {
        img.at<float>(qu, ref) = 1. / cost;
      }
    }
  }

  double minVal;
  double maxVal;
  cv::Point minLoc;
  cv::Point maxLoc;

  minMaxLoc(img, &minVal, &maxVal, &minLoc, &maxLoc);
  printf("[INFO] min value: %f max value: %f\n", minVal, maxVal);

  std::string cost_txt = "cost_matrix" + std::to_string(querySize) + "_" +
                         std::to_string(refSize) + ".txt";
  std::string img_name = "cost_matrix" + std::to_string(querySize) + "_" +
                         std::to_string(refSize) + ".png";
  std::ofstream out(cost_txt);
  out << img.rows << " " << img.cols << "\n";
  for (int r = 0; r < img.rows; ++r) {
    for (int c = 0; c < img.cols; ++c) {
      out << img.at<float>(r, c) << " ";
    }
    out << "\n";
  }
  printf("The matrix was saved to the file %s\n", cost_txt.c_str());

  cv::normalize(img, img, 0, 255, cv::NORM_MINMAX, CV_32FC1);
  img.convertTo(img, CV_8UC3);
  imwrite(img_name, img);
  printf("[INFO] Image was saved\n");

  return 0;
}
