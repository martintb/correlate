.PHONY: clean gdb

BASEPATH=../../src

MAINOBJ:=test.o
EXEC=exec
CC=mpicc
CXX=mpicxx
LD=mpicxx
LDLIBS+= -lboost_filesystem -lboost_system -lboost_program_options
CPPFLAGS+= -I$(BASEPATH)/kernel -I$(BASEPATH)/util -I$(BASEPATH)/atomgroup -I$(BASEPATH)/core -I$(BASEPATH)/reader -I$(BASEPATH)/writer -I$(BASEPATH)/file -I$(BASEPATH)/exception
CPPFLAGS+= -fdiagnostics-color=always -std=c++11 -g -Wall
CPPFLAGS+= -O3 

include gather.make

DEPS=
DEPS+= $(MAINOBJ)
DEPS+= $(COREOBJ)
DEPS+= $(AGOBJ)
DEPS+= $(READEROBJ)
DEPS+= $(WRITEROBJ)
DEPS+= $(FILEOBJ)
DEPS+= $(UTILOBJ)
DEPS+= $(KERNOBJ)

$(EXEC): $(DEPS)
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)


clean:
	rm -f $(EXEC)

allclean:
	rm -f $(EXEC) $(AGOBJ) $(READEROBJ) $(MAINOBJ) $(FILEOBJ) $(UTILOBJ) $(CUDAOBJ) $(KERNOBJ) $(COREOBJ)
