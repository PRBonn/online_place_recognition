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

#include "successor_manager/successor_manager.h"
#include <algorithm>
#include <cstdlib>

using std::vector;

bool SuccessorManager::setFanOut(int value) {
  if (value < 1) {
    printf("[ERROR][SuccessorManager] Invalid _fanout\n");
    return false;
  }
  _fan_out = value;
  return true;
}

bool SuccessorManager::setDatabase(iDatabase::Ptr database) {
  if (!database) {
    printf("[ERROR][SuccessorManager] Invalid database.\n");
    return false;
  }
  _database = database;
  return true;
}

bool SuccessorManager::isReady() const {
  if (!_database) {
    printf("[ERROR][SuccessorManager] Database is not set. Set it!\n");
    return false;
  }
  return true;
}

vector<Node> SuccessorManager::getSuccessors(int quId, int refId) {
  _successors.clear();
  // handle source children(quId = -1, refId == -1)
  if (quId == -1 && refId == -1) {
    printf(
        "[INFO][SuccessorManager] Requested to connect source.\nConnecting "
        "source to all images in the reference trajectory. This may take "
        "time...\n");
    for (int succ_ref = 0; succ_ref < _database->refSize(); ++succ_ref) {
      Node succ;
      double succ_cost = _database->getCost(quId + 1, succ_ref);
      succ.set(quId + 1, succ_ref, succ_cost);
      _successors.push_back(succ);
      fprintf(stderr, ".");
    }
    printf("\n");
    return _successors;
  }
  if (quId < 0 || refId < 0) {
    printf("[ERROR][SuccessorManager] Invalid  image IDs %d %d\n", quId, refId);
    exit(EXIT_FAILURE);
  }

  // check for regular succcessor
  _successors = getFanOutBasedSuccessors(quId, refId);
  return _successors;
}

vector<Node> SuccessorManager::getFanOutBasedSuccessors(int quId,
                                                        int refId) const {
  vector<Node> successors;
  int left_ref = std::max(refId - _fan_out, 0);
  int right_ref = std::min(refId + _fan_out, _database->refSize() - 1);

  for (int succ_ref = left_ref; succ_ref <= right_ref; ++succ_ref) {
    Node succ;
    double succ_cost = _database->getCost(quId + 1, succ_ref);
    succ.set(quId + 1, succ_ref, succ_cost);
    successors.push_back(succ);
  }
  return successors;
}
