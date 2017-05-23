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
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include "tools/timer/timer.h"

using std::vector;
using std::string;

OnlineLocalizer::OnlineLocalizer() {
  _pred[-1][-1] = Node(-1, -1, 0.0);
  _accCosts[-1][-1] = 0.0;
  QueueElement source(-1, -1, &_accCosts[-1][-1], 0.0);
  _frontier.push_back(source);

  _currentBestHyp.set(-1, -1, 0.0);
}

bool OnlineLocalizer::setSuccessorManager(SuccessorManager::Ptr succManager) {
  if (!succManager) {
    printf("[ERROR][OnlineLocalizer] Successor manager is not set\n");
    return false;
  }
  _successorManager = succManager;
  return true;
}

bool OnlineLocalizer::setVisualizer(iLocVisualizer::Ptr vis) {
  if (!vis) {
    printf("[ERROR][OnlineLocalizer] Visualizer is not set, but wanted!\n");
    return false;
  }
  _vis = vis;
  return true;
}

bool OnlineLocalizer::setExpansionRate(double rate) {
  if (rate < 0.0) {
    printf(
        "[ERROR][OnlineLocalizer] Wrong value for the expanstion rate. The "
        "value should be in [0,1].\n");
    return false;
  }
  _expansionRate = rate;
  return true;
}

bool OnlineLocalizer::setNonMatchingCost(double non_match) {
  // if (non_match < 1.0) {
  if (non_match < 0.0) {
    printf("[ERROR][OnlineLocalizer] Invalid Matching cost\n");
    return false;
  }
  _nonMatchCost = non_match;
  return true;
}

bool OnlineLocalizer::isReady() const {
  if (!_successorManager) {
    printf("[ERROR][OnlineLocalizer] Successor manager is not set\n");
    return false;
  }
  if (_querySize == 0) {
    printf("[ERROR][OnlineLocalizer] Size of the query sequence is not set\n");
    return false;
  }
  if (_expansionRate < 0.0) {
    printf("[ERROR][OnlineLocalizer] Expansion rate is not set\n");
    return false;
  }
  if (_nonMatchCost < 0.0) {
    printf("[ERROR][OnlineLocalizer] Non matching cost is not set\n");
    return false;
  }
  // if(_slidingWindowSize < 0){
  //   printf("[ERROR][OnlineLocalizer] Sliding window size is not set\n");
  //   return false;
  // }
  return true;
}

void OnlineLocalizer::processImage(int qu) {
  bool row_reached = false;
  _expandedRecently.clear();
  printf("[DEBUG][OnlineLocalizer] Checking image %d\n", qu);
  // printf("[INFO] Qu %d frontier empty %d\n", qu, frontier.empty());
  while (!_frontier.empty() && !row_reached) {
    // take the node with the smallest accumulative cost from queue
    QueueElement expandedEl = _frontier.top();
    _frontier.pop();
    int expanded_row = expandedEl.quId;
    int expanded_col = expandedEl.refId;

    if (!node_worth_expanding(expanded_row, expanded_col,
                              *expandedEl.acc_cost)) {
      // printf("[DEBUG] Node %d %d NOT worth expanding!\n", expanded_row,
      // expanded_col);
      continue;
    }
    // printf("[DEBUG] Node %d %d worth expanding!\n", expanded_row,
    // expanded_col);
    std::vector<Node> children;
    children = _successorManager->getSuccessors(expanded_row, expanded_col);

    if (children.empty()) {
      printf(
          "[ERROR][OnlineLocalizer] Expanded node (%d, %d) has no children "
          "do not know what to do! Exit\n",
          expanded_row, expanded_col);
      exit(EXIT_FAILURE);
    } else {
      updateGraph(expandedEl, children);
      updateSearch(children);
    }

    // printf("Size of children %lu\n", children.size());
    _expandedRecently.insert(children);
    // printf("Size of expansion %d\n", _expandedRecently.toVector().size());
    // constructed graph till the current image
    // Expanding always to the expanded_row + 1
    if (expanded_row == qu - 1) {
      row_reached = true;
    }
  }  // working with frontier

  printf("[OnlineLocalizer] Matched image %d\n", qu);
  // printf("[INFO] Qu %d frontier empty %d\n", qu, frontier.empty());
  if (_frontier.empty()) {
    printf("[ERROR] Frontier is empty!\n");
    exit(EXIT_FAILURE);
  }
}

void OnlineLocalizer::run() {
  if (!isReady()) {
    printf(
        "[ERROR][OnlineLocalizer] Online Localizer is not ready to work. Check "
        "if all needed parameters are set.\n");
    exit(EXIT_FAILURE);
  }
  Timer timer;
  // for every image in the query set
  for (int qu = 0; qu < _querySize; ++qu) {
    // while the graph is not expanded till row 'qu'
    timer.start();
    processImage(qu);
    timer.stop();
    timer.print_elapsed_time(TimeExt::MSec);
    visualize();
  }
  if (_vis) {
    _vis->processFinished();
    printf("[DEBUG][OnlineLocalizer] Localization finished\n");
  }
}

bool OnlineLocalizer::node_worth_expanding(int quId, int refId,
                                           double acc_cost) {
  if (quId == -1 && refId == -1) {
    // source node-> always worth expanding
    return true;
  }
  // if it the current best hypothesis
  if (quId == _currentBestHyp.quId && refId == _currentBestHyp.refId) {
    return true;
  }

  int row_dist = _currentBestHyp.quId - quId;
  if (row_dist < 0) {
    printf(
        "[ERROR][INTERNAL][OnlineLocalizer] Trying to expand a node further in "
        "future %d than current best hypothesis %d.\n",
        quId, _currentBestHyp.quId);
    exit(EXIT_FAILURE);
  }

  double mean_cost = compute_average_path_cost();
  double potential_cost = acc_cost + row_dist * mean_cost * _expansionRate;
  if (potential_cost < _accCosts[_currentBestHyp.quId][_currentBestHyp.refId]) {
    return true;
  } else {
    return false;
  }
}

