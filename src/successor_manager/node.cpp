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

#include "node.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Node::Node(int quId, int refId, double idvCost) {
  this->quId = quId;
  this->refId = refId;
  this->idvCost = idvCost;
}

void Node::set(int quId, int refId, double idvCost) {
  this->quId = quId;
  this->refId = refId;
  this->idvCost = idvCost;
}

void Node::print() const {
  printf("[INFO][Node] qu: %d; ref: %d; cost: %2.5f\n", quId, refId, idvCost);
}

void NodeSet::insert(const std::vector<Node> &v) {
  // for (int i = 0; i < v.size(); ++i) {
  for (const Node &el : v) {
    auto found_qu = _nodes.find(el.quId);
    if (found_qu == _nodes.end()) {
      // not found
      _nodes[el.quId][el.refId] = el;
    } else {
      auto found_ref = found_qu->second.find(el.refId);
      if (found_ref == found_qu->second.end()) {
        // not found
        _nodes[el.quId][el.refId] = el;
      }
    }
  }
}
std::vector<Node> NodeSet::toVector() const {
  std::vector<Node> nodesVec;
  nodesVec.reserve(_nodes.size());
  for (const auto &quIds : _nodes) {
    for (const auto &node : quIds.second) {
      nodesVec.push_back(node.second);
    }
  }
  return nodesVec;
}
