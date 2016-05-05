.PHONY: doc data clean test

# Gather Objects for Different Cases
MAINSRC:=$(wildcard main.cpp)
MAINOBJ:=$(subst .cpp,.o, $(MAINSRC))
CORESRC:=$(wildcard *.cpp)
COREOBJ:=$(subst .cpp,.o, $(CORESRC))

EXEC=exec
CC=cc
CXX=g++
LD=gcc
LDFLAGS += -lboost_filesystem
CPPFLAGS+=
CPPFLAGS+= -std=c++11 -g -Wall


# CPPFLAGS+= -O3 
# CPPFLAGS+= -DARMA_NO_DEBUG
# CPPFLAGS+= -fopenmp

# CUDA things
# CPPFLAGS+= -Icu
# LDLIBS+= -lcuda -lcudart
# LDFLAGS+= -L$(CUDA_LIB_HOME)
# CUDAFLAGS=-std=c++11 -arch=sm_35 -O3


$(EXEC): $(MAINOBJ) $(COREOBJ) 
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)

run: $(EXEC)
	./$(EXEC)

$(CUDAOBJ): $(CUDASRC)
	nvcc $(CUDAFLAGS) -c -o $@ $<

gdb: $(EXEC)
	gdb -x gdbinit $(EXEC)

clean:
	rm -f $(MAINOBJ) $(COREOBJ) $(EXEC)

allclean:
	rm -f $(EXEC) $(COREOBJ)
