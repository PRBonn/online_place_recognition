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

#ifndef SRC_SUCCESSOR_MANAGER_NODE_H_
#define SRC_SUCCESSOR_MANAGER_NODE_H_

#include <unordered_map>
#include <vector>

/**
 * @brief      Class for node. Stores the query id, reference id and a cost of
 * matching respective features.
 */
class Node {
 public:
  Node() {}
  Node(int quId, int refId, double idvCost);
  int quId = -1;
  int refId = -1;
  double idvCost = 0.0;
  void set(int quId, int refId, double idvCost);
  void print() const;
};

/**
 * @brief      Implementation if a set data structure to store Nodes.
 */
class NodeSet {
 public:
  using Nodes = std::unordered_map<int, std::unordered_map<int, Node> >;
  void insert(const std::vector<Node>& v);
  void clear() { _nodes.clear(); }
  int size() const { return _nodes.size(); }
  std::vector<Node> toVector() const;

  /** used for testing only **/
  const Nodes& showNodes() const { return _nodes; }

 private:
  Nodes _nodes;
};

#endif  // SRC_SUCCESSOR_MANAGER_NODE_H_
