#!/bin/bash -l

vpkg_require python gcc/4.8.3
vpkg_require openmpi/1.8.2-gcc-4.8.3
vpkg_devrequire jlab-rapidxml jlab-boost jlab-arma
# vpkg_require python-matplotlib python-numpy jlab-python-ipdb
