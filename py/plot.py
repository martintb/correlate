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
x = data[0]
y = data[1:]
print 'xshape:',x.shape
print 'yshape:',y.shape
y = np.average(y,axis=0)

N=50
fac2 = np.array([2*(N-i-1) for i in range(N)])[:-1]
fac1 = np.array([2*(N-i) for i in range(N)])[:-1]
# y*= 1.0/(x*fac2*8000/fac1)
y*= 1.0/(x*8000)

kw ={}
kw['lw'] = 1.25
kw['marker'] = 'o'
kw['fillstyle'] = 'none'
plt.plot(x,y,**kw)
plt.show()

