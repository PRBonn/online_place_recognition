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

#ifndef SRC_SUCCESSOR_MANAGER_SUCCESSOR_MANAGER_H_
#define SRC_SUCCESSOR_MANAGER_SUCCESSOR_MANAGER_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "database/idatabase.h"
#include "successor_manager/node.h"

/**
 * @brief      Class that manages the successors (children) of individual nodes.
 */
class SuccessorManager {
 public:
  using Ptr = std::shared_ptr<SuccessorManager>;
  using ConstPtr = std::shared_ptr<const SuccessorManager>;

  SuccessorManager() {}
  ~SuccessorManager() {}

  bool setFanOut(int value);
  bool setDatabase(iDatabase::Ptr database);

  /**
   * @brief      checks if all the neccessary parameters are set.
   *
   * @return     True if ready, False otherwise.
   */
  bool isReady() const;

  /**
  * @brief      Gets the successors.
  *
  * @param[in]  quId   The qu identifier
  * @param[in]  refId  The reference identifier
  *
  * @return     The successors.
  */
  std::vector<Node> getSuccessors(int quId, int refId);

 protected:
  /**
   * @brief      Gets the regular successors for node (quId, refId) from
   * database.
   * Select followers based on _fanOut;
   *
   * @param[in]  quId   The qu identifier
   * @param[in]  refId  The reference identifier
   *
   * @return
   */
  std::vector<Node> getFanOutBasedSuccessors(int quId, int refId) const;

  iDatabase::Ptr _database = nullptr;
  int _fan_out = 0;

 private:
  // current successors
  std::vector<Node> _successors;
};

/** \class SuccessorManager
 * Given the node, precisely node coordinates (quId, refId), this class decides,
 what should be the children of a current node.
 Typically the children set {quId+1, refId (+/-) fanOut} the cost are then asked
 from iDatabase::Ptr.
 **/

#endif  // SRC_SUCCESSOR_MANAGER_SUCCESSOR_MANAGER_H_
