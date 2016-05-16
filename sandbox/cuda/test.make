.PHONY: clean gdb

BASEPATH=../../src

MAINOBJ:=main.o
CUDASRC:=cuda.cpp
CUDAOBJ:=cuda.o
EXEC=exec
CC=mpicc
CXX=mpicxx
LD=mpicxx
LDFLAGS+= -L$(CUDA_LIB_HOME)
LDLIBS+= -lcuda -lcudart
CPPFLAGS+=  -std=c++11 -g -Wall
CUDAFLAGS= -arch=sm_35

$(EXEC): $(MAINOBJ)  $(CUDAOBJ) 
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(CUDAOBJ): $(CUDASRC)
	nvcc $(CUDAFLAGS) -c -o $@ $<

clean:
	rm -f $(EXEC) $(MAINOBJ) $(CUDAOBJ)
