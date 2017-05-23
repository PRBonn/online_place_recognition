# Feature based matching

You are in a right place, if you want to run the code with **precomputed features**.

You need to store features for individual images in the separate files. The features for difference datasets should also be located in different folders, e.g. query folder, reference folder.

Fill the configuration file and run:

`./run_localizer_feature_based pathto/config.yaml`

## Configuration file
Every binary requires the configuration file. For running the feature-based matching you need to specify the mandatory parameters:

 * `path2ref` - path to a folder, which stores ".txt" feature files for the reference trajectory

 * `path2qu` - path to a folder, which stores ".txt" feature files for the query trajectory

 * `querySize` - number of images in the query sequence

 * `expansionRate` - a weighting parameter [0,1], which controls the expansion/reduction of the graph. 0 - expands the graph that corresponds to the full matching matrix. 1 - approaches the greedy search. Not a sensitive parameter, should typically be selected from 0.5 - 0.7.

 * `fanOut` - (int) specifies number of possible outgoing edges for every node. It compensates for different speeds or framerates between the trajectories. Setting it to 1 means that the cameras have the same framerate and moved with the same speed. Setting it to 2 mean that one camera moved approx twice as fast as another one and so on.

 * `nonMatchCost` - maximum boundary for the matching cost to still be considered as a match. For example, if `nonMatchCost = 5.0` then every smaller cost should represent the fact that 2 images match. (sensitive parameter, depends on the matched trajectories). To get an idea for the `nonMatchingCost`, take a look at [estimate_non_match_cost](../estimate_non_match_cost/readme.md) example.

 * `bufferSize` - the size of the buffer to cache features.

The following parameters are optional and are used to show the pair of last matched images. For this feature to work the images should have the same name as feature files. For example, if you have feature `img_001-feature.txt` the corresponding image should be `img_001.jpg`.

 * `path2refImg` - path to the folder that stores corresponding reference images
 * `path2quImg` - path to the folder that stores corresponding query images
 * `imgExt` -externsion of the provided images



## Feature format
For matching images we use features extracted from 10th layer of [OverFeat](https://github.com/sermanet/OverFeat) neural network.  For images of approximate size of 450x253 the 10th layer extracts a 512x18x24 dimensional feature vector.

If you would like to use other kind of features, please check section [working with custom features](../../src/features/readme.md).

## Output

Upon completion the matching procedure and closing the visualizer the `matched_path.txt` is automatically written to your working directory.
The file stores the result of 2 sequences matching in the following format:

`quId refd status`

where `quId` - id of a query image, `refId` - id of the corresponding image found in the reference trajectory, `status` - **hidden** / **real** (0/1). Only **real** nodes correspond to valid matches.
