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

files = [ f for f in listdir(path) if isfile(join(path,f)) ]
files.sort()

max_dic = {}

## Collect data from the json files
max_values = []
min_values = []
all_values = []
all_results = [] #Tuples of the best locations
i = 0
successful_mutations = []
for f in files:
    f = path + "/" + f
    print("Loading file %s"%(f))

    json_data=open(f).read()
    data = json.loads(json_data)

    max_d = data['population'][0]

    find_string = (max_d['proj_weight'], max_d['align_weight'])
    if find_string in max_dic:
        print("I've seen this before")
        if flatten_graph:
            max_d['score'] = max_dic[find_string]
    else:
        max_dic[find_string] = max_d['score']

    max_values.append(int(max_d['score']))
    min_values.append(int(data['population'][len(data['population'])-1]['score']))
    mutate_count = 0
    j = 0
    for p in data['population']:
        if p['source'] == 'mutate':
            mutate_count += 1
        if len(all_values) <= j:
            all_values.append([])
        all_values[j].append(int(p['score']))

        if len(all_results) <= j:
            all_results.append([])
        all_results[j].append( (p['proj_weight'], p['align_weight'], p['score']) )

        j += 1
    successful_mutations.append(mutate_count)

    i += 1

max_values = []
min_values = []
for i in range(len(all_values[0])):
    max_v = -1
    min_v = 10000000000000
    for j in range(len(all_values)):
        if all_values[j][i] > max_v:
            max_v = all_values[j][i]
        if all_values[j][i] < min_v:
            min_v = all_values[j][i]
    max_values.append(max_v)
    min_values.append(min_v)

#### Plot where the points are
plt.figure(1)

def plot(result, color="#000000"):
    x_vals = []
    y_vals = []
    z_vals = []
    for e in result:
        x_vals.append(e[0])
        y_vals.append(e[1])
        z_vals.append(e[2])
    plt.plot(x_vals, y_vals, color=color)
    plt.plot(x_vals[len(x_vals)-1], y_vals[len(y_vals)-1], 'o',color=color)

#Plot grey lines for each member of the population
for result in all_results:
    plot(result, "#cccccc")

# Plost best and worst points
best_v = -1
best_result = []
worst_v = 100000000000000000000
worst_result = []
for result in all_results:
    if int(result[len(result)-1][2]) > best_v:
        best_result = result
        best_v = int(result[len(result)-1][2])
    if int(result[len(result)-1][2]) < worst_v:
        worst_result = result
        worst_v = int(result[len(result)-1][2])

plot(best_result, "#00ff00")
plot(worst_result, "#ff0000")

#Plot boundry line
plt.plot([0, 1], [1, 0], color="#bbbbbb")

plt.ylim([0, 1])
plt.xlim([0, 1])
plt.ylabel('Alignment weight')
plt.xlabel('Projection weight')
plt.title('Location of each individual in a population and\nwhere they are in the search space')
plt.show()
#for v in all_values[1:-1]:
#    plt.plot(v, color="#999999")

max_avr = []
if(len(max_values)>0):
    max_avr.append(max_values[0])
if(len(max_values)>1):
    max_avr.append((max_values[0]+max_values[1])/2)
for i in range(2,len(max_values)):
    max_avr.append((max_values[i-2]+max_values[i-1]+max_values[i])/3)

min_avr = []
if(len(min_values)>0):
    min_avr.append(min_values[0])
if(len(min_values)>1):
    min_avr.append((min_values[0]+min_values[1])/2)
for i in range(2,len(min_values)):
    min_avr.append((min_values[i-2]+min_values[i-1]+min_values[i])/3)

###### Plot line graph for each round
plt.figure(1)
gs = gridspec.GridSpec(2, 1, height_ratios=[3, 1]) 

plt.subplot(gs[0])

r = []
for i in range(0,len(max_values)):
    r.append(i)

#Plot grey range values
plt.fill_between(r, min_values, max_values, facecolor='gray', alpha=0.5)

#Plot grey lines for each member of the population
for v in all_values[1:-1]:
    plt.plot(v, color="#999999")

#Plot the min and max lines
plt.plot(max_values)
plt.plot(min_values)

plt.ylim([0, None])
plt.xlim([0, r[len(max_values)-1]])

plt.ylabel('Value')
#plt.xlabel('Round')
plt.title('Score of the best and worst individuals in the population for each round\n')
#plt.show()

## Plot successful mutations per round
#plt.figure(1)
plt.subplot(gs[1])
plt.bar(r, successful_mutations)
plt.ylim([0, None])
plt.xlim([0, r[len(max_values)-1]])
plt.ylabel('Successful mutations')
plt.xlabel('Round')
#plt.title('How the number of successful mutations changes over each round\n')
plt.show()

#sys.exit(0)



###### 3D graph
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.pyplot as plt
import numpy as np

import math
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
#ax = fig.gca(projection='3d')
#X = np.arange(-5, 5, 0.25)
#Y = np.arange(-5, 5, 0.25)
#X, Y = np.meshgrid(X, Y)
#R = -np.sqrt(X**2 + Y**2)+10
#Z = R#np.sin(R)
#surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=cm.coolwarm,
#        linewidth=0, antialiased=False)
#ax.set_zlim(0, 1)

ax.zaxis.set_major_locator(LinearLocator(10))
ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

#fig.colorbar(surf, shrink=0.5, aspect=5)

def plot(result, c="#000000"):
    x_vals = []
    y_vals = []
    z_vals = []
    for e in result:
        x_vals.append(float(e[0]))
        y_vals.append(float(e[1]))
        z_vals.append(float(e[2]))
    ax.plot(x_vals, y_vals, zs=z_vals, color=c)
    #plt.plot(x_vals[len(x_vals)-1], y_vals[len(y_vals)-1], 'o',color=color)
    ax.scatter(x_vals[len(x_vals)-1], y_vals[len(y_vals)-1], z_vals[len(z_vals)-1], c=c, marker='o')

#Plot grey lines for each member of the population
for result in all_results:
    plot(result, "#cccccc")

# Plost best and worst points
best_v = -1
best_result = []
worst_v = 100000000000000000000
worst_result = []
for result in all_results:
    if int(result[len(result)-1][2]) > best_v:
        best_result = result
        best_v = int(result[len(result)-1][2])
    if int(result[len(result)-1][2]) < worst_v:
        worst_result = result
        worst_v = int(result[len(result)-1][2])

plot(best_result, "#00ff00")
plot(worst_result, "#ff0000")

#boundry line
ax.plot([1,0], [0,1], zs=[0,0], color="#999999")

ax.set_xlim(0, 1)
ax.set_ylim(0, 1)
ax.set_zlim(0, None)

plt.ylabel('Alignment weight')
plt.xlabel('Projection weight')
plt.title('Location of each individual in a population and\nwhere they are in the search space')

plt.show()




sys.exit(0)
# Plot max and min values for each round average
#plt.figure(1)
plt.plot(max_avr)
plt.plot(min_avr)

r = []
for i in range(0,len(max_values)):
    r.append(i)

plt.fill_between(r, min_avr, max_avr, facecolor='gray', alpha=0.5)

plt.ylim([0, None])
plt.xlim([0, None])

plt.ylabel('Value')
plt.xlabel('Round')
plt.title('Score of the best and worst individuals in the population for each round\nRunning average\n')
plt.show()
