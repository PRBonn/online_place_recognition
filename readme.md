# Graph-based matching of image sequences

## What does this code do?
Given two sequences of images represented by the descriptors, the code constructs a data association graph and performs a search within this graph, so that for every query image, the code computes a matching hypothesis to an image in a database sequence as well as matching hypothesis for the previous images.

The matching procedure can be perfomed in two modes --- **feature based** and **cost matrix based** mode.

For more theoretical details, please refer to our paper [Lazy data association for image sequence matching under substantial appearance changes](http://www.ipb.uni-bonn.de/pdfs/vysotska16ral-icra.pdf).

Checkout the video:

[![Matching example video](http://img.youtube.com/vi/l-hNk7Z4lSk/0.jpg)](https://www.youtube.com/watch?v=l-hNk7Z4lSk&feature=youtu.be "Matching example video")

## Installation guide

### Prerequisites

* Yaml-cpp (requires BOOST till now): `sudo apt-get install libyaml-cpp-dev`
* OpenCV: `sudo apt-get install libopencv-dev`
* Qt5: `sudo apt-get install qt5-default`
* (optional) Doxygen (generate documentation): `sudo apt-get install doxygen`

For the OSX install, you may need to run the following commands:

 * `brew install  yaml-cpp`
 * `brew install opencv`
 * `brew install doxygen`
 * `brew install qt5`
 * `export CMAKE_PREFIX_PATH=/usr/local/Cellar/qt/[YOUR VERSION]`
    For example `eg. export CMAKE_PREFIX_PATH=/usr/local/Cellar/qt/5.8.0_2`

### Build
To build the project, run the following commands from the main directory:

* `mkdir build`
* `cd build`
* `cmake ..`
* `make -j4`


Additionally, you should be able to generate documentation as follows:
* `cd doc`
* `doxygen online_place_recognition.conf`

To access the documentation run `firefox html/index.html`.

## What do I need to run this code?

* Precomputed image descriptors or cost matrix
* Configuration file

An example of how to run the code please see [**RUN EXAMPLES**](examples/readme.md).


#### Feature based matching
In this mode, the program operates using **precomputed image descriptors**.
To run the matching procedure you need to provide the feature files. An example of how to run the code can be found [feature based matching example](apps/feature_based_matching/readme.md). 

For details on used feature descriptors please refer to [feature description](apps/feature_based_matching/readme.md#feature-format).

**Note**: In this mode, individual features will be loaded and matched on demand. In order to be able to deal with dramatic visual changes, we typically operate with high-dimensional features and the matching procedure can take quite a long time--depending on the size and the complexity of the sequences.

#### Cost matrix based matching

For this mode, we require the **cost matrix** between two sequences to be given/pre-computed. To compute the matching matrix, please see the following [estimating of a cost matrix example](apps/create_cost_matrix/readme.md).

An example on how to run the matching procedure in this mode can be found [cost matrix matching example](apps/cost_matrix_based_matching/readme.md).

**Note**: This method may be used if you have rather small sequences (up to 1000 images). For bigger sequences, you may run into memory issues since the programs has to store a quite big matrix.

## Adapting the code for custom features

This framework can be adapted to matching features of the different type --- your **own features**. To use the graph matching strategy with your **own features** see the following [description](src/features/readme.md).

## Related publication

Please cite the related publication, if you use the code:

```
@article{vysotska2016lazy, 
  title     = {Lazy Data Association for Image Sequences Matching Under Substantial Appearance Changes},
  author    = {Vysotska, Olga and Stachniss, Cyrill},
  year      = {2016},
  publisher = {IEEE Robotics and Automation Letters}
  number    = {1},
  pages     = {1-8},
  volume    = {1},
  doi       = {10.1109/LRA.2015.2512936}
}
```

## Troubleshooting

In case the code is not working for you or you experience some code related problems, please consider openning an issue.
