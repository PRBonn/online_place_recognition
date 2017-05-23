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

#include <QApplication>
#include <memory>
#include <thread>

#include "database/idatabase.h"
#include "database/online_database.h"
#include "online_localizer/ilocvisualizer.h"
#include "online_localizer/online_localizer.h"
#include "successor_manager/successor_manager.h"
#include "tools/config_parser/config_parser.h"
#include "visualizer/match_viewer.h"
#include "visualizer/visualizer.h"

using std::make_shared;

void localize(OnlineLocalizer *loc) { 
   // std::this_thread::sleep_for(std::chrono::seconds(20)); // sleep to start video recording
   loc->run(); }

int main(int argc, char *argv[]) {
  printf("===== Online place recognition ====\n");
  if (argc < 2) {
    printf("[ERROR] Not enough input parameters.\n");
    printf("Proper usage: ./run_localizer_featured_based config_file.yaml\n");
    exit(0);
  }
  std::string config_file = argv[1];
  ConfigParser parser;
  parser.parseYaml(config_file);
  parser.print();

  OnlineDatabase online_database;
  online_database.setRefFeaturesFolder(parser.path2ref);
  online_database.setQuFeaturesFolder(parser.path2qu);
  online_database.setBufferSize(parser.bufferSize);

  if (!online_database.isSet()) {
    printf("[ERROR] database is not set completely\n");
    return false;
  }
  auto onlineDatabasePtr = make_shared<OnlineDatabase>(online_database);

  // initialize SuccessorManager
  auto successorManagerPtr = make_shared<SuccessorManager>(SuccessorManager());
  successorManagerPtr->setFanOut(parser.fanOut);
  successorManagerPtr->setDatabase(onlineDatabasePtr);

  // initialize Visualizer
  QApplication app(argc, argv);

  Visualizer *visualizer = new Visualizer();
  visualizer->setWindowTitle(
      QApplication::translate("toplevel", "Olga's awesome viewer"));

  MatchViewer *matchViewer = new MatchViewer();
  matchViewer->setQuImageDirectory(parser.path2quImg);
  matchViewer->setRefImageDirectory(parser.path2refImg);
  matchViewer->setImageExtension(parser.imgExt);
  matchViewer->setDatabase(onlineDatabasePtr);

  LocalizationViewer *locViewer = new LocalizationViewer();
  locViewer->setDatabase(onlineDatabasePtr);

  if (!visualizer->setMatchViewer(matchViewer)) {
    printf("[ERROR] MatchViewer was not set\n");
  }

  if (!visualizer->setLocalizationViewer(locViewer)) {
    printf("[ERROR] LocalizationViewer was not set\n");
  }
  printf("[INFO] Visualizer was initialized\n");

  auto visPtr = iLocVisualizer::Ptr(visualizer);


  OnlineLocalizer localizer;
  localizer.setQuerySize(parser.querySize);
  localizer.setSuccessorManager(successorManagerPtr);
  localizer.setExpansionRate(parser.expansionRate);  // expand everything
  localizer.setNonMatchingCost(parser.nonMatchCost);
  if (visualizer->isReady()) {
    localizer.setVisualizer(visPtr);
  }

  std::thread process(localize, &localizer);
  app.exec();
  printf("%s\n", "Visualizer closed.");
  std::string pathFile = "matched_path.txt";
  localizer.printPath(pathFile);
  process.join();

  return 0;
}
