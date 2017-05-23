# Examples

To make the process of using the code easier, we provide an example using a small part of Freiburg dataset as well as a part of Bonn dataset.

The Freiburg dataset (276 MB) consists of 2 sequences of images query, recorded in summer, and reference, recorded in winter.
The Bonn dataset (786MB) exhibits different kind of visual apperance change, namely morning scene (query) vs late evening (reference).

To run the examples, you need to first **download** the data, by running the following script:

```
cd examples
./download_sample_data.sh
```
Afterwards to run the code using **Freiburg** dataset run:

`./match_freiburg_example.sh`

for **Bonn** dataset:

`./match_bonn_example.sh`


## Sample datasets

You can also find individual sample datasets on our webpage [**datasets**](http://www.ipb.uni-bonn.de/data/visual-place-recognition-datasets/).


## Expected output

During the program execution you should see the results using the visualizer. 
After the matching is finished, **close** the visualizer. The program will generate  the `matched_path.txt` with the final data associations.
Every line of the output file has the following format:

`quId refd status`, where 

`quId` - id of a query image, 

`refId` - id of the corresponding image found in the reference trajectory, 

`status` - **hidden** / **real** (0/1). 

Only **real** nodes correspond to valid matches.
