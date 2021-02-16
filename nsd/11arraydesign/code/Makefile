export PYTHONPATH=modmesh-master

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLINC ?= ${MKLROOT}/include
MKLLIB ?= ${MKLROOT}/lib
MKLEXT ?= a
else ifeq ($(UNAME_S),Linux)
MKLINC ?= /usr/include/mkl
MKLLIB ?= /usr/lib/x86_64-linux-gnu
MKLEXT ?= so
CXXFLAGS += -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLLIB}/libmkl_intel_lp64.${MKLEXT} \
	${MKLLIB}/libmkl_sequential.${MKLEXT} \
	${MKLLIB}/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

PYTHON ?= $(shell which python3)
PYTHON_VERSION ?= $(shell $(PYTHON) -c 'import sys; print("%d.%d" % (sys.version_info.major, sys.version_info.minor))')
PYTHON_INC := $(shell python3-config --prefix)/include/python$(PYTHON_VERSION)
NUMPY_INC := $(shell python3 -c 'import numpy ; print(numpy.get_include())')

PYTHON_LIB := $(shell python3-config --prefix)/lib

CPATH := $(PYTHON_INC):$(NUMPY_INC)

ifdef YHROOT
CPATH := $(CPATH):$(YHROOT)/usr/$(YHFLAVOR)/include
endif

export CPATH

BASES := 01_grid 01_solve_python_loop 01_solve_analytical
BASES += 02_solve_array
BASES += 03_solve_cpp
BASES += 04_fit_poly

SCRIPTS := $(patsubst %,%.py,$(BASES))
IMAGES := $(patsubst %,../image/%.png,$(BASES))

BINS := solve_cpp.so data_prep.so

.PHONY: default
default: $(IMAGES) $(BINS)

.PHONY: modmesh
modmesh:
	./build.sh modmesh

.PHONY: testimport
testimport: libst
	python3 -c 'import modmesh ; print(modmesh)'
	python3 -c 'import libst ; print(libst)'

#cpp -v /dev/null -o /dev/null

data_prep.so: data_prep.cpp Makefile
	g++ $< -o $@ -O3 -fPIC -shared -std=c++17 -lpython$(PYTHON_VERSION) -L$(PYTHON_LIB) -I${MKLINC} ${MKLLINKLINE}

%.so: %.cpp Makefile
	g++ $< -o $@ -O3 -fPIC -shared -std=c++17 -lpython$(PYTHON_VERSION) -L$(PYTHON_LIB)

../image/03_solve_cpp.png: 03_solve_cpp.py solve_cpp.so
	./$<

../image/04_fit_poly.png: 04_fit_poly.py data_prep.so
	./$<

../image/%.png: %.py
	./$<

.PHONY: clean
clean:
	rm -rf *.o *.so
