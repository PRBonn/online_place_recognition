# How to make the code work with custom image descriptors

This framework was designed to be adaptable to different kind of features. That's why the main components of the code work with an abstract feature class, namely `iFeature`. 

To be able to use your own features, you need to:

* write your own implementation for `iFeature`, see `ifeature.h` for details.
* let the `FeatureFactory` know that there exist another implementation for a feature, by changing the places marked with `// NEW_FEATURE` in `feature_factory.*`
