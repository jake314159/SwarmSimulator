import json
import sys

from os import listdir
from os.path import isfile, join

import matplotlib.pyplot as plt
import numpy as np

import matplotlib.cm as cm
import matplotlib.mlab as mlab
from matplotlib import gridspec

import math

 # Takes as the first argument the file path for the output json from
 # VisSpreadCalc.cpp

json_data=open(sys.argv[1]).read()
data = json.loads(json_data)
data.pop() #Last element is empty so throw it away

x = []
y = []
e = []

for point in data:
	x.append(point["visibility"])
	mean = 0
	sd = 0
	for val in point["spread"]:
		mean += val
	mean /= len(point["spread"])

	for val in point["spread"]:
		sd += (mean-val)**2
	sd /= len(point["spread"])
	sd = math.sqrt(sd)

	y.append(mean)

	e.append(sd*1.959964) #95% margin of error

import numpy as np
import matplotlib.pyplot as plt



fig, (ax0) = plt.subplots(nrows=1)
ax0.errorbar(x, y, yerr=e, fmt='-o')
ax0.set_title('How visibility affects the spread of the swarm\n')
ax0.set_ylabel('Spread')
ax0.set_xlabel('Visibility (px)')
#ax1.errorbar(x, y, xerr=asymmetric_error, fmt='o')
#ax1.set_title('variable, asymmetric error')
#ax1.set_yscale('log')
plt.show()