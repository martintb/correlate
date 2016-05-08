.PHONY: clean gdb

EXEC=correlate
CC=mpicc
CXX=mpicxx
LD=mpicxx
LDLIBS+= -lboost_filesystem -lboost_system -lboost_program_options
CPPFLAGS+= -Isrc/kernel -Isrc/util -Isrc/atomgroup -Isrc/core -Isrc/reader
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
MAINOBJ:=main.o
CORESRC:=$(wildcard src/core/*.cpp)
COREOBJ:=$(filter-out src/core/$(MAINOBJ),$(subst .cpp,.o, $(CORESRC)))

AGSRC:=$(wildcard src/atomgroup/*.cpp)
AGOBJ:=$(filter-out src/atomgroup/$(MAINOBJ),$(subst .cpp,.o, $(AGSRC)))

READERSRC:=$(wildcard src/reader/*.cpp)
READEROBJ:=$(filter-out src/reader/$(MAINOBJ),$(subst .cpp,.o, $(READERSRC)))
ifeq ($(MAKE_XML),false)
	READEROBJ:=$(filter-out src/reader/xmlReader.o,$(READEROBJ))
endif

UTILSRC:=$(wildcard src/util/*.cpp)
UTILOBJ:=$(filter-out src/util/$(MAINOBJ),$(subst .cpp,.o, $(UTILSRC)))

KERNSRC:=$(wildcard src/kernel/*.cpp)
KERNOBJ:=$(filter-out src/kernel/$(MAINOBJ),$(subst .cpp,.o, $(KERNSRC)))

CUDASRC:=$(wildcard src/cu/*.cu)
CUDAOBJ:=$(filter-out src/cu/$(MAINOBJ),$(subst .cpp,.o, $(CUDASRC)))


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
