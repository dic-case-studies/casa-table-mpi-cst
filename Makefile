CXXFLAGS=-std=c++14 -Wall -Wextra -pedantic

CFITSIO=`pkg-config  --cflags --libs cfitsio`

# ADIOS=`adios2-config --cxx-flags --cxx-libs`

CASA=-lcasa_casa -lcasa_lattices -lcasa_images -lcasa_tables -lcasa_scimath -lcasa_coordinates -lcasa_fits

# OPENMPI=`pkg-config --cflags --libs ompi`

# OPTIMISATION=-fsanitize=address -g
OPTIMISATION=-O3

CXX=mpicxx

SRC = src/casacore_table

all: dir $(SRC)

src/%: src/%.cc dir
	$(CXX) -o build/$@ $(CXXFLAGS) $(CFITSIO) $(CASA) $(OPT) $<

clean:
	rm -rf build/*

dir:
	mkdir -p build/src

.PHONY: all clean dir

