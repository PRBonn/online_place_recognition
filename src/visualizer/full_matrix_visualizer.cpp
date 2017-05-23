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

#include "visualizer/full_matrix_visualizer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void FullMatrixVisualizer::drawPath(const std::vector<PathElement> &path) {
  _path = path;
}

void FullMatrixVisualizer::setOutImageName(const std::string &outfileName) {
  if (outfileName.empty()) {
    printf(
        "[WARNING][FullMatrixVisualizer] You are trying to set an empty "
        "filename. Ignoring.\n");
    return;
  }
  _outfileImg = outfileName;
}

void FullMatrixVisualizer::drawExpansion(NodeSet expansion) {
  // _expansion.merge(expansion);
  std::vector<Node> nodes = expansion.toVector();
  _expansion.insert(nodes);
}
void FullMatrixVisualizer::drawFrontier(const PriorityQueue &frontier) {}

void FullMatrixVisualizer::processFinished() {
  // plot everything now
  if (!_database) {
    printf("Oops. It seems like you forgot to set the database :(\n");
    return;
  }
  printf("[INFO] Plotting image...\n");
  cv::Mat costs = _database->getCosts();
  cv::cvtColor(costs, costs, CV_GRAY2BGR);
  if (costs.type() != CV_32FC3) {
    costs.convertTo(costs, CV_32FC3);
  }


  // overlay expansion
  std::vector<Node> expanded = _expansion.toVector();
  for (const auto &node : expanded) {
    if (node.quId < 0 || node.quId >= costs.rows) {
      printf("[ERROR] Query index outside the range %d\n", node.quId);
      exit(EXIT_FAILURE);
    }
    if (node.refId < 0 || node.refId >= costs.cols) {
      printf("[ERROR] Reference index outside the range %d\n", node.refId);
      exit(EXIT_FAILURE);
    }
    // green
    costs.at<cv::Vec3f>(node.quId, node.refId) = cv::Vec3f(0.0, 1.0, 0.0);
  }

  // overlay path
  for (const auto &el : _path) {
    if (el.quId < 0 || el.quId >= costs.rows) {
      printf("[ERROR] Query index outside the range %d\n", el.quId);
      exit(EXIT_FAILURE);
    }
    if (el.refId < 0 || el.refId >= costs.cols) {
      printf("[ERROR] Reference index outside the range %d\n", el.refId);
      exit(EXIT_FAILURE);
    }

    if (el.state == HIDDEN) {
      // blue
      costs.at<cv::Vec3f>(el.quId, el.refId) = cv::Vec3f(1.0, 0.0, 0.0);
    } else {
      // red
      costs.at<cv::Vec3f>(el.quId, el.refId) = cv::Vec3f(0.0, 0.0, 1.0);
    }
  }

  cv::normalize(costs, costs, 0, 255, cv::NORM_MINMAX, CV_32FC3);
  costs.convertTo(costs, CV_8UC3);
  cv::imwrite(_outfileImg.c_str(), costs);
  printf("[INFO][FullMatrixVisualizer] Image saved to a file %s\n",
         _outfileImg.c_str());
}

void FullMatrixVisualizer::setDatabase(CostMatrixDatabase::Ptr database) {
  if (!database) {
    printf("[ERROR][FullMatrixVisualizer] Invalid database\n");
    return;
  }
  _database = database;
}
