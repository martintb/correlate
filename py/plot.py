import numpy as np
import matplotlib.pyplot as plt
import ipdb; ist = ipdb.set_trace

with open('calc.dat','r') as f:
  keys = f.readline()[1:].split()
hist = np.loadtxt('calc.dat')

data = {k:v for k,v in zip(keys,hist.T)}

plt.plot(data['x'],data['rdf'])
plt.show()

