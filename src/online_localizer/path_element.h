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

#ifndef SRC_ONLINE_LOCALIZER_PATH_ELEMENT_H_
#define SRC_ONLINE_LOCALIZER_PATH_ELEMENT_H_

enum NodeState { REAL, HIDDEN };

/**
 * @brief      Data container for an element of a path. Used mainly for
 * visualization and printing out the results.
 */
class PathElement {
 public:
  PathElement() {}
  PathElement(int quId, int refId, NodeState state) {
    this->quId = quId;
    this->refId = refId;
    this->state = state;
  }
  int quId = -1;
  int refId = -1;
  NodeState state = HIDDEN;
  void print() const;
};

#endif  // SRC_ONLINE_LOCALIZER_PATH_ELEMENT_H_
