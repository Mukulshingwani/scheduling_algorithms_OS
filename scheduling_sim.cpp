#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <random>
#include<bits/stdc++.h>
using namespace std;


double total_wait = 0.0;
double total_initial = 0.0;
double max_turnaround = 0.0;
double min_turnaround = 5000.0;
double max_wait;
double min_wait;
int context_switch = 0;
double total_turnaround = 0.0;
double max_initial;
double min_initial;


//Creates Process class 
class process {
private:
	int arrival_time;
	int burst_time;
	int premp_time;
	int burst_left;
	bool been_seen;
	int response_time;
	int total_wait;
	int priority;
	int PID;
public:
	process(int burst, int time, int id, int _priority);
	void set_arrival(int time);
	void set_pid(int num);
	int get_arrival();
	int get_burst();
	int get_priority();
	int get_burst_left();
	void dec_burst();
    int get_initial_wait();
	int get_total_wait();
	void set_premp_time(int time);
	void add_wait(int time);
	bool get_seen();
	void seen(int time);
	int get_pid();
};

process::process(int burst, int time, int id, int _priority)
{
	PID = id;
	burst_time = burst;
	burst_left = burst;
	arrival_time = time;
	been_seen = false;
	priority = _priority;
	response_time = 0;
	total_wait = 0;
}

void process::set_pid(int num)
{
	PID = num;
}

bool process::get_seen()
{
	return been_seen;
}

int process::get_priority()
{
	return priority;
}

int process::get_burst()
{
	return burst_time;
}

int process::get_pid()
{
	return PID;
}

void process::set_arrival(int time)
{
	arrival_time = time;
}

int process::get_arrival()
{
	return arrival_time;
}

int process::get_burst_left()
{
	return burst_left;
}

void process::dec_burst()
{
	burst_left = burst_left - 1;
}

int process::get_initial_wait()
{
	return response_time;
}

int process::get_total_wait()
{
	return total_wait;
}

void process::set_premp_time(int time)
{
	premp_time = time;
}

void process::add_wait(int time)
{
	total_wait = total_wait + time;
}

void process::seen(int time)
{
	been_seen = true;
	response_time = time;
	total_wait = time;
}

bool sorter(process a, process b)
{
	return a.get_burst() < b.get_burst();
}

bool sorter2(process a, process b)
{
	return a.get_priority() < b.get_priority();
}

bool sorter3(process a, process b)
{
	return a.get_arrival() < b.get_arrival();
}

//Prints statistics
void statistics(int num_processes, string algorithm)
{
    cout << "Algorithm-" << algorithm << ":" << endl;
	
    // min Turnaround times in ms
	cout << "Turn Around Time <min average max>: ";
	printf("%.3f", min_turnaround);
	cout << " ";
    // avg. turnaround time
	printf("%.3f", total_turnaround/num_processes);
	cout << " ";
    // max turnaround time
	printf("%.3f", max_turnaround);
    cout << endl;

	cout << "Response Time <min average max>: ";
	// min. first response time
	printf("%.3f", min_initial);
	cout << " ";
	// avg. first response time
	printf("%.3f", total_initial/num_processes);
	cout << " ";
	// max. first response time
	printf("%.3f", max_initial);
    cout << endl;

	cout << "Wait Time <min average max>: ";
    // min waiting time
	printf("%.3f", min_wait);
	cout << " ";
    // avg waiting time
	printf("%.3f", total_wait/num_processes);
    // max waiting time
	cout << " ";
	printf("%.3f", max_wait);

	cout << endl;

	cout << total_turnaround/num_processes << " " << total_initial/num_processes << " " << total_wait/num_processes << endl;
	

	total_turnaround = 0.0;
	total_wait = 0.0;
	total_initial = 0.0;
	max_wait = 0.0;
	min_wait = num_processes * 5000.0;
	max_initial = 0.0;
	min_initial = num_processes * 5000.0;
	max_turnaround = 0.0;
	min_turnaround = 5000.0 * num_processes;
	cout << endl;
}

