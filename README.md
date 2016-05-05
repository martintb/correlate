CORRELATE

# Dependencies:
## Required:
* gcc 4.9 or greater
* boost
  * boost_filesystem
  * boost_system
  * boost_program_options

## Optional:
* rapidxml
  * for reading HOOMD-blue xml files (not implemented)
* CUDA
  * for gpu accelerated kernels


# Supported Kernels:
* histogram: radial distance histogram of atom positions
* rdf: radial distribution function
* omega: structure factor calculation via sin(kr)/kr summation
* inter_mol_rdf: inter molecular radial distribution function
* inter_mol_omega: inter molecular structure factor calculator
* printProcXYZ: Print the xyz coordinates of each mpi process (debugging tool)
