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

#include "visualizer/match_viewer.h"

#include <QDebug>
#include <QScrollBar>
#include <string>
#include <vector>

MatchViewer::MatchViewer() {}

bool MatchViewer::init(int width, int height) {
  if (!isReady()) {
    printf(
        "[ERROR][MatchViewer] There is a reason why MatchViewer is not ready. "
        "No match visualization\n");
    return false;
  }
  this->setDragMode(QGraphicsView::ScrollHandDrag);
  this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  // this->setScene(&_scene);
  // QGraphicsView::fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
  
  _quScene = new QGraphicsScene(0.0, 0.0, width * 0.9, height * 0.45);
  _refScene = new QGraphicsScene(0.0, 0.0, width * 0.9, height * 0.4);

  _quImage.setScene(_quScene);
  _refImage.setScene(_refScene);

  _quImage.setDragMode(QGraphicsView::ScrollHandDrag);

  _refImage.setDragMode(QGraphicsView::ScrollHandDrag);
  _refImage.setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  QTransform scaleText(2.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 1.0);

  _textItem = _refImage.scene()->addText("No Match");
  _textItem->setVisible(false);
  _textItem->setTransform(scaleText);
  _textItem->setDefaultTextColor(QColor(255, 255, 255));
  _textItem->setPos(_refImage.scene()->width() / 3,
                    _refImage.scene()->height() / 2);

  _quText = _quImage.scene()->addText("Query Image");
  _quText->setVisible(true);
  _quText->setTransform(scaleText);

  _quText->setDefaultTextColor(QColor(0, 0, 0));
  _quText->setPos(_refImage.scene()->width() / 3,
                  _refImage.scene()->height() / 2);

  _refText = _refImage.scene()->addText("Found Match");
  _refText->setVisible(true);
  _refText->setDefaultTextColor(QColor(0, 0, 0));
  // _refText->setTextWidth(10.0);
  _refText->setTransform(scaleText);
  _refText->setPos(_refImage.scene()->width() / 3,
                   _refImage.scene()->height() / 2);

  _layout = new QGridLayout;
  _layout->addWidget(&_quImage, 0, 0);
  _layout->addWidget(&_refImage, 1, 0);

  this->setLayout(_layout);
  show();
  return true;
}

MatchViewer::~MatchViewer() {
  if (_layout) {
    delete _layout;
  }
  if (_refScene) {
    delete _refScene;
  }
  if (_quScene) {
    delete _quScene;
  }
}

bool MatchViewer::setDatabase(OnlineDatabase::Ptr database) {
  if (!database) {
    printf("[ERROR][PathViewer] The database is not set\n");
    return false;
  }
  _database = database;
  return true;
}

void MatchViewer::wheelEvent(QWheelEvent *event) {
  const QPointF p0scene = mapToScene(event->pos());

  qreal factor = std::pow(1.001, event->delta());
  scale(factor, factor);

  const QPointF p1mouse = mapFromScene(p0scene);
  const QPointF move = p1mouse - event->pos();  // The move
  horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
  verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
}

void MatchViewer::receivedMatch(int quId, int refId, bool hidden) {
  // printf("Received match %d %d \n", quId, refId);
  std::string qu_path, ref_path;
  _quText->setVisible(false);
  _refText->setVisible(false);

  qu_path = _database->getQuFeatureName(quId);
  qu_path = _quImagesFolder + qu_path;
  // chop out '-feature.txt' -> 12 symbols
  qu_path.erase(qu_path.end() - 12, qu_path.end());
  qu_path += _imgExt;

  // printf("Trying to load image: %s\n", qu_path.c_str());

  QPixmap img_qu;
  img_qu.load(qu_path.c_str());
  // img_qu = img_qu.scaledToHeight(_quImage.scene()->height());
  img_qu = img_qu.scaled(_quImage.scene()->width(), _quImage.scene()->height());
  if (!_qu_imgItem) {
    _qu_imgItem = _quImage.scene()->addPixmap(img_qu);
  } else {
    _qu_imgItem->setPixmap(img_qu);
  }

  if (hidden) {
    // make black pixmap;
    QPixmap empty_img(_quImage.scene()->width(), _quImage.scene()->height());
    empty_img.fill(Qt::black);
    if (!_ref_imgItem) {
      _ref_imgItem = _refImage.scene()->addPixmap(empty_img);
    } else {
      _ref_imgItem->setPixmap(empty_img);
    }
    _textItem->setVisible(true);
    _textItem->setZValue(1);
  } else {
    _textItem->setVisible(false);
    ref_path = _database->getRefFeatureName(refId);
    ref_path = _refImagesFolder + ref_path;
    // chop out '-feature.txt' -> 12 symbols
    ref_path.erase(ref_path.end() - 12, ref_path.end());
    ref_path += _imgExt;

    QPixmap img_ref;
    // printf("[MatchViewer] reading image %s\n", ref_path.c_str());
    img_ref.load(ref_path.c_str());
    // img_ref = img_ref.scaledToHeight(_refImage.scene()->height());
    img_ref =
        img_ref.scaled(_refImage.scene()->width(), _refImage.scene()->height());

    if (!_ref_imgItem) {
      _ref_imgItem = _refImage.scene()->addPixmap(img_ref);
    } else {
      _ref_imgItem->setPixmap(img_ref);
    }
  }
}

bool MatchViewer::isReady() const {
  if (!_database) {
    printf("[ERROR][MatchViewer] Database is not set\n");
    return false;
  }
  if (_quImagesFolder.empty()) {
    printf("[ERROR][MatchViewer] Folder for query images is not set\n");
    return false;
  }
  if (_refImagesFolder.empty()) {
    printf("[ERROR][MatchViewer] Folder for reference images is not set\n");
    return false;
  }
  // # extension. Maybe also works without it
  // if (_ext.empty()) {
  //   printf("[ERROR][MatchViewer] Folder for reference images is not set\n");
  //   return false;
  // }
  return true;
}

void MatchViewer::setQuImageDirectory(const std::string &folder) {
  _quImagesFolder = folder;
}
void MatchViewer::setRefImageDirectory(const std::string &folder) {
  _refImagesFolder = folder;
}
void MatchViewer::setImageExtension(const std::string &ext) { _imgExt = ext; }