//Simulates first come first serve
void first_come_first_serve(vector <process> processes, int num_processes)
{
	int num_completed = 0;
	int clock = 0;
	int last_pid = 0;
	vector <process> queue;
	bool cswitch = false;
        unsigned int index = processes.size();
	//Function runs until it detects it has completed every process
	while(num_completed != num_processes)
	{
		index = processes.size();
		for(unsigned int i = 0; i < index; i++)
		{
			if (processes[i].get_arrival() <= clock)
			{
				queue.push_back(processes[i]);
				// // cout << "[time " << processes[i].get_arrival() << "ms] Process " << processes[i].get_pid() << " created (requires " << 					processes[i].get_burst() << "ms CPU time)" << endl;
				processes.erase(processes.begin() + i);
				index--;
				i--;
			}
				
		}
	 
		if (queue.size() != 0)
		{
			if (cswitch)
			{
				//// cout << "[time " << clock - 7 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
				clock = clock + 7;
				cswitch = false;
			}
			if (queue[0].get_seen() == false)
			{
				//The initials will change in non zero arrival times
				int arriv = clock - queue[0].get_arrival();
				queue[0].seen(arriv);
				total_initial += arriv;
				if (arriv < min_initial)
					min_initial = arriv;
				if (arriv > max_initial)
					max_initial = arriv;
				//total_wait += clock;
				// cout << "[time " << clock << "ms] Process " << queue[0].get_pid() << " accessed CPU for the first time (initial wait time " << queue[0].get_initial_wait() << "ms)" << endl;
			}
			queue[0].dec_burst();
			if (queue[0].get_burst_left() == 0)
			{
				int fwait = ((clock + 1) - queue[0].get_burst() - queue[0].get_arrival());
				// cout << "[time " << clock + 1 << "ms] Process " << queue[0].get_pid() << " completed its CPU burst (turnaround time " << ((clock + 1) - queue[0].get_arrival()) << "ms, initial wait time " << queue[0].get_initial_wait() << "ms, total wait time " << fwait << "ms)" << endl;
				last_pid = queue[0].get_pid();
				int turn = ((clock + 1) - queue[0].get_arrival());
				total_turnaround += turn;
				total_wait += fwait;
				if (fwait < min_wait)
					min_wait = fwait;
				if (fwait > max_wait)
					max_wait = fwait;
				if (turn < min_turnaround)
					min_turnaround = turn;
				if (turn > max_turnaround)
					max_turnaround = turn;
				queue.erase(queue.begin());
				num_completed++;

				if (queue.size() != 0)
				{
					// cout << "[time " << clock + 1 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
					clock = clock + context_switch;
				}
				else
				{
					cswitch = true;
					//Releases finished process
					clock = clock + 7;
				}
			}
		}
	   clock++;
	}
  
}

//Simulates SJF non-preemptive 
void shortest_job_first(vector <process> processes, int num_processes)
{
	int num_completed = 0;
	int clock = 0;
	int last_pid = 0;
	vector <process> queue;
	vector <process> temp;
	bool cswitch = false;
        unsigned int index = processes.size();
	while(num_completed != num_processes)
	{
		index = processes.size();
		for(unsigned int i = 0; i < index; i++)
		{
			if (processes[i].get_arrival() <= clock)
			{
				temp.push_back(processes[i]);
				// // cout << "[time " << clock << "ms] Process " << processes[i].get_pid() << " created (requires " << 					processes[i].get_burst() << "ms CPU time)" << endl;
				processes.erase(processes.begin());
				index--;
				i--;
			}
				
		}

		sort(temp.begin(), temp.end(), sorter);
		for (unsigned int i = 0; i < temp.size(); i++)
		{
			queue.push_back(temp[i]);
		}
		temp.clear();
		//Sorts the processes by burst time length, ignores the currently running process because it is non-preemptive
		sort(queue.begin() + 1, queue.end(), sorter);
	 
		if (queue.size() != 0)
		{
			if (cswitch)
			{
				// // cout << "[time " << clock - 7 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
				clock = clock + 7;
				cswitch = false;
			}
			if (queue[0].get_seen() == false)
			{
				//Statistical info
				int arriv = clock - queue[0].get_arrival();
				queue[0].seen(arriv);
				total_initial += arriv;
				if (arriv < min_initial)
					min_initial = arriv;
				if (arriv > max_initial)
					max_initial = arriv;
				// cout << "[time " << clock << "ms] Process " << queue[0].get_pid() << " accessed CPU for the first time (initial wait time " << queue[0].get_initial_wait() << "ms)" << endl;
			}

			queue[0].dec_burst();

			if (queue[0].get_burst_left() == 0)
			{
				int fwait = ((clock + 1) - queue[0].get_burst() - queue[0].get_arrival());
				// cout << "[time " << clock + 1 << "ms] Process " << queue[0].get_pid() << " completed its CPU burst (turnaround time " << ((clock + 1) - queue[0].get_arrival()) << "ms, initial wait time " << queue[0].get_initial_wait() << "ms, total wait time " << fwait << "ms)" << endl;

				last_pid = queue[0].get_pid();
				int turn = ((clock + 1) - queue[0].get_arrival());
				total_turnaround += turn;
				total_wait += fwait;
				if (fwait < min_wait)
					min_wait = fwait;
				if (fwait > max_wait)
					max_wait = fwait;
				if (turn < min_turnaround)
					min_turnaround = turn;
				if (turn > max_turnaround)
					max_turnaround = turn;
				queue.erase(queue.begin());

				num_completed++;

				if (queue.size() != 0)
				{
					// cout << "[time " << clock + 1 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
					clock = clock + context_switch;
				}
				else
				{
					cswitch = true;
					//Releases finished process
					clock = clock + 7;
				}
			}
		}
	   clock++;
	}
  
}


