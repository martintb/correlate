.PHONY: doc data clean test

# Gather Objects for Different Cases
ALLSRC:=$(wildcard *.cpp)
MAINOBJ:=$(subst .cpp,.o, $(ALLSRC))
AGSRC:=$(wildcard atomgroup/*.cpp)
AGOBJ:=$(subst .cpp,.o, $(AGSRC))
DEBUGSRC:=$(wildcard debug/*.cpp)
DEBUGOBJ:=$(subst .cpp,.o, $(DEBUGSRC))
CUDASRC:=$(wildcard cu/*.cu)
CUDAOBJ:=$(subst .cu,.o, $(CUDASRC))

EXEC=corr
CC=mpicc
CXX=mpicxx
LD=mpicxx
LDLIBS+= -lboost_filesystem -lboost_system -lboost_regex  -lboost_program_options
CPPFLAGS+= -Idebug -Iatomgroup
CPPFLAGS+= -std=c++11 -g
CPPFLAGS+= -O3 
# CPPFLAGS+= -DARMA_NO_DEBUG
# CPPFLAGS+= -fopenmp

# CUDA things
# CPPFLAGS+= -Icu
# LDLIBS+= -lcuda -lcudart
# LDFLAGS+= -L$(CUDA_LIB_HOME)
# CUDAFLAGS=-std=c++11 -arch=sm_35 -O3


$(EXEC): $(MAINOBJ) $(AGOBJ) $(DEBUGOBJ)
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
	rm -f $(EXEC) $(AGOBJ) $(MAINOBJ) $(DEBUGOBJ) $(CUDAOBJ)