# Cost matrix based matching

You are in right place, if you want to use graph matching procedure on already **computed similarity / cost matrix**.

**+** No need to implement own features


**-** Limited sequences size. Up to the size of the matrices that fit in RAM.

Change the respective parameters in the `config_cost_matrix.yaml` to account for your cost matrix.
Then, run:

`./run_localizer_cost_matrix_based pathto/config_cost_matrix.yaml`

To create the cost matrix, please visit [create_cost_matrix](../create_cost_matrix/readme.md)

## Cost matrix format
The code knows how to read the matrix from a `.txt` file, stored as matrix.
First line in the `.txt` should specify number of `rows` and `cols` that the matrix has.

For more details, please check `database/cost_matrix_database.h`.

**Warning** this code expects the costs to be __cosine distance__ costs, e.g. the higher values correspond to the better match.
If your costs have different meaning, e.g. the lower the cost the better the match, then you need to either invert your costs before passing in to a program or derive from the class `CostMatrixDatabase` a class with your own implementation of `getCost` function.

## Expected output

Upon finishing the code should provide the file `matched_path.txt` with image associations.
Additionally, it should generate an image with costs and overlayed path (red/blue) as well as expanded nodes (green).


