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

#ifndef SRC_VISUALIZER_MATCH_VIEWER_H_
#define SRC_VISUALIZER_MATCH_VIEWER_H_

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QGridLayout>
#include <QtGui>
#include <string>

#include "database/online_database.h"
#include "online_localizer/online_localizer.h"

/**
 * @brief      Visualizes the last query image and the found image match. To use
 * this class the path to the images should be specified.
 */
class MatchViewer : public QGraphicsView {
  Q_OBJECT

 public:
  using Ptr = std::shared_ptr<MatchViewer>;
  using ConstPtr = std::shared_ptr<const MatchViewer>;

  MatchViewer();
  ~MatchViewer();

  bool setDatabase(OnlineDatabase::Ptr database);
  void setQuImageDirectory(const std::string &folder);
  void setRefImageDirectory(const std::string &folder);
  void setImageExtension(const std::string &ext);

  bool isReady() const;
  bool init(int width, int height);

  void wheelEvent(QWheelEvent *event);

 public slots:
  void receivedMatch(int quId, int refId, bool hidden);

 private:
  std::string _refImagesFolder = "";
  std::string _quImagesFolder = "";
  std::string _imgExt = "";

  // variable for storing two single images
  QGraphicsPixmapItem *_ref_imgItem = nullptr;
  QGraphicsPixmapItem *_qu_imgItem = nullptr;
  QGraphicsTextItem *_textItem = nullptr;

  QGraphicsTextItem *_quText = nullptr;
  QGraphicsTextItem *_refText = nullptr;


  QGraphicsView _quImage, _refImage;
  QGraphicsScene *_quScene = nullptr;
  QGraphicsScene *_refScene = nullptr;
  QGridLayout *_layout = nullptr;
  OnlineDatabase::Ptr _database = nullptr;
};

#endif  // SRC_VISUALIZER_MATCH_VIEWER_H_
