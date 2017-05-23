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

#include "visualizer/localization_viewer.h"

#include <QColormap>
#include <QDebug>
#include <QScrollBar>
#include <QTransform>
#include <vector>

LocalizationViewer::LocalizationViewer() {
  this->setDragMode(QGraphicsView::ScrollHandDrag);
  this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  if (!_pixmap.load("../src/visualizer/localization_screen.png")) {
    printf("Screensaver was not loaded\n");
  }
  _initScreen = new QGraphicsPixmapItem;
  _initScreen->setPixmap(_pixmap);
  _scene.addItem(_initScreen);
  this->setScene(&_scene);
  show();
}

void LocalizationViewer::setSceneSize(qreal width, qreal height) {
  _scene.setSceneRect(0.0, 0.0, width, height);
}

bool LocalizationViewer::setDatabase(iDatabase::Ptr database) {
  if (!database) {
    printf("[ERROR][Visualizer] The database is not set\n");
    return false;
  }
  _database = database;
  return true;
}

void LocalizationViewer::drawNode(float x, float y, const QPen &pen,
                                  bool filled) {
  QPolygonF point;
  point << QPointF(x, y) << QPointF(x + 1, y) << QPointF(x + 1, y + 1)
        << QPointF(x, y + 1);
  if (filled) {
    _scene.addPolygon(point, pen, QBrush(pen.color()));
  } else {
    _scene.addPolygon(point, pen);
  }
}

void LocalizationViewer::wheelEvent(QWheelEvent *event) {
  const QPointF p0scene = mapToScene(event->pos());

  qreal factor = std::pow(1.001, event->delta());
  scale(factor, factor);

  const QPointF p1mouse = mapFromScene(p0scene);
  const QPointF move = p1mouse - event->pos();  // The move
  horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
  verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
}

void LocalizationViewer::receivedPath(const std::vector<PathElement> &path) {
  if (_initScreen) {
    _initScreen->setVisible(false);
    _scene.removeItem(_initScreen);
    delete (_initScreen);
    _initScreen = nullptr;
  }

  QPen pen(Qt::blue);
  pen.setCapStyle(Qt::SquareCap);
  pen.setWidthF(0.1);

  for (const PathElement &el : path) {
    if (el.state == HIDDEN) {
      pen.setColor(Qt::blue);
    } else {
      pen.setColor(Qt::red);
    }
    auto found = _path.find(el.quId);
    if (found == _path.end()) {
      // query images was not found
      QGraphicsItem *item =
          _scene.addRect(el.refId, el.quId, 1.0, 1.0, pen, QBrush(pen.color()));
      item->setZValue(1);  // should be plotted on top of the expanded nodes
      _path[el.quId] = std::make_pair(el.refId, item);
    } else {
      // query id was found
      if (el.refId == found->second.first) {
        // if the refId hasn't changed
        // do nothing
      } else {
        // remove previous item + add new one
        _scene.removeItem(found->second.second);
        // query images was not found
        QGraphicsItem *item = _scene.addRect(el.refId, el.quId, 1.0, 1.0, pen,
                                             QBrush(pen.color()));
        item->setZValue(1);  // should be plotted on top of the expanded nodes
        _path[el.quId] = std::make_pair(el.refId, item);
      }
    }
  }
  PathElement last = path.back();
  this->centerOn(QPointF(last.refId, last.quId));
  show();
}
void LocalizationViewer::receivedFrontier(const PriorityQueue &frontier) {
  // printf("[Visualizer] Frontier received\n");
  // _frontier = frontier;
}

void LocalizationViewer::receivedExpansion(NodeSet expansion) {
  if (_initScreen) {
    _initScreen->setVisible(false);
  }
  QPen pen;
  pen.setCapStyle(Qt::SquareCap);
  pen.setWidthF(0.1);
  std::vector<Node> expanded_nodes = expansion.toVector();
  for (const Node &node : expanded_nodes) {
    double x = _database->getCost(node.quId, node.refId);
    x = 1 - 76 * log(1. / x);
    double gray_cost = 255 - x;
    pen.setColor(QColor(gray_cost, gray_cost, gray_cost));
    drawNode(node.refId, node.quId, pen, true);
  }
}

void LocalizationViewer::receivedLocalizationFinished() {
  _scene.setSceneRect(_scene.itemsBoundingRect());
  QGraphicsView::fitInView(0.0, 0.0, _scene.width(), _scene.height(),
                           Qt::KeepAspectRatio);
}
