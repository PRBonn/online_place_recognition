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

#ifndef SRC_TOOLS_CONFIG_PARSER_CONFIG_PARSER_H_
#define SRC_TOOLS_CONFIG_PARSER_CONFIG_PARSER_H_

#include <string>

/**
 * @brief      Class for configuration parser.
 */
class ConfigParser {
 public:
  ConfigParser() {}
  /**
   * @brief      parses the provided yaml file.
   *
   * @param[in]  yamlFile  The yaml file
   *
   * @return     returns false, if the file was not found.
   */
  bool parseYaml(const std::string &yamlFile);
  /**
   * @brief      prints to console all the parameters.
   */
  void print() const;

  std::string path2qu = "";
  std::string path2ref = "";
  std::string path2quImg = "";
  std::string path2refImg = "";
  std::string imgExt = "";
  std::string costMatrix = "";
  std::string costOutputName = "";

  int querySize = -1;
  int fanOut = -1;
  int bufferSize = -1;
  double nonMatchCost = -1.0;
  double expansionRate = -1.0;
};

/*! \var std::string ConfigParser::path2qu
    \brief stores path to the folder with query features.
*/
/*! \var std::string ConfigParser::path2ref
    \brief stores path to the folder with reference features.
*/

/*! \var std::string ConfigParser::path2quImg
    \brief stores path to the folder with query images.
*/
/*! \var std::string ConfigParser::path2refImg
    \brief stores path to the folder with reference images.
*/
/*! \var std::string ConfigParser::imgExt
    \brief stores the extension of the provided images (".jpg", ".png", etc).
*/
/*! \var std::string ConfigParser::costMatrix
    \brief stores path to precomputed cost/similarity matrix.
*/
/*! \var std::string ConfigParser::costOutputName
    \brief stores the name of the produced result for the cost_matrix_based
   matching.
*/

/*! \var int ConfigParser::querySize
    \brief stores number of query images.
*/
/*! \var int ConfigParser::fanOut
    \brief stores the fan out parameter, number of children each node in the
   graph can have. Represents possible difference in speed of moving cameras or
   frame rate.
*/

/*! \var int ConfigParser::bufferSize
    \brief number of image features to be cached. Speeds up the computation for
   feature_based matching. Irrelevant for cost_matrix_based matching.
*/
/*! \var double ConfigParser::nonMatchCost
    \brief maximum boundary for the matching cost to still be considered as a
   match. For example, if `nonMatchCost = 5.0` then every smaller cost should
   represent the fact that 2 images match and every higher cost represents the
   fact that 2 image descriptors do not represent the same place. (sensitive
   parameter, depends on the matched trajectories).
*/

/*! \var double ConfigParser::expansionRate
    \brief a weighting parameter [0,1], which controls the expansion/reduction
   of the graph. 0 - expands the graph that corresponds to the full matching
   matrix. 1 - approaches the greedy search. Not a sensitive parameter, should
   typically be selected from 0.5 - 0.7.
*/

#endif  // SRC_TOOLS_CONFIG_PARSER_CONFIG_PARSER_H_
