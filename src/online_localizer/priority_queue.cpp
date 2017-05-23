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
#include "priority_queue.h"
#include <algorithm>
#include <iostream>

using std::vector;

void PriorityQueue::push_back(const QueueElement &el) {
  _elements.push_back(el);
  std::sort(_elements.begin(), _elements.end(), more_than_key());
}

void PriorityQueue::print() const {
  std::cout << "Elements of the priority queue: \n";
  for (size_t i = 0; i < _elements.size(); ++i) {
    std::cout << i << ". " << *_elements[i].acc_cost << "\t"
              << _elements[i].quId << "\t" << _elements[i].refId << std::endl;
  }
}

void PriorityQueue::update() {
  std::sort(_elements.begin(), _elements.end(), more_than_key());
}

QueueElement PriorityQueue::top() const { return _elements.back(); }

void PriorityQueue::pop() { return _elements.pop_back(); }
bool PriorityQueue::empty() const { return _elements.empty(); }
