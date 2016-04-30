.PHONY: doc data clean test

# Gather Objects for Different Cases
MAINOBJ:=main.o
CORESRC:=$(wildcard src/core/*.cpp)
COREOBJ:=$(subst .cpp,.o, $(CORESRC))
AGSRC:=$(wildcard src/atomgroup/*.cpp)
AGOBJ:=$(subst .cpp,.o, $(AGSRC))
UTILSRC:=$(wildcard src/util/*.cpp)
UTILOBJ:=$(subst .cpp,.o, $(UTILSRC))
KERNSRC:=$(wildcard src/kernel/*.cpp)
KERNOBJ:=$(subst .cpp,.o, $(KERNSRC))
CUDASRC:=$(wildcard src/cu/*.cu)
CUDAOBJ:=$(subst .cu,.o, $(CUDASRC))

EXEC=corr
CC=mpicc
CXX=mpicxx
LD=mpicxx
LDLIBS+= -lboost_filesystem -lboost_system -lboost_regex  -lboost_program_options
# LDLIBS+= -lsatlas -larmadillo 
CPPFLAGS+= -Isrc/kernel -Isrc/util -Isrc/atomgroup -Isrc/core
CPPFLAGS+= -std=c++11 -g -Wall
# CPPFLAGS+= -O3 
# CPPFLAGS+= -DARMA_NO_DEBUG
# CPPFLAGS+= -fopenmp

# CUDA things
# CPPFLAGS+= -Icu
# LDLIBS+= -lcuda -lcudart
# LDFLAGS+= -L$(CUDA_LIB_HOME)
# CUDAFLAGS=-std=c++11 -arch=sm_35 -O3


$(EXEC): $(MAINOBJ) $(COREOBJ) $(AGOBJ) $(UTILOBJ) $(KERNOBJ)
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)

run: $(EXEC)
	./$(EXEC)

$(CUDAOBJ): $(CUDASRC)
	nvcc $(CUDAFLAGS) -c -o $@ $<

gdb: $(EXEC)
	gdb -x gdbinit $(EXEC)

clean:
	rm -f $(MAINOBJ) $(CUDAOBJ) $(EXEC)

allclean:
	rm -f $(EXEC) $(AGOBJ) $(MAINOBJ) $(UTILOBJ) $(CUDAOBJ) $(KERNOBJ) $(COREOBJ)
