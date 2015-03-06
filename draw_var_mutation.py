import json
import sys
from pprint import pprint

import matplotlib.pyplot as plt
import numpy as np

import matplotlib.cm as cm
import matplotlib.mlab as mlab

import random
import math

mu_rate_start = 0.2
mu_rate_end = 0.001
round_N = 3000
mutation_delta = math.exp((math.log(mu_rate_end/mu_rate_start))/(round_N))
print(mutation_delta)

data = [mu_rate_start]
for i in range(round_N):
    data.append(data[-1]*mutation_delta)

#pprint(t)
plt.figure(1)
plt.plot(data)
plt.ylabel('Mutation rate')
plt.xlabel('Round number')
plt.title('How the mutation rate varies over a 3000 round evolution cycle\n')
plt.show()
