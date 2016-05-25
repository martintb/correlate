# import ipdb; ist = ipdb.set_trace
from typy import analyze
from typy.analyze.trials import make 
import numpy as np
import re,os,time,cPickle
from functools import partial
from collections import defaultdict
import MDAnalysis
import subprocess,shlex

# trial_path = '/home/work/jayaraman_lab/martintb/bigpnc/trials/main'
# trial_key_file = os.path.join(trial_path,'trial_key.py')
# RE,SI = analyze.create.trialRE(trial_key_file, {}, sortKey=['fG','epsAC'])
# trialDictA = analyze.trials.parse(trial_path,RE,sort=SI,makeDict=True)

trial_path = '/home/work/jayaraman_lab/martintb/bigpnc/trials/small'
trial_key_file = os.path.join(trial_path,'trial_key.py')
RE,SI = analyze.create.trialRE(trial_key_file, {'fG':[0.7,1.0],'lenMatrix1':50}, sortKey=['fG','epsAC'])
trialDictA = analyze.trials.parse(trial_path,RE,sort=SI,makeDict=True)


trial_path = '/home/work/jayaraman_lab/martintb/bigpnc/trials/omega'
trial_key_file = os.path.join(trial_path,'trial_key.py')
RE,SI = analyze.create.trialRE(trial_key_file, {}, sortKey=['fG','epsAC'])
trialDictB = analyze.trials.parse(trial_path,RE,sort=SI,makeDict=True)

trialDict = {}
trialDict.update(trialDictA)
trialDict.update(trialDictB)

#think of better names for these variables 
rmax = 50
dr = 0.1
N = int(rmax/dr)
dk = np.pi/((N+1)*dr)
kmax = N*dk
num_frames_to_sample = 80
frame_skip = 1
frames_per_chunk = 10
num_chunks = int(num_frames_to_sample/frames_per_chunk)

pklKeys = [dr,rmax,'{:5.4f}'.format(dk),'{:4.2f}'.format(kmax),num_frames_to_sample,frame_skip,frames_per_chunk]
pklStr = '-'.join([str(i) for i in pklKeys])
dumpName = '/home/work/jayaraman_lab/martintb/bigpnc/pkl/SMALLCHI-{}.pkl'.format(pklStr)
updateAll = False

tempFile = 'temp.dat'
# topoFile = 'data.lmpbond'
# trjFile = 'trajectory.eq.dcd'
# topology_format = 'DATA'
xmlFile = 'initial.xml'
topoFile = 'hoomd.topo'
trjFile = 'eq_trajectory.dcd'
topology_format = 'xml'

pairs = []
pairs += [('P','P')]
pairs += [('P','G')]
pairs += [('P','M')]
pairs += [('G','G')]
pairs += [('G','M')]
pairs += [('M','M')]

pairMap = {}
pairMap['A']='1'
pairMap['B']='2'
pairMap['C']='3'
pairMap['D']='4'
pairMap['P']='6'
pairMap['G']='1,2'
pairMap['M']='3,4'

try:
  with open(dumpName,'rb') as f:
    dump = cPickle.load(f)
    print '.:: Successfully loaded',dumpName,'!' 
except IOError:
  print '.:: Making new',dumpName,'!' 
  dump={}

