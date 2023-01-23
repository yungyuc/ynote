export PYTHONPATH=modmesh-master

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
# Do not define "HASMKL" macro.
INC += -I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers
LINKFLAGS := -framework Accelerate
else ifeq ($(UNAME_S),Linux)
CXXFLAGS += -DHASMKL -Wl,--no-as-needed
ifeq (${MKLROOT},)
INC += -I/usr/include/mkl
MKLLIB ?= /usr/lib/x86_64-linux-gnu
else
INC += -I${MKLROOT}/include
MKLLIB ?= ${MKLROOT}/lib/intel64
endif
MKLEXT ?= so

LINKFLAGS := \
	${MKLLIB}/libmkl_intel_lp64.${MKLEXT} \
	${MKLLIB}/libmkl_sequential.${MKLEXT} \
	${MKLLIB}/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl
endif

PYTHON ?= $(shell which python3)
PYTHON_VERSION ?= $(shell $(PYTHON) -c 'import sys; print("%d.%d" % (sys.version_info.major, sys.version_info.minor))')
PYTHON_INC := $(shell python3-config --prefix)/include/python$(PYTHON_VERSION)
NUMPY_INC := $(shell python3 -c 'import numpy ; print(numpy.get_include())')

MODMESH_ROOT=modmesh_copy
MODMESH_PYMOD=$(MODMESH_ROOT)/modmesh/buffer/pymod

INC += -I$(shell python3-config --prefix)/include -I$(MODMESH_ROOT)

PYTHON_LIB := $(shell python3-config --prefix)/lib

CPATH := $(PYTHON_INC):$(NUMPY_INC)

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

data_prep.o: data_prep.cpp Makefile
	g++ -c $< -o $@ ${CXXFLAGS} -O3 -fPIC -std=c++17 ${INC}

data_prep.so: data_prep.o wrap_SimpleArray.o wrap_ConcreteBuffer.o buffer_pymod.o Makefile
	g++ $< wrap_ConcreteBuffer.o wrap_SimpleArray.o buffer_pymod.o -o $@ -shared -std=c++17 -lpython$(PYTHON_VERSION) -L$(PYTHON_LIB) ${LINKFLAGS}

solve_cpp_xarray.so: solve_cpp_xarray.cpp Makefile
	g++ $< -o $@ -O3 -fPIC -shared -std=c++17 -lpython$(PYTHON_VERSION) -L$(PYTHON_LIB) ${INC}

../image/03_solve_cpp_xarray.png: 03_solve_cpp_xarray.py solve_cpp_xarray.so
	./$<

%.o: $(MODMESH_PYMOD)/%.cpp Makefile
	g++ -c $< -o $@ -O3 -fPIC -std=c++17 ${INC}

solve_cpp.o: solve_cpp.cpp Makefile
	g++ -c $< -o $@ -O3 -fPIC -std=c++17 ${INC}

solve_cpp.so: solve_cpp.o wrap_SimpleArray.o wrap_ConcreteBuffer.o buffer_pymod.o Makefile
	g++ $< wrap_ConcreteBuffer.o wrap_SimpleArray.o buffer_pymod.o -o $@ -shared -std=c++17 -lpython$(PYTHON_VERSION) -L$(PYTHON_LIB)

../image/03_solve_cpp.png: 03_solve_cpp.py solve_cpp.so
	./$<

../image/04_fit_poly.png: 04_fit_poly.py data_prep.so
	./$<

../image/%.png: %.py
	./$<

.PHONY: clean
clean:
	rm -rf *.o *.so
