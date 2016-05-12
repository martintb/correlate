#**CORRELATE**
Calculates various spatial correlation functions in real and reciprocal space. Code is MPI aware and can be used with any number of MPI processes up to the number of atoms in your calculation.

Example invocation:
```
mpirun -np 10 ./correlate --type1=A,C --type2=B --kernel=rdf --config_file=inputs.cfg
```
This will split up a partial radial-distribution function calculation amongst 10 MPI processes and write the output to the default corr.out file. The rdf calculation will be between all atoms of type A and C with all atoms of type B. 

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
6. **intra_mol_omega**
    * intra molecular structure factor calculator
7. **printProcXYZ**
    * Print the xyz coordinates of each mpi process (debugging tool)

## Input Variables:
### via command line:
note the double (--) vs single (-) dashes
```
Base Options:
  -h [ --help ]                  produce help message
  -f [ --config_file ] arg       optional configuration file
  --kernel arg                   calculation kernel
  --type1 arg                    bead type 1 (e.g A or A,B)
  --type2 arg                    bead type 2
  --nthreads arg (=1)            placeholder pending future threading support

File Reading:
  --input_path arg (=./)         path to input data files
  --topo arg                     topology file name
  --trj arg                      trajectory file name

File Writing:
  --output_path arg (=./)        path to ouput files
  --output_file arg (=calc.dat)  file to write results to
  --output_freq arg (=-1)        frequency of writing data to file
  --overwrite                    overwrite existing output files

Calculation:
  --frame_start arg (=0)         first frame to use in calculation
  --frame_end arg (=-1)          last frame to use in calculation
  --frame_step arg (=1)          interval between used frames
  --dx arg (=0.100000001)        grid size for output (real or kspace)
  --xmax arg (=100)              largest value in output grid (real or kspace)


The following readers are available:
.dcd            DCD trajectory file
.lmpbond        LAMMPS structure file w/ AtomStyle Bond
.lmpfull        LAMMPS structure file w/ AtomStyle Full
.lmpmolecular   LAMMPS structure file w/ AtomStyle Molecular

The following calculation kernels are available:
printProcXYZ
histogram
rdf
inter_mol_rdf
omega
inter_mol_omega
intra_mol_omega
```
### via configiration file:
note the lack of quotation marks for strings
```
input_path    = ./data/
topo          = data.lj.lmpbond
trj           = trajectory.lj.dcd
type1         = 3
type2         = 3
frame_start   = -10
frame_end     = -1
kernel        = rdf
dx            = 0.1
xmax          = 100
output_file   = calc.dat
output_freq   = 1
```
###Input Variables Notes:
* Values in parenthesis represent the default used if unspecified
* Type groups can be specified by using commas
    * e.g. --type1=A,B or --type2=3,4,5
* frame_start supports negative indexing from the last frame in the trajectory
* frame_end takes a -1 argument to mean the last frame in the trajectory
* all variables can be specified either as command line arguments or in config file or both
    * command line specification super-cedes the input file

## Supported Topology and Trajectory Files:
* LAMMPS data file w/ AtomStyle Bond (.lmpbond)
* LAMMPS data file w/ AtomStyle Full (.lmpfull)
* LAMMPS data file w/ AtomStyle Molecular (.lmpmolecular)
* CHARMM/LAMMPS/HOOMD style DCD trajectory file (.dcd)

## General Notes
* Currently, the output file has several columns that correspond to different normalization of the output histogram. Only the column labeled according to you specified calculation may have any meaning i.e. use the rdf column for rdf calculations and the omega column for omega calculations. This is on the list of things to improve in future updates. 

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


### Future Optional Dependencies:
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
