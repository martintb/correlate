.PHONY: clean gdb

BASEPATH=../../src

MAINOBJ:=test.o
EXEC=exec
CC=mpicc
CXX=mpicxx
LD=mpicxx
LDLIBS+= -lboost_filesystem -lboost_system -lboost_program_options
CPPFLAGS+= -I$(BASEPATH)/kernel -I$(BASEPATH)/util -I$(BASEPATH)/atomgroup -I$(BASEPATH)/core -I$(BASEPATH)/reader
CPPFLAGS+= -fdiagnostics-color=always -std=c++11 -g -Wall
CPPFLAGS+= -O3 

# CUDA things
# CPPFLAGS+= -Icu
# LDLIBS+= -lcuda -lcudart
# LDFLAGS+= -L$(CUDA_LIB_HOME)
# CUDAFLAGS=-std=c++11 -arch=sm_35 -O3


#This doesn't actually work yet
MAKE_XML=false
ifeq ($(MAKE_XML),true)
	CPPFLAGS+= -DXML
endif


# Gather Objects for Different Cases
CORESRC:=$(wildcard $(BASEPATH)/core/*.cpp)
COREOBJ:=$(filter-out $(BASEPATH)/core/main.o,$(subst .cpp,.o, $(CORESRC)))

AGSRC:=$(wildcard $(BASEPATH)/atomgroup/*.cpp)
AGOBJ:=$(filter-out $(BASEPATH)/atomgroup/main.o,$(subst .cpp,.o, $(AGSRC)))

READERSRC:=$(wildcard $(BASEPATH)/reader/*.cpp)
READEROBJ:=$(filter-out $(BASEPATH)/reader/main.o,$(subst .cpp,.o, $(READERSRC)))
ifeq ($(MAKE_XML),false)
	READEROBJ:=$(filter-out $(BASEPATH)/reader/xmlReader.o,$(READEROBJ))
endif

UTILSRC:=$(wildcard $(BASEPATH)/util/*.cpp)
UTILOBJ:=$(filter-out $(BASEPATH)/util/main.o,$(subst .cpp,.o, $(UTILSRC)))

KERNSRC:=$(wildcard $(BASEPATH)/kernel/*.cpp)
KERNOBJ:=$(filter-out $(BASEPATH)/kernel/main.o,$(subst .cpp,.o, $(KERNSRC)))

CUDASRC:=$(wildcard $(BASEPATH)/cu/*.cu)
CUDAOBJ:=$(filter-out $(BASEPATH)/cu/main.o,$(subst .cpp,.o, $(CUDASRC)))


$(EXEC): $(MAINOBJ) $(COREOBJ) $(AGOBJ) $(READEROBJ) $(UTILOBJ) $(KERNOBJ)
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
	rm -f $(EXEC) $(AGOBJ) $(READEROBJ) $(MAINOBJ) $(UTILOBJ) $(CUDAOBJ) $(KERNOBJ) $(COREOBJ)
