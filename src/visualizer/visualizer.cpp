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

#include "visualizer/visualizer.h"
#include <QPoint>
#include <QVBoxLayout>
#include <QtGui>
#include <iostream>
#include <vector>

Q_DECLARE_METATYPE(PriorityQueue)
Q_DECLARE_METATYPE(PathElement)
Q_DECLARE_METATYPE(Node)
Q_DECLARE_METATYPE(NodeSet)

Visualizer::Visualizer() {
  resize(WINDOW_WIDTH, WINDOW_HEIGHT);
  _mainLayout = new QGridLayout;
  setLayout(_mainLayout);
  show();
}

bool Visualizer::setLocalizationViewer(LocalizationViewer *locViewer) {
  if (!locViewer) {
    printf(
        "[ERROR][Visualizer] Localization Viewer is not set. Can't add to "
        "visualizer.\n");
    return false;
  }

  _locViewer = locViewer;
  _mainLayout->addWidget(_locViewer, 0, 0);
  setLayout(_mainLayout);
  // _locViewer->setSceneSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  show();

  qRegisterMetaType<std::vector<PathElement> >("Path");
  qRegisterMetaType<PriorityQueue>("Frontier");
  qRegisterMetaType<NodeSet>("Expansion");

  QObject::connect(
      this, SIGNAL(drawPath_signal(const std::vector<PathElement> &)),
      _locViewer, SLOT(receivedPath(const std::vector<PathElement> &)));

  QObject::connect(this, SIGNAL(drawFrontier_signal(const PriorityQueue &)),
                   _locViewer, SLOT(receivedFrontier(const PriorityQueue &)));
  QObject::connect(this, SIGNAL(drawExpansion_signal(NodeSet)), _locViewer,
                   SLOT(receivedExpansion(NodeSet)));
  QObject::connect(this, SIGNAL(localizationFinished()), _locViewer,
                   SLOT(receivedLocalizationFinished()));
  printf("[INFO][Visualizer] Localization Viewer is set\n");
  return true;
}

bool Visualizer::setMatchViewer(MatchViewer *matchViewer) {
  if (!matchViewer) {
    printf(
        "[ERROR][Visualizer] Match Viewer is not set. Can't add to "
        "visualizer.\n");
    return false;
  }
  _matchViewer = matchViewer;
  if (_matchViewer->init(WINDOW_WIDTH * 0.3, WINDOW_HEIGHT)) {
    _mainLayout->addWidget(_matchViewer, 0, 1);
    // _locViewer->setSceneSize(WINDOW_WIDTH * 0.7, WINDOW_HEIGHT);
    _mainLayout->setColumnMinimumWidth(0, WINDOW_WIDTH * 0.7);
    _mainLayout->setColumnMinimumWidth(1, WINDOW_WIDTH * 0.3);

    setLayout(_mainLayout);
    QObject::connect(this, SIGNAL(showPathImage(int, int, bool)), _matchViewer,
                     SLOT(receivedMatch(int, int, bool)));
    return true;
  }
  return false;
}

Visualizer::~Visualizer() { delete _mainLayout; }

void Visualizer::drawPath(const std::vector<PathElement> &path) {
  emit drawPath_signal(path);
  PathElement lastMatch = path.back();
  emit showPathImage(lastMatch.quId, lastMatch.refId,
                     lastMatch.state == HIDDEN);
}
void Visualizer::drawFrontier(const PriorityQueue &frontier) {
  emit drawFrontier_signal(frontier);
}

void Visualizer::drawExpansion(NodeSet expansion) {
  emit drawExpansion_signal(expansion);
}

bool Visualizer::isReady() const {
  if (!_locViewer) {
    printf("[WARNING] Visualizer is not ready\n");
    return false;
  }
  return true;
}

void Visualizer::processFinished() { emit localizationFinished(); }
