.PHONY: clean gdb

BASEPATH=../../src

MAINOBJ:=test.o
EXEC=exec
CC=mpicc
CXX=mpicxx
LD=mpicxx
LDLIBS+= -lboost_filesystem -lboost_system -lboost_program_options
CPPFLAGS+= -I$(BASEPATH)/file -I$(BASEPATH)/util
CPPFLAGS+= -fdiagnostics-color=always -std=c++11 -g -Wall
CPPFLAGS+= -O3 

# Gather Objects for Different Cases
FILESRC:=$(wildcard $(BASEPATH)/file/*.cpp)
FILEOBJ:=$(filter-out $(BASEPATH)/file/main.o,$(subst .cpp,.o, $(FILESRC)))

$(EXEC): $(MAINOBJ) $(FILEOBJ)
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)

gdb: $(EXEC)
	gdb -x gdbinit $(EXEC)

clean:
	rm -f $(EXEC) $(FILEOBJ) $(MAINOBJ)