repSelect = ['0','L100']
repSelect = ['0']
for i,TP in enumerate(analyze.trials.loop(trialDict,repSelect=repSelect)):
  t1 = time.time()
  print 'Considering trial',i,'of',len(trialDict.keys())

  trialSpec = make.trialDict(trial_key_file,TP)
  trialSpec['pairs'] = pairs
  trialSpec['pairMap'] = pairMap

  if not os.path.exists(TP.make(topoFile)):
    print '.:: topoFile file doesn\'t exist! Skipping...'
    continue

  if not os.path.exists(TP.make(trjFile)):
    print '.:: trjFile doesn\'t exist! Skipping...'
    continue

  try:
    if topology_format=='DATA':
      universe = MDAnalysis.Universe(TP.make(topoFile),
                                   TP.make(trjFile),
                                   topology_format=topology_format)
    elif topology_format=='xml':
      universe = MDAnalysis.Universe(TP.make(xmlFile),
                                   TP.make(trjFile),
                                   topology_format=topology_format)
    else:
      raise ValueError('Incorrect topology_format')
  except IOError,OSError:
    print '.:: It appears that either the data or trj file are missing or corrupted. Skipping...'
    continue

  numFrames = universe.trajectory.n_frames
  trialSpec['numFrames'] = numFrames

  if not updateAll:
    if (TP.trial in dump) and (TP.rep in dump[TP.trial]):
      if 'numFrames' in dump[TP.trial][TP.rep]['spec']: 
        if numFrames==dump[TP.trial][TP.rep]['spec']['numFrames']:
          print '.:: --> Skipping {} because dump appears up to date'.format(TP.trial)
          continue


  print '.:: Updating',TP.trial,'/',TP.rep

  AG = {}
  AG['universe'] = universe
  AG['uni'] = universe
  print '.:: Created the following atomgroups:'
  for k,v in AG.items():
    print '({:>15s})'.format(k),v
    
  fd = analyze.trials.make.frameDictNoAG(trialPath = TP,
                                         numFrames=numFrames,
                                         samples = num_frames_to_sample,
                                         skipFrame=frame_skip,
                                         chunk_size=frames_per_chunk)
  if not fd:
    continue

  print '.:: Doing first pass reading of trajectory to get box information...'
  boxDD  = {}
  rhoDD  = {}
  phiDD  = {}
  for chunk in range(num_chunks):
    boxDD[chunk] = {}
    rhoDD[chunk] = {}
    phiDD[chunk] = {}

    print '.:: Printing data for chunk',chunk
    frameStart = fd['frames'][chunk,0]
    frameEnd =   fd['frames'][chunk,1]
    box_data = []
    for frameNo in range(frameStart,frameEnd):
      frame = AG['universe'].trajectory[frameNo]
      box_data.append(frame.dimensions[:3])

    L = np.average(box_data,axis=0)
    V = np.product(L)
    print '--> AVG BOXL:',L

    boxDD[chunk]['all']       = box_data
    boxDD[chunk]['L']       = L
    boxDD[chunk]['V']       = V
    #-------------------------------------------------
    lenGrafts        = float(trialSpec['lenGrafts'])
    numGrafts        = float(trialSpec['numGrafts'])
    numParticles     = float(trialSpec['numParticles'])
    particleDiameter = float(trialSpec['particleDiameter'])
    numMatrix1       = float(trialSpec['numMatrix1'])
    numMatrix2       = float(trialSpec['numMatrix2'])
    lenMatrix1       = float(trialSpec['lenMatrix1'])
    lenMatrix2       = float(trialSpec['lenMatrix2'])
    #-------------------------------------------------
    vD = (4.0/3.0) * np.pi * (particleDiameter/2.0)**(3.0)
    vd = (4.0/3.0) * np.pi * (1.0/2.0)**(3.0)
    vGrafts    =  vd * numGrafts * lenGrafts
    totVGrafts = vGrafts*numParticles
    vPGP       = vGrafts + vD
    totVPGP    = numParticles*vPGP
    totVMat    = vd * (numMatrix1*lenMatrix1 + numMatrix2*lenMatrix2)
    totV       = totVPGP + totVMat
    #-------------------------------------------------
    rhoDD[chunk]['P'] = numParticles/V
    rhoDD[chunk]['G'] = numParticles*numGrafts*lenGrafts/V
    rhoDD[chunk]['M'] = numMatrix1*lenMatrix1/V
    for k,v in rhoDD[chunk].items():
      print '--> Type: ',k,'site_density:',v
    #-------------------------------------------------
    phiDD[chunk]['3COMP'] = {}
    phiDD[chunk]['3COMP']['P']   = vD*numParticles/V
    phiDD[chunk]['3COMP']['G']   = totVGrafts/V
    phiDD[chunk]['3COMP']['M']   = totVMat/V
    phiDD[chunk]['3COMP']['PGP'] = totVPGP/V
    for k,v in phiDD[chunk]['3COMP'].items():
      print '--> Type: ',k,'phi1:',v
    #-------------------------------------------------
    phiDD[chunk]['2COMP'] = {}
    phiDD[chunk]['2COMP']['G'] = totVGrafts/(totVGrafts+totVMat)
    phiDD[chunk]['2COMP']['M'] = totVMat/(totVGrafts+totVMat)
    for k,v in phiDD[chunk]['2COMP'].items():
      print '--> Type: ',k,'phi2:',v

  intraRDFDD      = {}
  interRDFDD      = {}
  fullRDFDD       = {}
  intraOmegaDD    = {}
  interOmegaDD    = {}
  fullOmegaDD     = {}
  frameStart      = fd['frames'][0,0]
  frameEnd        = fd['frames'][-1,1]
  for pair in pairs:
    if (numParticles==0) and ('P' in pair):
      print '--> Skipping pair',pair,'as numParticles is',numParticles
      continue

    if (numParticles==0) and ('G' in pair):
      print '--> Skipping pair',pair,'as numParticles is',numParticles
      continue

    if (numMatrix1==0) and ('M' in pair):
      print '--> Skipping pair',pair,'as numMatrix1 is',numMatrix1
      continue

    cmd_list=  ['mpirun correlate']
    cmd_list+= [' --output_file={}'.format(tempFile)]
    cmd_list+= [' --output_freq={}'.format(frames_per_chunk)]
    cmd_list+= [' --overwrite']
    cmd_list+= [' --input_path={}'.format(TP.make())]
    cmd_list+= [' --topo={}'.format(topoFile)]
    cmd_list+= [' --trj={}'.format(trjFile)]
    cmd_list+= [' --type1={}'.format(pairMap[pair[0]])]
    cmd_list+= [' --type2={}'.format(pairMap[pair[1]])]
    cmd_list+= [' --frame_start={}'.format(frameStart)]
    cmd_list+= [' --frame_end={}'.format(frameEnd)]
    cmd_list+= [' --dx={}'.format(dr)]
    cmd_list+= [' --xmax={}'.format(rmax)]
    cmd_list+= [' --kernel={}'.format('rdf')]
    cmd_list+= ['']

    cmd = ''
    print '.:: Setting up the following command to calculate the full rdf:'
    cmd_list[-4]=' --dx={}'.format(dr)
    cmd_list[-3]=' --xmax={}'.format(rmax)
    cmd_list[-2]=' --kernel={}'.format('rdf')
    cmd_list[-1]=''
    for cmd_chunk in cmd_list:
      cmd += cmd_chunk
      print '\t',cmd_chunk
    subprocess.check_call(shlex.split(cmd))
    corr_data = np.loadtxt(tempFile)
    fullRDFDD[pair] = {'x':corr_data[0],'y':corr_data[1:]}
    os.remove(tempFile)

    cmd = ''
    print '.:: Setting up the following command to calculate the inter-molecular rdf:'
    cmd_list[-4]=' --dx={}'.format(dr)
    cmd_list[-3]=' --xmax={}'.format(rmax)
    cmd_list[-2]=' --kernel={}'.format('rdf')
    cmd_list[-1]=' --inter'
    for cmd_chunk in cmd_list:
      cmd += cmd_chunk
      print '\t',cmd_chunk
    subprocess.check_call(shlex.split(cmd))
    corr_data = np.loadtxt(tempFile)
    interRDFDD[pair] = {'x':corr_data[0],'y':corr_data[1:]}
    os.remove(tempFile)


    cmd = ''
    print '.:: Setting up the following command to calculate the intra-molecular omega:'
    cmd_list[-4]=' --dx={}'.format(dk)
    cmd_list[-3]=' --xmax={}'.format(kmax)
    cmd_list[-2]=' --kernel={}'.format('omega')
    cmd_list[-1]=' --intra'
    for cmd_chunk in cmd_list:
      cmd += cmd_chunk
      print '\t',cmd_chunk
    subprocess.check_call(shlex.split(cmd))
    corr_data = np.loadtxt(tempFile)
    intraOmegaDD[pair] = {'x':corr_data[0],'y':corr_data[1:]}
    os.remove(tempFile)


    cmd = ''
    print '.:: Setting up the following command to calculate the full omega:'
    cmd_list[-4]=' --dx={}'.format(dk)
    cmd_list[-3]=' --xmax={}'.format(kmax)
    cmd_list[-2]=' --kernel={}'.format('omega')
    cmd_list[-1]=''
    for cmd_chunk in cmd_list:
      cmd += cmd_chunk
      print '\t',cmd_chunk
    subprocess.check_call(shlex.split(cmd))
    corr_data = np.loadtxt(tempFile)
    fullOmegaDD[pair] = {'x':corr_data[0],'y':corr_data[1:]}
    os.remove(tempFile)



  newdict={}
  newdict['rdf']    = {'full':fullRDFDD,'inter':interRDFDD,'intra':intraRDFDD}
  newdict['omega']  = {'full':fullOmegaDD,'inter':interOmegaDD,'intra':intraOmegaDD}
  newdict['box']    = boxDD
  newdict['rho']    = rhoDD
  newdict['phi']    = phiDD
  newdict['spec']   = trialSpec

  
  if not TP.trial in dump:
    dump[TP.trial] = {}

  dump[TP.trial][TP.rep]=newdict

  with open(dumpName,'wb') as f:
    cPickle.dump(dump,f,protocol=2)
    print '.:: Successfully wrote to',dumpName,'!'

  t2 = time.time()
  print t2-t1
  print "LOOP TIME:", (t2-t1)/60
