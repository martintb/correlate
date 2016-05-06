#**CORRELATE**
Calculates various spatial correlation functions in real and reciprocal space. Code is MPI aware and can be used with any number of MPI processes up to the number of atoms in your calculation.

Example invocation:
```
mpirun -np 10 ./correlate --type1=A --type2=B --kernel=rdf --config_file=inputs.cfg
```

## Supported Calculation Kernels:
1. **histogram**
    * radial distance histogram of atom positions
2. **rdf**
    * radial distribution function
3. **omega**
    * structure factor calculation via sin(kr)/kr summation
4. **inter_mol_rdf**
    * inter molecular radial distribution function
5. **inter_mol_omega**
    * inter molecular structure factor calculator
6. **printProcXYZ**
    * Print the xyz coordinates of each mpi process (debugging tool)

## Input Variables:
### via command line:
note the double (--) vs single (-) dashes
```
  -h [ --help ]             produce help message
  -f [ --config_file ] arg  optional configuration file
  --path arg (=./)          path to data files
  --topo arg                topology file name
  --trj arg                 trajectory file name
  --type1 arg               bead type 1 for calculating correlations
  --type2 arg               bead type 2 for calculating correlations
  --frame_start arg (=0)    first frame to use in calculation
  --frame_end arg (=-1)     last frame to use in calculation
  --frame_step arg (=1)     interval between used frames
  --nthreads arg (=1)       number of threads per mpi process
  --dx arg (=0.100000001)   grid size for output
  --xmax arg (=100)         largest value in output grid
  --kernel arg              kernel used for calculation
  --outfile arg (=corr.out) name of file to write output to
```
### via configiration file:
note the lack of quotation marks for strings
```
path          = ./data
topo          = data.lj.lmpbond
trj           = trajectory.lj.dcd
type1         = 3
type2         = 3
frame_start   = -25
frame_end     = -1
kernel        = rdf
dx            = 0.1
xmax          = 50
outfile       = calc.dat
```
###Input Variables Notes:
* Values in parenthesis represent the default used if unspecified
* frame_start supports negative indexing from the last frame in the trajectory
* frame_end takes a -1 argument to mean the last frame in the trajectory
* all variables can be specified either as command line arguments or in config file or both
    * command line specification super-cedes the input file

## Supported Topology and Trajectory Files:
* LAMMPS data file
    * AtomStyle bond (.lmpbond)
    * Other AtomStyles are trivial, but not currently implemented
* CHARMM/LAMMPS/HOOMD style DCD trajectory file (.dcd)

## Dependencies:
### Required:
* cpp compiler with c++11 and std::regex support
    * tested with gcc 4.9.3
* boost
    * boost_filesystem
    * boost_system
    * boost_program_options
* an implementation of MPI
    * tested with OpenMPI 1.8.2 and 1.10.2


### Optional:
1. rapidxml
    * for reading HOOMD-blue .xml files (not implemented)
2. CUDA
    * for GPU accelerated kernels (not implemented)

## Build Instructions:
```
make -j6
```
Correlate doesn't currently have an install routine set up. Just copy or symlink the correlate executable to directory you need it in. 

## Future Features
* Support for GPU based kernels via CUDA
* Support for full CPU + GPU simultaneous operation
    * User should be able to select fraction of system to be calculated on GPU
* Cleaner output handling
    * frame-by-frame output
    * remove extraneous output from file (e.g. no omega column for rdf output)