//Simulates Shortest Job remaining (preemptive SJF)
void shortest_job_remaining(vector <process> processes, int num_processes)
{
	int num_completed = 0;
	int clock = 0;
	int last_pid = 0;
	vector <process> queue;
	bool cswitch = false;
        unsigned int index = processes.size();
	while(num_completed != num_processes)
	{
		index = processes.size();
		for(unsigned int i = 0; i < index; i++)
		{
			if (processes[i].get_arrival() <= clock)
			{
				// cout << "[time " << clock << "ms] Process " << processes[i].get_pid() << " created (requires " << 					processes[i].get_burst() << "ms CPU time)" << endl;
				if (queue.size() != 0 && processes[i].get_burst() < queue[0].get_burst() && queue[0].get_burst_left() != queue[0].get_burst())
				{
					//Context switch on preemption
					// cout << "[time " << clock  << "ms] Context switch (swapped out process " << queue[0].get_pid() << " for process " << processes[i].get_pid() << ")" << endl;
					clock = clock + 14;
				}

				queue.push_back(processes[i]);
				sort(queue.begin(), queue.end(), sorter);
				processes.erase(processes.begin());
				index--;
				i--;
			}
				
		}
	 
		if (queue.size() != 0)
		{
			if (cswitch)
			{
				// cout << "[time " << clock - 7 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
					clock = clock + 7;
					cswitch = false;
			}
			if (queue[0].get_seen() == false)
			{
				int arriv = clock - queue[0].get_arrival();
				queue[0].seen(arriv);
				total_initial += arriv;
				if (arriv < min_initial)
					min_initial = arriv;
				if (arriv > max_initial)
					max_initial = arriv;
				// cout << "[time " << clock << "ms] Process " << queue[0].get_pid() << " accessed CPU for the first time (initial wait time " << queue[0].get_initial_wait() << "ms)" << endl;
			}
			queue[0].dec_burst();
			if (queue[0].get_burst_left() == 0)
			{
				int fwait = ((clock + 1) - queue[0].get_burst() - queue[0].get_arrival());
				// cout << "[time " << clock + 1 << "ms] Process " << queue[0].get_pid() << " completed its CPU burst (turnaround time " << ((clock + 1) - queue[0].get_arrival()) << "ms, initial wait time " << queue[0].get_initial_wait() << "ms, total wait time " << fwait << "ms)" << endl;

				last_pid = queue[0].get_pid();
				int turn = ((clock + 1) - queue[0].get_arrival());
				total_turnaround += turn;
				total_wait += fwait;
				if (fwait < min_wait)
					min_wait = fwait;
				if (fwait > max_wait)
					max_wait = fwait;
				if (turn < min_turnaround)
					min_turnaround = turn;
				if (turn > max_turnaround)
					max_turnaround = turn;
				queue.erase(queue.begin());

				num_completed++;

				if (queue.size() != 0)
				{
					// cout << "[time " << clock + 1 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
					clock = clock + context_switch;
				}
				else
				{
					cswitch = true;
					//Releases finished process
					clock = clock + 7;
				}
			}
		}
	   clock++;
	}
  
}

