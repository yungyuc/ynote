FLAGS ?=

UNAME_S := $(shell uname -s)

CXXFLAGS += -std=c++17 -O3 -g -m64 ${FLAGS}

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

CXX = g++

BINS := 01_skip_access 02_locality 03_matrix_matrix

.PHONY: default
default: ${BINS}

.PHONY: runall
runall: $(patsubst %,run_%,$(BINS))

.PHONY: run_%
run_%: %
	./$<

03_matrix_matrix: 03_matrix_matrix.cpp StopWatch.hpp Makefile
	${CXX} $< -o $@ ${CXXFLAGS} -I${MKLINC} ${MKLLINKLINE}

%: %.cpp StopWatch.hpp Makefile
	${CXX} $< -o $@ ${CXXFLAGS}

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
