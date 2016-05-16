# Gather Objects for Different Cases
CORE../../src:=$(wildcard ../../src/core/*.cpp)
COREOBJ:=$(filter-out ../../src/core/main.o,$(subst .cpp,.o, $(CORE../../src)))

AG../../src:=$(wildcard ../../src/atomgroup/*.cpp)
AGOBJ:=$(filter-out ../../src/atomgroup/main.o,$(subst .cpp,.o, $(AG../../src)))

READER../../src:=$(wildcard ../../src/reader/*.cpp)
READEROBJ:=$(filter-out ../../src/reader/main.o,$(subst .cpp,.o, $(READER../../src)))
ifeq ($(MAKE_XML),false)
	READEROBJ:=$(filter-out ../../src/reader/xmlReader.o,$(READEROBJ))
endif

WRITER../../src:=$(wildcard ../../src/writer/*.cpp)
WRITEROBJ:=$(filter-out ../../src/writer/main.o,$(subst .cpp,.o, $(WRITER../../src)))

FILE../../src:=$(wildcard ../../src/file/*.cpp)
FILEOBJ:=$(filter-out ../../src/file/main.o,$(subst .cpp,.o, $(FILE../../src)))

UTIL../../src:=$(wildcard ../../src/util/*.cpp)
UTILOBJ:=$(filter-out ../../src/util/main.o,$(subst .cpp,.o, $(UTIL../../src)))

KERN../../src:=$(wildcard ../../src/kernel/*.cpp)
KERNOBJ:=$(filter-out ../../src/kernel/main.o,$(subst .cpp,.o, $(KERN../../src)))

CUDA../../src:=$(wildcard ../../src/cu/*.cu)
CUDAOBJ:=$(filter-out ../../src/cu/main.o,$(subst .cpp,.o, $(CUDA../../src)))