void OnlineLocalizer::updateSearch(const std::vector<Node> &successors) {
  Node possibleHyp = get_prominent_successor(successors);
  // printf("[DEBUG][OnlineLocalizer] Prominent child is: ");
  // possibleHyp.print();

  if (possibleHyp.quId > _currentBestHyp.quId) {
    _currentBestHyp = possibleHyp;
  } else if (possibleHyp.quId == _currentBestHyp.quId) {
    double accCost_current =
        _accCosts[_currentBestHyp.quId][_currentBestHyp.refId];
    double accCost_poss = _accCosts[possibleHyp.quId][possibleHyp.refId];
    if (accCost_poss <= accCost_current) {
      _currentBestHyp = possibleHyp;
    }
  }
}

Node OnlineLocalizer::get_prominent_successor(
    const std::vector<Node> &successors) const {
  double min_cost = std::numeric_limits<double>::max();
  Node minCost_node;
  for (const Node &node : successors) {
    if (node.idvCost < min_cost) {
      min_cost = node.idvCost;
      minCost_node = node;
    }
  }
  return minCost_node;
}

double OnlineLocalizer::compute_average_path_cost() {
  double mean_cost = 0;
  bool source_reached = false;
  int elInPath = 0;
  Node pred = _currentBestHyp;

  while (!source_reached) {
    if (pred.quId == -1 && pred.refId == -1) {
      source_reached = true;
      continue;
    }
    mean_cost += pred.idvCost;
    elInPath++;
    pred = _pred[pred.quId][pred.refId];
  }
  mean_cost = mean_cost / elInPath;
  return mean_cost;
}

bool OnlineLocalizer::predExists(const Node &node) const {
  auto quId_found = _pred.find(node.quId);
  if (quId_found == _pred.end()) {
    // not found
    return false;
  }
  auto node_found = quId_found->second.find(node.refId);
  if (node_found == quId_found->second.end()) {
    return false;
  }
  return true;
}

void OnlineLocalizer::updateGraph(const QueueElement &parent,
                                  const vector<Node> &successors) {
  // for every successor
  // check if the child was visited before (The child was visited if there
  // exists a predecessor for it)
  // if yes, check if the proposed accumulated cost is smaller than existing one
  // if no set a pred for a child.
  for (const Node &node : successors) {
    if (predExists(node)) {
      // child was visisted before
      double prev_accCost = _accCosts[node.quId][node.refId];
      double poss_accCost = node.idvCost + *parent.acc_cost;
      if (poss_accCost < prev_accCost) {
        printf(
            "[BINGO] You were lucky to reach this point in the code. Please "
            "save the dataset and parameters that you have used and  "
            "contact me at olga.vysotska@uni-bonn.de. I am very interested how "
            "did you do that. Cheers Olga :)\n");
        throw 42;
        // update pred; update accu_costs + update frontier.
        _pred[node.quId][node.refId] =
            Node(parent.quId, parent.refId, parent.idvCost);
        _accCosts[node.quId][node.refId] = poss_accCost;
        _frontier.update();
      }
    } else {
      // new successor
      _pred[node.quId][node.refId] =
          Node(parent.quId, parent.refId, parent.idvCost);
      _accCosts[node.quId][node.refId] = node.idvCost + *parent.acc_cost;
      QueueElement succEl(node.quId, node.refId,
                          &_accCosts[node.quId][node.refId], node.idvCost);
      _frontier.push_back(succEl);
    }
  }
}

std::vector<PathElement> OnlineLocalizer::getCurrentPath() const {
  std::vector<PathElement> path;
  bool source_reached = false;
  Node pred = _currentBestHyp;
  // pred.print();

  while (!source_reached) {
    if (pred.quId == -1 && pred.refId == -1) {
      source_reached = true;
      continue;
    }
    NodeState state = pred.idvCost > _nonMatchCost ? HIDDEN : REAL;
    PathElement pathEl(pred.quId, pred.refId, state);
    path.push_back(pathEl);
    pred = _pred.at(pred.quId).at(pred.refId);
  }
  return path;
}

/**
 * @brief      sends path + frontier to the visualizer
 */
void OnlineLocalizer::visualize() const {
  if (!_vis) {
    // visualizer is not set.
    // printf(
    //     "[WARNING][OnlineLocalizer] The visualizer is not set. Skipping "
    //     "plotting\n");
    return;
  }
  // _vis->drawFrontier(_frontier);
  _vis->drawExpansion(_expandedRecently);
  std::vector<PathElement> path = getCurrentPath();
  std::reverse(path.begin(), path.end());
  _vis->drawPath(path);
}

void OnlineLocalizer::printPath(const std::string &filename) const {
  std::ofstream out(filename.c_str());
  if (!out) {
    printf("[ERROR][OnlineLocalizer] Couldn't open the file %s\n",
           filename.c_str());
    printf("======================= The path is NOT saved\n");
    return;
  }
  std::vector<PathElement> path = getCurrentPath();
  for (const PathElement &el : path) {
    out << el.quId << " " << el.refId << " ";
    out << (el.state == NodeState::HIDDEN ? 0 : 1) << "\n";
  }
  out.close();
  printf("[INFO][OnlineLocalizer] Found path was written to %s\n",
         filename.c_str());
}