//Simulates Round Robin Scheduling 
void round_robin(vector <process> processes, int num_processes)
{
	int num_completed = 0;
	int clock = 0;
	int last_pid = 0;

	//Sets the timeslice size
	int timeslice = 2;
	int current_slice = 0;
	vector <process> queue;
        unsigned int index = processes.size();
	bool cswitch = false;
	while(num_completed != num_processes)
	{
		for(unsigned int i = 0; i < index; i++)
		{
			if (processes[i].get_arrival() <= clock)
			{
				queue.push_back(processes[i]);
				// cout << "[time " << clock << "ms] Process " << processes[i].get_pid() << " created (requires " << 					processes[i].get_burst() << "ms CPU time)" << endl;
				processes.erase(processes.begin());
				index--;
				i--;
			}
				
		}
	 
		if (queue.size() != 0)
		{
			if (cswitch)
			{
				// cout << "[time " << clock - 7 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
					clock = clock + 7;
					cswitch = false;
			}
			if (queue[0].get_seen() == false)
			{
				
				int initial = (clock - queue[0].get_arrival());
				queue[0].seen(initial);
				total_initial += initial;
				if (initial  < min_initial)
					min_initial = initial;
				if (initial > max_initial)
					max_initial = initial;
				
				// cout << "[time " << clock << "ms] Process " << queue[0].get_pid() << " accessed CPU for the first time (initial wait time " << queue[0].get_initial_wait() << "ms)" << endl;
			}
			queue[0].dec_burst();
			current_slice++;
			if (queue[0].get_burst_left() == 0)
			{
				int total_wait_p = ((clock + 1) - queue[0].get_arrival() - queue[0].get_burst());
				// cout << "[time " << clock + 1 << "ms] Process " << queue[0].get_pid() << " completed its CPU burst (turnaround time " << ((clock + 1) - queue[0].get_arrival()) << "ms, initial wait time " << queue[0].get_initial_wait() << "ms, total wait time " << total_wait_p << "ms)" << endl;
				last_pid = queue[0].get_pid();
				int turnaround = ((clock + 1) - queue[0].get_arrival());
				total_turnaround += turnaround;
				total_wait += total_wait_p;
				if (total_wait_p < min_wait)
					min_wait = total_wait_p;
				if (total_wait_p > max_wait)
					max_wait = total_wait_p;
				if ((turnaround) < min_turnaround)
					min_turnaround = turnaround;
				if ((turnaround) > max_turnaround)
					max_turnaround = turnaround;
				queue.erase(queue.begin());
				num_completed++;
				if (queue.size() != 0)
				{
					// cout << "[time " << clock + 1 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
					clock = clock + context_switch;
				}
				else
				{
					cswitch = true;
					//Releases finished process
					clock = clock + 7;
				}
				current_slice = 0;
			}
			else if (current_slice == timeslice) //When timeslice is over
			{
				last_pid = queue[0].get_pid();
				queue.push_back(queue[0]);
				queue.erase(queue.begin());
				current_slice = 0;
				// cout << "[time " << clock + 1 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
				clock = clock + context_switch;
				
			}
		}
	   clock++;
	}
  
}


