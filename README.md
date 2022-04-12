# scheduling_algorithms
Below are the notes and instructions to compile and run the code for each server and, client.
Before running any of the file/commands below, please ensure that the current working directory contains all the assignment files, i.e., `scheduling_sim.cpp` and `plot.py`.

ASSUMPTION: context switch is instant.


## Instructions for Compiling the Code

To compile the code, run the following command:
```bash
g++ scheduling_sim.cpp -o out/simulating_sim
```


## Run the code by following command:
```bash
out/scheduling_sim <number of processes to emulate>
```

Running the above command will do the following 2 things:
- generate `process_table.txt` file with properties of processes.
- print some statistics related to each algorithm. The last line therein serves as a part of the input for the last part.


## Plotting
To run the simulaion for this part, run the following command:

```bash
python3 plot.py
```

Running this will generate all the required plots in the `plots` folder.

How we do this? The program `out/scheduling_sim` only runs simulation for a certain number of processes at once. Therefore, we run this program multiple times through the script `plot.py` to run it 10 times for `{10, 20, 30, 40, 50}` processes. 
