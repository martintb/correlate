import os
import re
import numpy as np
import ipdb; ist = ipdb.set_trace
import subprocess,shlex

def lammpsDataSkim(file,pattern='Atoms'):
  while True:
    line = file.readline()
    if pattern in line:
      break
    elif line is None:
      break
  file.readline() #blank line after data header

def readNLines(file,nlines):
  lines = []
  for i in range(nlines):
    lines.append(file.readline())
  return lines

def readData(dataPath,dataFile=''):
  ld={}
  with open(os.path.join(dataPath,dataFile),'r') as f:
    header = ' '.join([i.strip() for i in readNLines(f,15)])
    ld['natoms'] = int(re.search(r'([0-9]+)\s*atoms',header).groups()[0])
    ld['nbonds'] = int(re.search(r'([0-9]+)\s*bonds',header).groups()[0])
    ld['natomtypes'] = int(re.search(r'([0-9]+)\s*atom types',header).groups()[0])
    ld['nbondtypes'] = int(re.search(r'([0-9]+)\s*bond types',header).groups()[0])
    box = re.search(r'(?P<xlo>\-?[0-9]+\.[0-9]+)\s*(?P<xhi>\-?[0-9]+\.[0-9]+)\s*xlo xhi\s*' \
                    r'(?P<ylo>\-?[0-9]+\.[0-9]+)\s*(?P<yhi>\-?[0-9]+\.[0-9]+)\s*ylo yhi\s*' \
                    r'(?P<zlo>\-?[0-9]+\.[0-9]+)\s*(?P<zhi>\-?[0-9]+\.[0-9]+)\s*zlo zhi\s*' \
                    ,header).groupdict()
    ld.update({i:float(j) for i,j in box.items()})

    f.seek(0)
    ld['mass_map'] = {}
    lammpsDataSkim(f,'Masses')
    for i in range(ld['natomtypes']):
      type,mass = f.readline().split()
      ld['mass_map'][type] = float(mass)

    f.seek(0)
    lammpsDataSkim(f,'Atoms')
    line = f.readline()
    ncol = len(line.split())
    f.seek(0)
    lammpsDataSkim(f,'Atoms')
    atomLines = np.fromfile(f,dtype=float,count=ld['natoms']*ncol,sep=' ').reshape(-1,ncol)
    if ncol==9:
      ld['indices'] = atomLines[:,0].astype(int) - 1
      ld['molecules'] = atomLines[:,1].astype(int) - 0
      ld['types'] = atomLines[:,2].astype(int).astype(str)
      ld['positions'] = atomLines[:,3:6].astype(float)
      ld['images'] = atomLines[:,6:].astype(int)
      ld['masses'] = np.array([ld['mass_map'][i] for i in ld['types']])
    elif ncol==10:
      ld['indices'] = atomLines[:,0].astype(int) - 1
      ld['molecules'] = atomLines[:,1].astype(int) - 0
      ld['types'] = atomLines[:,2].astype(int).astype(str)
      ld['charges'] = atomLines[:,3].astype(float)
      ld['positions'] = atomLines[:,4:7].astype(float)
      ld['images'] = atomLines[:,7:].astype(int)
      ld['masses'] = np.array([ld['mass_map'][i] for i in ld['types']])
    else:
      raise ValueError('AtomGroup not set up to handle datafile of this type!')

    f.close()
  return ld

dataFileIn = 'data.lammps'
xmlFileIn = 'data.NoMol.xml'
xmlFileOut = 'data.xml'
data = readData('./','data.lammps')

print '.:: Creating tcl script for VMD...'
with open('gen_xml.tcl','w') as f:
  f.write('package require topotools\n')
  f.write('topo readlammpsdata data.lammps bond\n')
  f.write('set sel [atomselect top all]\n')
  f.write('animate write hoomd {} sel $sel top\n'.format(xmlFileIn))
  f.write('exit\n')

print '.:: Calling VMD to create base xml: {}'.format(xmlFileIn)
cmd = 'vmd -dispdev none -e gen_xml.tcl'
stdout = subprocess.check_output(cmd,stderr=subprocess.STDOUT,shell=True)

print '.:: Adding molecule node to vmd-generated xml...'
with open(xmlFileIn,'r') as fin,open(xmlFileOut,'w') as fout:
  for line in fin:
    if '</configuration>' in line:
      line = ''
      line += '<molecule>\n'
      line += '\n'.join(data['molecules'].astype(str))
      line += '\n</molecule>\n'
      line += '</configuration>\n'
    fout.write(line)

print '.:: Done!'


