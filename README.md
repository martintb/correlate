CORRELATE
# Supported Calculation Kernels:
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

# Input Variables:
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
###Input Variables Notes:
* the frame_start argument supports negative indexing from the last frame in the trajectory
* the frame_end takes a -1 argument to mean the last frame int he trajectory
* all variables can be specified either as command line arguments or in config file.
    * Important! Note the lack of quotation marks.
```
path          = ./data
topo          = data.lj.lmpbond
trj           = trajectory.lj.dcd
type1         = 3
type2         = 3
frame_start   =-25
frame_end     =-1
kernel        =rdf
dx            =0.1
xmax          =50
outfile       =calc.dat
```
# Supported Topology and Trajectory Files:
* LAMMPS Data File
    * AtomStyle Bond
    * Other AtomStyles are trivial (yet not currently) to implemented
* CHARMM/LAMMPS/HOOMD style dcd file

# Dependencies:
## Required:
* gcc 4.9 or greater
    * need c++11 and std::regex support
    * untested with other compilers, but others should work in theory
* boost
    * boost_filesystem
    * boost_system
    * boost_program_options
* some mpi implementation
    * tested with OpenMPI 1.8.2 and 1.10.2
    * other implementations of mpi may work, but are currently untested

## Optional:
1. rapidxml
    * for reading HOOMD-blue xml files (not implemented)
2. CUDA
    * for gpu accelerated kernels