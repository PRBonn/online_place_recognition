# Computing the cost matrix

To compute the cost matrix or the similarity matrix between 2 sequences, fill the `config.yaml` file with appropriate paths to the features.
Then, run:

`./create_cost_matrix pathto/config.yaml`

After the end of computation, the resulting matrix will be written to the file `cost_matrixNxM.txt`, where `N` - is a number of query images (rows) and `M` - number of reference images (cols). Additionally, the image `cost_matrix.png` will be generated.