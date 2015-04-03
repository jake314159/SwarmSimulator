import json
import sys
from pprint import pprint

import matplotlib.pyplot as plt
import numpy as np

import matplotlib.cm as cm
import matplotlib.mlab as mlab

import random
import math

import simplejson
from matplotlib.colors import LogNorm

## Takes the ourput from DataCollector and creates
## a heat map showing the values

json_data=open(sys.argv[1])

data = json.load(json_data)
#pprint(data)
json_data.close()

grid_n = len(data)
row_n = len(data[0])
col_n = len(data[0][0])
sample_n = len(data[0][0][0])
#pprint(data[0])

samplePoint = sample_n/4

print("Grid number   :%d"%grid_n)
print("Row number    :%d"%row_n)
print("Col number    :%d"%col_n)
print("Sample number :%d"%sample_n)
print("")

pair = 1 #spread or speed?
limit = None
if pair == 0:
    limit = 120
elif pair == 1:
    limit = 5

grid = [[[float('NaN') for s in range(sample_n)] for x in range(col_n)] for x in range(row_n)] 

for x in range(len(data[0])-1):
    for y in range(len(data[0][x])-1):
        for s in range(len(data[0][x][y])-1):#range(sample_n-1):
            try:
                total = 0
                count = 0
                #print("(%d,%d,%d)"%(x,y,s))
                for g in data:
                    try:
                        total += g[x][y][s][pair]
                        count += 1
                    except IndexError:
                        pass
                if count >0:
                    average = total/count
                    #sample_value = random.randint(0,4);
                    grid[x][y][s] = average #float(arr2[x][y][1:(len(arr2[x][y])-2)].split(',')[sample_value])
                    if x+y >= 100:
                        grid[x][y][s] = float('NaN')
                    if grid[x][y][s] < 0:
                        grid[x][y][s] = grid[x-1][y][s] #int max
            except ValueError:
                grid[x][y][s] = float('NaN')

# The mean value for each place
grid2 = [[float('NaN') for x in range(col_n)] for x in range(row_n)]
for x in range(row_n-1):
    for y in range(col_n-1):
        try:
            if len(data[0][x][y]) == 0:
                continue
            #sample_value = random.randint(0,4);
            print("%f at %d,%d"%(data[0][x][y][samplePoint][pair], x, y))
            grid2[y][x] = float(data[0][x][y][samplePoint][pair])#grid[x][y][1] #float(arr2[x][y][1:(len(arr2[x][y])-2)].split(',')[sample_value])
            print(grid2[x][y])
            #if x+y >= 100:
             #   grid2[x][y] = float('NaN')
            #if grid2[x][y] < 0:
            #    grid2[x][y] = grid2[x-1][y] #int max
        except ValueError:
            grid2[x][y] = float('NaN')


## Plot percentile line

def f(t):
    for sample in range(len(t)):
        s = int(t[sample])
        error = 0
        count = 0
    
        for x in range(row_n-1):
            for y in range(col_n-1):
                if not math.isnan(grid[x][y][s]):
                    for g in range(grid_n):
                        d = []
                        for point in data[g][x][s]:
                            d.append(point[pair])
                        if len(d) ==0:
                            continue
                        print("e    "+str(s)+" "+str(len(d)) + " "+str(d[s]))
                        error += (d[s]-grid[x][y][s])**2
                        count += 1
        t[sample] = error
    #pprint(t)
    return t

t1 = np.arange(0.0, sample_n, 1)
t2 = np.arange(0.0, sample_n, 1)#np.arange(0.0, 5.0, 0.02)

#plt.figure(1)
#plt.subplot(211)
#plt.plot(t1, f(t1), 'bo', t2, f(t2), 'k')

#plt.subplot(212)
#plt.plot(t2, np.cos(2*np.pi*t2), 'r--')
#plt.show()

t = list(range(sample_n-1))
for i in range(len(t)):
    t[i] = t[i]*(100/(sample_n-1))+1

percentile_error_data = f(list(range(sample_n-1)))

best_percentile = 0
for i in range(len(t)):
    if percentile_error_data[i] < percentile_error_data[best_percentile]:
        best_percentile = i

best_percentile = t[best_percentile]
print("The best percentile is %d"%best_percentile)

#pprint(t)
plt.figure(1)
plt.plot(t, percentile_error_data)
plt.ylabel('Noise estimated with the mean squared error of multiple runs')
plt.xlabel('Percentile measured')
plt.title('Comparing the noise present for different sample percentiles\n')
plt.show()

# The next graph is boring so lets exit before we get there
#sys.exit(0)

# PLOT
methods = ['none'] #[None, 'none', 'nearest', 'bilinear', 'bicubic', 'spline16',
          # 'spline36', 'hanning', 'hamming', 'hermite', 'kaiser', 'quadric',
          # 'catrom', 'gaussian', 'bessel', 'mitchell', 'sinc', 'lanczos']

print(grid2)

X = grid2#10*np.random.rand(5,3)
fig, ax = plt.subplots()
#colormap = cm.gray
colormap = cm.jet
masked_array = np.ma.array(grid2, mask=np.isnan(grid2))
#colormap.set_bad('w',1.)
im = ax.imshow(masked_array, interpolation='none', cmap=colormap, extent=[0,1,1,0],vmin=0.000001, vmax=limit)

#colormap.set_bad(color=u'k', alpha=None)
#ax.imshow(X, cmap=colormap, interpolation='none')


numrows, numcols = [len(grid[0]), len(grid)]
def format_coord(x, y):
    col = int(x*numcols+0.5)
    row = int(y*numrows+0.5)
    return "TODO"
    if col>=0 and col<numcols and row>=0 and row<numrows:
        z = grid[row][col]
        return 'x=%1.4f, y=%1.4f, z=%1.4f'%(x, y, z)
    else:
        return 'x=%1.4f, y=%1.4f'%(x, y)

type_string = ["spread", "speed"]

plt.gca().invert_yaxis()
ax.format_coord = format_coord
plt.ylabel('Projection weight')
plt.xlabel('Alignment weight')
plt.title('How the simulation parameters affects\n the swarms '+type_string[pair])
fig.colorbar(im)
plt.show()

