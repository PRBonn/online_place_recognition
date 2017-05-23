#!/bin/bash

mkdir data/

wget http://www.ipb.uni-bonn.de/html/projects/visual_place_recognition/freiburg_example.zip -O data/freiburg_example.zip
unzip data/freiburg_example.zip -d data/
rm data/freiburg_example.zip 

wget http://www.ipb.uni-bonn.de/html/projects/visual_place_recognition/bonn_example.zip -O data/bonn_example.zip
unzip data/bonn_example.zip -d data/
rm data/bonn_example.zip 


