#!/bin/bash -l

vpkg_require python gcc/4.8.3
vpkg_require openmpi/1.8.2-gcc-4.8.3
vpkg_devrequire jlab-rapidxml jlab-boost 
vpkg_devrequire lapack/3.5.0
vpkg_devrequire atlas/3.10.2
vpkg_devrequire jlab-arma
vpkg_require python-matplotlib typy/3
