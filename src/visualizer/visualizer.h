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

#ifndef SRC_VISUALIZER_VISUALIZER_H_
#define SRC_VISUALIZER_VISUALIZER_H_

#include <QGridLayout>
#include <QWidget>
#include <set>
#include <string>
#include <memory>
#include <vector>

#include "database/online_database.h"
#include "online_localizer/ilocvisualizer.h"
#include "visualizer/localization_viewer.h"
#include "visualizer/match_viewer.h"

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;


/**
 * @brief      Main window for the online database visualizer.
 */
class Visualizer : public QWidget, public iLocVisualizer {
  Q_OBJECT

 public:
  using Ptr = std::shared_ptr<Visualizer>;
  Visualizer();
  ~Visualizer();

  /**
   * @brief      Set the match viewer to see the last matched images.
   *
   * @param      matchViewer  The match viewer
   *
   * @return     { false if matchViewer is null}
   */
  bool setMatchViewer(MatchViewer *matchViewer);
  bool setLocalizationViewer(LocalizationViewer *loc_viewer);

  void drawPath(const std::vector<PathElement> &path) override;
  void drawFrontier(const PriorityQueue &frontier) override;
  void drawExpansion(NodeSet expansion) override;
  void processFinished() override;

  bool isReady() const;

 signals:
  void drawPath_signal(const std::vector<PathElement> &path);
  void showPathImage(int quId, int refId, bool hidden);
  // not implemented yet 
  void drawFrontier_signal(const PriorityQueue &frontier);
  void drawExpansion_signal(const NodeSet &expansion);
  void localizationFinished();

 private:
  void setSignalSlots();
  LocalizationViewer *_locViewer = nullptr;
  MatchViewer *_matchViewer = nullptr;
  QGridLayout *_mainLayout = nullptr;
};

#endif  // SRC_VISUALIZER_VISUALIZER_H_
