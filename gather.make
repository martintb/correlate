# Gather Objects for Different Cases
CORESRC:=$(wildcard src/core/*.cpp)
COREOBJ:=$(filter-out src/core/main.o,$(subst .cpp,.o, $(CORESRC)))

AGSRC:=$(wildcard src/atomgroup/*.cpp)
AGOBJ:=$(filter-out src/atomgroup/main.o,$(subst .cpp,.o, $(AGSRC)))

READERSRC:=$(wildcard src/reader/*.cpp)
READEROBJ:=$(filter-out src/reader/main.o,$(subst .cpp,.o, $(READERSRC)))
ifeq ($(MAKE_XML),false)
	READEROBJ:=$(filter-out src/reader/xmlReader.o,$(READEROBJ))
endif

UTILSRC:=$(wildcard src/util/*.cpp)
UTILOBJ:=$(filter-out src/util/main.o,$(subst .cpp,.o, $(UTILSRC)))

KERNSRC:=$(wildcard src/kernel/*.cpp)
KERNOBJ:=$(filter-out src/kernel/main.o,$(subst .cpp,.o, $(KERNSRC)))

CUDASRC:=$(wildcard src/cu/*.cu)
CUDAOBJ:=$(filter-out src/cu/main.o,$(subst .cpp,.o, $(CUDASRC)))
