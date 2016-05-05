CORRELATE
# Supported Calculation Kernels:
1. histogram
    * radial distance histogram of atom positions
2. rdf
    * radial distribution function
3. omega
    * structure factor calculation via sin(kr)/kr summation
4. inter_mol_rdf
    * inter molecular radial distribution function
5. inter_mol_omega
    * inter molecular structure factor calculator
6. printProcXYZ
    * Print the xyz coordinates of each mpi process (debugging tool)

# Dependencies:
## Required:
* gcc 4.9 or greater
* boost
    * boost_filesystem
    * boost_system
    * boost_program_options

## Optional:
1. rapidxml
    * for reading HOOMD-blue xml files (not implemented)
2. CUDA
    * for gpu accelerated kernels

