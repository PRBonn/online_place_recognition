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

#include "config_parser.h"
#include <fstream>
#include <sstream>
#include "yaml-cpp/yaml.h"

using std::string;

void ConfigParser::print() const {
  printf("== Read parameters ==\n");
  printf("== Path2query: %s\n", path2qu.c_str());
  printf("== Path2ref: %s\n", path2ref.c_str());
  printf("== CostMatrix: %s\n", costMatrix.c_str());
  printf("== CostOutputName: %s\n", costOutputName.c_str());

  printf("== Query size: %d\n", querySize);
  printf("== NonMatchCost: %3.4f\n", nonMatchCost);
  printf("== Expansion Rate: %3.4f\n", expansionRate);
  printf("== FanOut: %d\n", fanOut);

  printf("== Path2query images: %s\n", path2quImg.c_str());
  printf("== Path2reference images: %s\n", path2refImg.c_str());
  printf("== Image extension: %s\n", imgExt.c_str());
  printf("== Buffer size: %d\n", bufferSize);
}

bool ConfigParser::parseYaml(const std::string &yamlFile) {
  YAML::Node config;
  try {
    config = YAML::LoadFile(yamlFile.c_str());
  } catch (...) {
    printf("[ERROR][ConfigParser] File %s cannot be opened\n",
           yamlFile.c_str());
    return false;
  }
  if (config["path2ref"]) {
    path2ref = config["path2ref"].as<std::string>();
  }
  if (config["path2qu"]) {
    path2qu = config["path2qu"].as<std::string>();
  }
  if (config["querySize"]) {
    querySize = config["querySize"].as<int>();
  }
  if (config["fanOut"]) {
    fanOut = config["fanOut"].as<int>();
  }
  if (config["nonMatchCost"]) {
    nonMatchCost = config["nonMatchCost"].as<double>();
  }
  if (config["expansionRate"]) {
    expansionRate = config["expansionRate"].as<double>();
  }
  if (config["path2quImg"]) {
    path2quImg = config["path2quImg"].as<std::string>();
  }

  if (config["path2refImg"]) {
    path2refImg = config["path2refImg"].as<std::string>();
  }
  if (config["imgExt"]) {
    imgExt = config["imgExt"].as<std::string>();
  }
  if (config["bufferSize"]) {
    bufferSize = config["bufferSize"].as<int>();
  }
  if (config["costMatrix"]) {
    costMatrix = config["costMatrix"].as<std::string>();
  }
  if (config["costOutputName"]) {
    costOutputName = config["costOutputName"].as<std::string>();
  }

  return true;
}
