.PHONY: clean gdb

EXEC=correlate
MAINOBJ:=src/core/main.o
CC=mpicc
CXX=mpicxx
LD=mpicxx
LDLIBS+= -lboost_filesystem -lboost_system -lboost_program_options
CPPFLAGS+= -Isrc/core -Isrc/atomgroup -Isrc/reader -Isrc/writer -Isrc/file -Isrc/kernel -Isrc/util -Isrc/exception
CPPFLAGS+= -fdiagnostics-color=always -std=c++11 -g -Wall
CPPFLAGS+= -O3 

include gather.make
include cuda.make


DEPS=src/util/version.o
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

src/util/version.cpp: .git/HEAD .git/index
	    echo "const char *version = \"$(shell git describe --abbrev=4 --dirty --always --tags)\";" > $@
	    echo "const char *build_date = \"$(shell date)\";" >> $@

$(CUDAOBJ): $(CUDASRC)
	nvcc $(CUDAFLAGS) -c -o $@ $<

gdb: $(EXEC)
	gdb -x gdbinit $(EXEC)

clean:
	rm -f $(EXEC) $(DEPS)
