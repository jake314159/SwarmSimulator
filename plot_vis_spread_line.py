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

 # Takes as the arguments the file path for the output json from
 # VisSpreadCalc.cpp. Note any number of these files can be passed as arguments
 # and they will all be merged together before being displayed


points = []
vals = dict()

for f in sys.argv[1:]:
	print("Loading file %s"%f)
	json_data=open(f).read()
	d = json.loads(json_data)
	d.pop()
	for point in d:
		if point["visibility"] == 65:
			#On the edge so ignore :/
			continue
		if (point["proj_w"]>0.1 or point["align_w"]>0.1) and (point["proj_w"]<0.2 or point["align_w"]<0.2):
			continue
		#if (point["proj_w"]<0.1 and point["align_w"]<0.1):
		#	continue
		points.append(point["visibility"])
		av_point = 0
		for d in point["spread"]:
			av_point += d
		av_point /= len(point["spread"])

		print("Visibility %d\t(%f,%f)\tHas a average spread of %d"%(point["visibility"],point["proj_w"],point["align_w"],av_point))

		if point["visibility"] in vals:
			#print("%i is already here"%point["visibility"])
			vals[point["visibility"]].extend(point["spread"])
#			vals[point["visibility"]].append(av_point)
		else:
			#print("Not seen %i before"%point["visibility"])
			vals[point["visibility"]] = point["spread"]
#			vals[point["visibility"]] = [av_point]

#print(json.dumps(vals, sort_keys=True, indent=4, separators=(',', ': ')))

for p in points:
	if True:
		break
	while len(vals[p]) > 200:
		vals[p].sort()
		for i in range(1,7):
			vals[p].pop()
		vals[p].sort(reverse=True)
		for i in range(1,5):
			vals[p].pop()

x = []
y = []
e = []

points.sort()

for point in points:
	x.append(point)
	mean = 0
	sd = 0
	for val in vals[point]:
		mean += val
	mean /= len(vals[point])

	for val in vals[point]:
		sd += (mean-val)**2
	sd /= len(vals[point])
	sd = math.sqrt(sd)

	y.append(mean)

	e.append(sd*1.959964) #95% confidence level

import numpy as np
import matplotlib.pyplot as plt



fig, (ax0) = plt.subplots(nrows=1)
#ax0.set_ylim([0, 250])
ax0.errorbar(x, y, yerr=e, fmt='-o')
ax0.set_title('How visibility affects the spread of the swarm\n')
ax0.set_ylabel('Spread')
ax0.set_xlabel('Visibility (px)')
#ax1.errorbar(x, y, xerr=asymmetric_error, fmt='o')
#ax1.set_title('variable, asymmetric error')
#ax1.set_yscale('log')
plt.show()
