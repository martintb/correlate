# CORRELATE
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
4. **msid**
    * mean-square internal distances 
5. **procData**
    * print the xyz coordinates of each mpi process (debugging tool)

### WARNING 1 (FOR MSID ONLY)
All of the molecules must have be in "unwrapped" coordinates. The minimum image convention will not be applied during the calculation. The unwrapped coordinates can be specified in LAMMPS or created via post-processing in VMD or a custom script.

Furthermore, only linear chain molecules will work for this calculator. The calculator assumes that if index 7 and index 12 are in the same molecule, they are separated by 5 bonds.  

## Input Variables:
### via command line:
note the double (--) vs single (-) dashes
```
Base Options:
  -h [ --help ]                  produce help message
  -f [ --config_file ] arg       optional configuration file
  --kernel arg                   calculation kernel
  --intra                        restrict calculation to intra-molecular pairs
  --inter                        restrict calculation to inter-molecular pairs
  --type1 arg                    bead type 1 (e.g A or A,B)
  --type2 arg                    bead type 2

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
.topo           Two-Column (Type,Molecule) topology file
.lmpdump        LAMMPS dump trajectory file (sorted or unsorted)
.lmpbond        LAMMPS topology file w/ AtomStyle Bond
.lmpfull        LAMMPS topology file w/ AtomStyle Full
.lmpmolecular   LAMMPS topology file w/ AtomStyle Molecular
.xml            HOOMD-Blue topology file (optional molecule section is supported)

The following calculation kernels are available:
histogram
rdf
omega
msid
procData
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
kernel        = omega
intra         = True
overwrite     = True
dx            = 0.1
xmax          = 6.2
output_file   = calc.dat
output_freq   = 1
```
###Input Variables Notes:
* Values in parenthesis represent the default used if unspecified
* Type groups can be specified by using commas
    * e.g. --type1=A,B or --type2=3,4,5
* Use of --inter and --intra require that *sane* molecule numbers be passed in the topology file
    * These flags are meaningless for the msid kernel, and therefore are ignored in this case. 
* frame_start supports negative indexing from the last frame in the trajectory
    * e.g. -10 represents 10 frames from the end of the trajectory
* frame_end takes a -1 argument to mean the last frame in the trajectory
* all variables can be specified either as command line arguments or in config file or both
    * command line specification super-cedes the input file

## Supported Topology and Trajectory Files:
* Two-Column (Type,Molecule) topology File (.topo)
* LAMMPS dump trajectory file (.lmpdump)
* LAMMPS data file w/ AtomStyle bond (.lmpbond)
* LAMMPS data file w/ AtomStyle Full (.lmpfull)
* LAMMPS data file w/ AtomStyle Molecular (.lmpmolecular)
* CHARMM/LAMMPS/HOOMD style DCD trajectory file (.dcd)
* HOOMD-Blue xml files (with optional molecule section supported) (.xml)

Currently, correlate recognizes the filetype purely through the extension of the file. 
In order to differentiate the various types of LAMMPS files (and others), the user will likely need to rename their files. An alternative is to create symbolic links to the original files, with the corrected extension: e.g. for a lammps data file named *data.lammps* with AtomStyle Full:
```
ln -s data.lammps data.lmpfull
```
In this way you preserve the original file with the original name name, while allowing correlate to easily recognize the file types. 

### WARNING 1
All filedata must be sorted by atom index! This includes all types and all positions at all frames. LAMMPS, in particular, does not sort its data or dump files by default. *Correlate* code assumes that¸ in the array of types and positions, type[10] and pos[10] both correspond to the same atom. 

### WARNING 2
The LAMMPS dump reader currently only works with **unscaled** coordinates. You must have x, y, and z columns (not xs,ys,zs) in your dump file trajectory. Note that this is **not** the default dump format for LAMMPS, and you will have to define a dump custom to get these columns.

## Dependencies:
### Required:
* cpp compiler with c++11 and std::regex support
    * developed with gcc 4.9.3
* Boost C++ Libraries
    * developed with Boost 1.60
    * required compiled boost sub-libraries:
        * boost_filesystem
        * boost_system
        * boost_program_options
* an implementation of MPI
    * tested with OpenMPI 1.8.2 and 1.10.2


## Build Instructions:
Just make sure that the above dependencies are fulfilled and that CFLAGS,LDLIBS, and LDFLAGS are properly set (in environment or in the Makefile) and then run make with the appropriate parallelization level:
```
make -j6
```
Correlate doesn't currently have an install target set up. Just copy or symlink the correlate executable to directory you need it in or add the source directory to your PATH.
