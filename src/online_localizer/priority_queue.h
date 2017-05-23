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
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <vector>

class QueueElement {
 public:
  QueueElement() {}
  QueueElement(int quId, int refId, double* cost, double idvCost) {
    this->quId = quId;
    this->refId = refId;
    acc_cost = cost;
    this->idvCost = idvCost;
  }
  int quId, refId;
  double idvCost;
  double* acc_cost;
};

struct more_than_key {
  inline bool operator()(const QueueElement& lhs, const QueueElement& rhs) {
    return (*(lhs.acc_cost) > *(rhs.acc_cost));
  }
};


/**
 * @brief      Implementation of the priority queue with variable priorities
 */
class PriorityQueue {
 public:
  void push_back(const QueueElement& el);
  // gets the element with the highest priority
  QueueElement top() const;
  // deletes the element with the highest priority from the queue
  void pop();
  bool empty() const;
  void update();
  void print() const;
  std::vector<QueueElement> getElements()const {return _elements;}

 private:
  std::vector<QueueElement> _elements;
};

#endif
