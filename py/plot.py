import numpy as np
import matplotlib.pyplot as plt
import ipdb; ist = ipdb.set_trace
import sys

# with open('calc.dat','r') as f:
#   keys = f.readline()[1:].split()
# hist = np.loadtxt('calc.dat')
# 
# data = {k:v for k,v in zip(keys,hist.T)}

data = np.loadtxt(sys.argv[1])
r = data[0]
rdf = data[1:]
print r.shape
print rdf.shape
rdf = np.average(rdf,axis=0)

plt.plot(r,rdf)
plt.show()