//Simulates priority scheduling
void priority(vector <process> processes, int num_processes)
{
	int num_completed = 0;
	int clock = 0;
	int last_pid = 0;

	//Sets the timeslice size
	int timeslice = 2;
	int current_slice = 0;
	vector <process> queue;
	vector <process> temp_queue;
    unsigned int index = processes.size();
	unsigned int index2;
	bool cswitch = false;
	int most_priority = 6;
	while(num_completed != num_processes)
	{
		for(unsigned int i = 0; i < index; i++)
		{
			if (processes[i].get_arrival() <= clock)
			{
				temp_queue.push_back(processes[i]);
				index2 = temp_queue.size();
				// cout << "[time " << clock << "ms] Process " << processes[i].get_pid() << " created (requires " << 					processes[i].get_burst() << "ms CPU time  Priority: " <<  processes[i].get_priority() << ")" << endl;
				sort(temp_queue.begin(), temp_queue.end(), sorter2);
				//Finds highest priority present
				if(processes[i].get_priority() < most_priority)
					most_priority = processes[i].get_priority();
				processes.erase(processes.begin());
				index--;
				i--;
			}
				
		}
		index2 = temp_queue.size();
		for(unsigned int i = 0; i < index2; i++)
		{
			if (temp_queue[i].get_priority() == most_priority)
			{
				if (queue.size() != 0 && temp_queue[i].get_priority() < queue[0].get_priority())
				{
					// cout << "[time " << clock  << "ms] Context switch (swapped out process " << queue[0].get_pid() << " for process " << temp_queue[i].get_pid() << ")" << endl;
					clock = clock + 14;
					for (unsigned int x = 0; x < queue.size(); x++)
					{
						temp_queue.push_back(queue[x]);
					}
					queue.clear();
				}
				queue.push_back(temp_queue[i]);
				temp_queue.erase(temp_queue.begin());
				index2--;
				i--;
			}
		
		}
	 	//Uses round robin for multiple processes with same highest priority
		if (queue.size() != 0 && queue.size() > 1)
		{

			if (cswitch)
			{
				// cout << "[time " << clock - 7 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
					clock = clock + 7;
					cswitch = false;
			}
			if (queue[0].get_seen() == false)
			{
				//The initials will change in non zero arrival times
				int initial = (clock - queue[0].get_arrival());
				queue[0].seen(initial);
				total_initial += initial;
				if (initial  < min_initial)
					min_initial = initial;
				if (initial > max_initial)
					max_initial = initial;
				//total_wait += initial;
				// cout << "[time " << clock << "ms] Process " << queue[0].get_pid() << " accessed CPU for the first time (initial wait time " << queue[0].get_initial_wait() << "ms)" << endl;
			}
			queue[0].dec_burst();
			current_slice++;
			if (queue[0].get_burst_left() == 0)
			{
				int total_wait_p = ((clock + 1) - queue[0].get_arrival() - queue[0].get_burst());
				// cout << "[time " << clock + 1 << "ms] Process " << queue[0].get_pid() << " completed its CPU burst (turnaround time " << ((clock + 1) - queue[0].get_arrival()) << "ms, initial wait time " << queue[0].get_initial_wait() << "ms, total wait time " << total_wait_p << "ms)" << endl;
				last_pid = queue[0].get_pid();
				int turnaround = ((clock + 1) - queue[0].get_arrival());
				total_turnaround += turnaround;
				total_wait += total_wait_p;
				if (total_wait_p < min_wait)
					min_wait = total_wait_p;
				if (total_wait_p > max_wait)
					max_wait = total_wait_p;
				if ((turnaround) < min_turnaround)
					min_turnaround = turnaround;
				if ((turnaround) > max_turnaround)
					max_turnaround = turnaround;
				queue.erase(queue.begin());
				num_completed++;
				if (queue.size() != 0)
				{
					// cout << "[time " << clock + 1 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
					clock = clock + context_switch;
				}
				else
				{
					cswitch = true;
					//Releases finished process
					clock = clock + 7;
					if (temp_queue.size() != 0)
						most_priority = temp_queue[0].get_priority();
					else
						most_priority++;
				}
				current_slice = 0;
			}
			else if (current_slice == timeslice)
			{
				last_pid = queue[0].get_pid();
				queue.push_back(queue[0]);
				queue.erase(queue.begin());
				current_slice = 0;
				// cout << "[time " << clock + 1 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
				clock = clock + context_switch;
				
			}
		
		}
		else if (queue.size() == 1) //Process runs unhindered when theres only 1 process of highest priority
		{
			if (cswitch)
			{
				// cout << "[time " << clock - 7 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
					clock = clock + 7;
					cswitch = false;
			}
			if (queue[0].get_seen() == false)
			{
				//The initials will change in non zero arrival times
				int arriv = clock - queue[0].get_arrival();
				queue[0].seen(arriv);
				total_initial += arriv;
				if (arriv < min_initial)
					min_initial = arriv;
				if (arriv > max_initial)
					max_initial = arriv;
				//total_wait += clock;
				// cout << "[time " << clock << "ms] Process " << queue[0].get_pid() << " accessed CPU for the first time (initial wait time " << queue[0].get_initial_wait() << "ms)" << endl;
			}
			queue[0].dec_burst();
			if (queue[0].get_burst_left() == 0)
			{
				int fwait = ((clock + 1) - queue[0].get_burst() - queue[0].get_arrival());
				// cout << "[time " << clock + 1 << "ms] Process " << queue[0].get_pid() << " completed its CPU burst (turnaround time " << ((clock + 1) - queue[0].get_arrival()) << "ms, initial wait time " << queue[0].get_initial_wait() << "ms, total wait time " << fwait << "ms)" << endl;
				last_pid = queue[0].get_pid();
				int turn = ((clock + 1) - queue[0].get_arrival());
				total_turnaround += turn;
				total_wait += fwait;
				if (fwait < min_wait)
					min_wait = fwait;
				if (fwait > max_wait)
					max_wait = fwait;
				if (turn < min_turnaround)
					min_turnaround = turn;
				if (turn > max_turnaround)
					max_turnaround = turn;
				queue.erase(queue.begin());
				num_completed++;
				if (queue.size() != 0)
				{
					// cout << "[time " << clock + 1 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
					clock = clock + context_switch;
				}
				else
				{
					cswitch = true;
					//Releases finished process
					clock = clock + 7;
					if (temp_queue.size() != 0)
						most_priority = temp_queue[0].get_priority();
					else
						most_priority++;
				}
			}
		}
		else if (queue.size() == 0)
		{
			if (temp_queue.size() != 0)
				most_priority = temp_queue[0].get_priority();
			else
				most_priority++;
		}
	  clock++;
	}

}

