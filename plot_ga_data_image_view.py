# plot_ga_data.py
# Plots the data from the new GA which is part of SwarmSimulator
#
# How to use:
#    python3 plot_ga_data.py ./data_dir
# or
#    python3 plot_ga_data.py ./data_dir mesh_data.json
#
# ./data_dir
#     Directory containing the json data outputted from the GA program. Should
#     be a series of files with incrementing file names from "000000.json"
#
# mesh_data.json
#     Data about the search space generated from the AnalyseSearchSpace
#     program (the stdout piped into a file) which contains information about
#     the search space the GA was trying to optimise in
#

# If there are lots of -ve values then flipping it is a good idea
flip = True

import json
import sys

from os import listdir
from os.path import isfile, join

import matplotlib.pyplot as plt
import numpy as np

import matplotlib.cm as cm
import matplotlib.mlab as mlab
from matplotlib import gridspec

flatten_graph = True
path = sys.argv[1]
mesh_data_path = None
if len(sys.argv)>2:
    mesh_data_path = sys.argv[2]

#files = [ f for f in listdir(path) if isfile(join(path,f)) ]
#files.sort()
files = [sys.argv[1]]

max_dic = {}

## Collect data from the json files
max_values = []
min_values = []
all_values = []
all_results = [] #Tuples of the best locations
i = 0
successful_mutations = []
for f in files:

    print("Loading file %s"%(f))

    json_data=open(f).read()
    data = json.loads(json_data)
    for i in range(len(data['population'])):
        data['population'][i]['score'] *= -1;

    all_results.append(data['population'])

    i += 1



#### Plot where the points are
plt.figure(1)

def plot(result, i, color="#000000"):
    x_vals = []
    y_vals = []
    z_vals = []
    for e in result:
        x_vals.append(e[0])
        y_vals.append(e[1])
        z_vals.append(e[2])
    #plt.plot(x_vals, y_vals, color=color)
    plt.plot(x_vals[i], y_vals[i], 'o',color=color)

print('You may need to make a directory called "images" to store the output images\n\n')

for i in range(len(all_results)):
    print('Drawing image number %06d'%(i))
    #Plot grey lines for each member of the population
    #for result in all_results:
    #    plot(result, i, "#cccccc")

    for j in range(len(all_results[i])):
        plt.plot(all_results[i][j]['proj_weight'], all_results[i][j]['align_weight'], 'o',color="#cccccc")

    #Plot boundry line
    plt.plot([0, 1], [1, 0], color="#bbbbbb")

    plt.ylim([0, 1])
    plt.xlim([0, 1])
    plt.ylabel('Alignment weight')
    plt.xlabel('Projection weight')
    plt.title('Location in the search space of each individual in a population\n')
    plt.show()
    #plt.savefig('images/%06d.png'%(i), bbox_inches='tight')
    plt.cla()
