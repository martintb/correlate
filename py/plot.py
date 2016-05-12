import numpy as np
import matplotlib.pyplot as plt
import ipdb; ist = ipdb.set_trace

# with open('calc.dat','r') as f:
#   keys = f.readline()[1:].split()
# hist = np.loadtxt('calc.dat')
# 
# data = {k:v for k,v in zip(keys,hist.T)}

data = np.loadtxt('calc.dat')

r   = [j for i,j in enumerate(data) if i%2==0]
rdf = [j for i,j in enumerate(data) if i%2==1]
print len(r),len(rdf)

r   = np.average(r,axis=0)
rdf = np.average(rdf,axis=0)

plt.plot(r,rdf)
plt.show()

