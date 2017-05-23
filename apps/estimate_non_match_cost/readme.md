# Estimating nonMatchCost for a new dataset

The `nonMatchCost` is a parameter that specifies the cost boundary that separates the images that represent the same place from those that do not represent the same place.  This parameter depends on the visual appearance of the sequences to be matched as well as extracted features and matching metric for the features.

To be able to get at least a rough idea on how the `nonMatchingCost` for a dataset may look like, we provide a small estimation tool.

To run the tool one needs to **create** 2 separate files `match_example.txt` and `nonMatch_example.txt`.

Then specify the pairs of features that represent the same place and, thus, should match, in the 'match' file and pairs of features and should not be matched in 'nonMatch' file.

Afterwards run:
`./estimate_non_match_cost pathto/matchFile(.txt) pathto/nonMatchFile(.txt)`

