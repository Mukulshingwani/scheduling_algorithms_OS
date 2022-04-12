import os
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import json

sns.set()

def main():

    n_vals = [10, 20, 30, 40, 50]
    NUM_SAMPLES = 10
    algos = ['FCFS', 'SJF Non-Preemptive', 'SJF Preemptive', 'RoundRobin', 'Priority']

    ATT = {}
    ART = {}
    AWT = {}
    
    for n in n_vals:

        n_ATT = {x: [] for x in algos}
        n_ART = {x: [] for x in algos}
        n_AWT = {x: [] for x in algos}

        for i in range(NUM_SAMPLES):
            os.system(f'out/scheduling_sim {n} > temp.txt')

            algo_scores = []
            with open('temp.txt') as f:
                for line in f:
                    x = line.replace('\n', '')

                    if x != '':
                        algo_scores.append(x)

            algo_scores = algo_scores[4::5]

            for i, scores in enumerate(algo_scores):
                sc = [float(x) for x in scores.split()]
                n_ATT[algos[i]].append(sc[0])
                n_ART[algos[i]].append(sc[1])
                n_AWT[algos[i]].append(sc[2])
        
        ATT[n] = n_ATT
        ART[n] = n_ART
        AWT[n] = n_AWT
    

    print('Demo?')

    print('Data for ATT')

    for k, v in AWT.items():
        print(f'\tN={k}')

        for algo, data in v.items():
            print(f'\t\tAlgo: {algo}')
            print(f'\t\t\tmin={min(data)}')
            print(f'\t\t\tmax={max(data)}')
            print(f'\t\t\tavg={round(sum(data)/len(data), 3)}')
    
    for metrics in ['ATT', 'ART', 'AWT']:
        logs = eval(metrics)
        _min = {algo: [] for algo in algos}
        _max = {algo: [] for algo in algos}
        _avg = {algo: [] for algo in algos}

        for _, proc_logs in logs.items():
            for algo, data in proc_logs.items():
                _min[algo].append(min(data))
                _max[algo].append(max(data))
                _avg[algo].append(sum(data) / NUM_SAMPLES)
        
        for g in ['_min', '_max', '_avg']:
            th = eval(g)
            log_data = pd.DataFrame({
                'Number of Processes': n_vals,
                **th
            })
            plot = sns.lineplot(x='Number of Processes', y='Time (ms)', hue='Scheduling Algorithm', data=pd.melt(log_data, ['Number of Processes'], value_name='Time (ms)', var_name='Scheduling Algorithm'))
            plot.set_title(f'{g[1:].upper()}. {metrics}')
            plot.get_figure().savefig(f'plots/{metrics + g}.png')
            plt.clf()

    
    os.system('rm temp.txt')



                


if __name__ == '__main__':
    main()