# scheduling_algorithms

## Problem Description

We have taken in N, the number of processes as input from the user, and
after that generated inter-arrival time as a random variable[0,10] following
exponential distribution with some given mean mu =1/5. The CPU bursts of the
processes are generated as uniform random variables [1,20]. The priorities of the
processes are also generated as uniform random variables[1,10] (where a lower
number signifies higher priority).
<br>
<br>
Following this we have written code for 5 scheduling algorithms, namely:
<br>
<br>
● First Come First Serve (FCFS)
<br>
● Non-preemptive Shortest Job First
<br>
● Preemptive Shortest Job First
<br>
● Round Robin with time quantum δ = 2-time units
<br>
● Priority-based scheduling
<br>
<br>
And using them found out the average turnaround time (ATT), average waiting
time (AWT), and average response time (ART) for the processes.


## Instructions for Compiling the Code

Before running any of the file/commands below, please ensure that the current working directory contains all the assignment files, i.e., `scheduling_sim.cpp` and `plot.py`.

ASSUMPTION: context switch is instant.

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

## Plots