int main(int argc, char *argv[])
{
	srand(time(NULL));

    if (argc < 2)
    {
        cout << "Usage: ./a.out num_processes" << endl;
        return 1;
    }

	//Sets number of processes
	int num_processes = atoi(argv[1]);


	int random_burst;
	int _priority;

	//vector will serve as the ready queue
	vector <process> processes;

	//all of the processes have a semi random arrival time determined by exponential distribution except 1

	int num_zero = 1;
	int num_rand = num_processes - 1;

	/* Part A begins*/

	// define random number generators
	random_device random_dev;
	default_random_engine generator(random_dev());
	uniform_int_distribution<int> burst_dist(1, 20);
	uniform_int_distribution<int> priority_dist(1, 10);
	uniform_real_distribution<float> R_dist(0.0, 1.0);



	//Creates processes with zero arrival time
	for (int i = 0; i < num_zero; i++)
	{
		random_burst = burst_dist(generator);
		_priority = priority_dist(generator);
		process temp(random_burst, 0, i+1, _priority);
		processes.push_back(temp);
	}

	//Creates processes with non zero arrival time
	for(int i = 0; i < num_rand; i++)
	{
		random_burst = burst_dist(generator);
		_priority = priority_dist(generator);
		double mu = 0.2;
		double r = (double)R_dist(generator);
		double x = -(log(r)/mu);
		if ( x > 8000 ) { i--; continue; }
		process temp(random_burst, (int)x, (num_zero + i+1), _priority);
		processes.push_back(temp);
    }

	//Sorts processes by arrival time
	sort(processes.begin(), processes.end(), sorter3);

	//Assigns pids 1 - n based on arrival time. Makes the output easier to follow
	for (unsigned int i = 0; i < processes.size(); i++)
	{
		processes[i].set_pid(i+1);
	}

	// write the processes to 'process_table.txt'
	ofstream outfile;
	outfile.open("process_table.txt");
	outfile << "PID    Burst    Arrival    Priority" << endl;
	for (unsigned int i = 0; i < processes.size(); i++)
	{
		outfile << " " << processes[i].get_pid() << "\t\t" << processes[i].get_burst() << "\t\t  " << processes[i].get_arrival() << "\t\t\t" << processes[i].get_priority() << endl;
	}
	outfile.close();

	/* End of Part-A */

	
	//Sets variables for statistical information
	max_wait = 0.0;
    min_wait = num_processes * 5000.0;
    max_initial = 0.0;
	min_initial = num_processes * 5000.0;

	
	/*Part-B and Part-C*/
	//FCFS
    first_come_first_serve(processes, num_processes);
	cout << endl;

	//Statistics
	statistics(num_processes, "FCFS");

	//Non-Preemptive SJF
	shortest_job_first(processes, num_processes);
	cout << endl;

	statistics(num_processes, "SJF, non-preemptive");

	//Shortest_job_remaining or preemptive sjf
	shortest_job_remaining(processes, num_processes);
	cout << endl;

	statistics(num_processes, "SJF, preemptive");


	//Round Robin
	round_robin(processes, num_processes);
	cout << endl;

	statistics(num_processes, "Round Robin");
	
	//Priority
	priority(processes, num_processes);
	cout << endl;

	statistics(num_processes, "Priority");

	cout << "The process table was generated and saved in process_table.txt" << endl;
	
	
  return 0;
	
}