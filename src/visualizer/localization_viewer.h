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

#ifndef SRC_VISUALIZER_LOCALIZATION_VIEWER_H_
#define SRC_VISUALIZER_LOCALIZATION_VIEWER_H_

#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QPixmap>
#include <QtGui>

#include <set>
#include <vector>
#include <unordered_map>

#include "database/idatabase.h"
#include "online_localizer/online_localizer.h"
#include "online_localizer/priority_queue.h"
#include "successor_manager/node.h"

/**
 * @brief      Visualizes the expanded graph as well as current  best matching
 * hypothesis.
 */
class LocalizationViewer : public QGraphicsView {
  Q_OBJECT

 public:
  using Ptr = std::shared_ptr<LocalizationViewer>;
  using ConstPtr = std::shared_ptr<const LocalizationViewer>;

  LocalizationViewer();
  ~LocalizationViewer() {}
  void setSceneSize(qreal width, qreal height);

  /**
   * @brief      The costs for visualization are taken from the database.
   *
   * @param[in]  database  The database
   *
   * @return     { false, if database::Ptr is null}
   */
  bool setDatabase(iDatabase::Ptr database);

  void wheelEvent(QWheelEvent *event);
  void drawNode(float x, float y, const QPen &pen, bool filled);

 public slots:
  void receivedPath(const std::vector<PathElement> &path);
  void receivedFrontier(const PriorityQueue &frontier);
  void receivedExpansion(NodeSet expansion);
  void receivedLocalizationFinished();

 private:
  // std::vector<QGraphicsPolygonItem *> _pathItems;
  std::unordered_map<int, std::pair<int, QGraphicsItem*> > _path;
  // PriorityQueue _frontier;
  QGraphicsScene _scene;
  iDatabase::Ptr _database = nullptr;
  QGraphicsPixmapItem *_initScreen = nullptr;
  QPixmap _pixmap;
};

#endif  // SRC_VISUALIZER_LOCALIZATION_VIEWER_H_
