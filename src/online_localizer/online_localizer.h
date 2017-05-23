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
#ifndef SRC_ONLINE_LOCALIZER_ONLINE_LOCALIZER_H_
#define SRC_ONLINE_LOCALIZER_ONLINE_LOCALIZER_H_

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "online_localizer/ilocvisualizer.h"
#include "online_localizer/path_element.h"
#include "online_localizer/priority_queue.h"
#include "successor_manager/node.h"
#include "successor_manager/successor_manager.h"

/**
 * @brief      Class that performs the simultaneous expansion and search in the
 * graph.
 */
class OnlineLocalizer {
 public:
  /**
   * stores the predecessor Nodes for a given node
   */
  using PredMap = std::unordered_map<int, std::unordered_map<int, Node> >;
  /**
   * stores the accumulated costs for every node
   */
  using AccCostsMap = std::unordered_map<int, std::unordered_map<int, double> >;

  OnlineLocalizer();
  ~OnlineLocalizer() {}
  void setQuerySize(int size) { _querySize = size; }
  bool setSuccessorManager(SuccessorManager::Ptr succManager);
  /** if no visualizer is set, the localizer will not be visualized **/
  bool setVisualizer(iLocVisualizer::Ptr vis);
  bool setExpansionRate(double rate);
  bool setNonMatchingCost(double non_match);

  /**
   * @brief      dumps path to the file. Line format: quId refId status (0-
   * hidden, 1-real)
   *
   * @param[in]  filename  The filename
   */
  void printPath(const std::string &filename) const;

  bool isReady() const;
  /** loops over every image in a query sequence and calls processImage function **/
  void run();
  /**
   * @brief      Expands neccessary nodes and perfoms the update of the current best matching hypothesis for an image id qu as well as all previous images.
   *
   * @param[in]  qu    { id of the query image}
   */
  void processImage(int qu);
  /**
   * @brief      returns the full path found so far
   *
   * @return     The current path.
   */
  std::vector<PathElement> getCurrentPath() const;


  /**
   * @brief      updates the current best matching hypothesis, based on most promissing successor.
   *
   * @param[in]  successors  The successors
   */
  void updateSearch(const std::vector<Node> &successors);
  /**
   * @brief      Updates the frontier.
   *
   * @param[in]  parent      The parent
   * @param[in]  successors  The successors
   */
  void updateGraph(const QueueElement &parent,
                   const std::vector<Node> &successors);
  /**
   * @brief      selects the successor with the smallest accumulated cost
   *
   * @param[in]  successors  The successors
   *
   * @return     The prominent successor.
   */
  Node get_prominent_successor(const std::vector<Node> &successors) const;
  /** checks if for a given node there exists a predecessor, namely if the node was visited already **/
  bool predExists(const Node &node) const;
  /** Checks if the node is worth expanding, based on heuristic. Check our paper for more details **/
  bool node_worth_expanding(int quId, int refId, double acc_cost);
  double compute_average_path_cost();

  void visualize() const;

 private:
  int _querySize = 0;
  int _slidingWindowSize = 10;
  bool _needReloc = false;
  double _expansionRate = -1.0;
  double _nonMatchCost = -1.0;

  PriorityQueue _frontier;
  // stores parent for each node
  PredMap _pred;
  // stores the accumulative  cost for each node
  AccCostsMap _accCosts;
  Node _currentBestHyp;

  SuccessorManager::Ptr _successorManager = nullptr;
  iLocVisualizer::Ptr _vis = nullptr;

  NodeSet _expandedRecently;
};

/** \class OnlineLocalizer
 * \warning The graph is not stored explicitly
 * **/

#endif  // SRC_ONLINE_LOCALIZER_ONLINE_LOCALIZER_H_
