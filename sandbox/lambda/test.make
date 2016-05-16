.PHONY: clean gdb

BASEPATH=../../src

MAINOBJ:=main.o
EXEC=exec
CC=mpicc
CXX=mpicxx
LD=mpicxx
LDLIBS+= 
CPPFLAGS+= 
CPPFLAGS+= -fdiagnostics-color=always -std=c++11 -g -Wall
# CPPFLAGS+= -O3 

$(EXEC): $(MAINOBJ) 
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)

gdb: $(EXEC)
	gdb -x gdbinit $(EXEC)

clean:
	rm -f $(EXEC) $(FILEOBJ) $(MAINOBJ)
