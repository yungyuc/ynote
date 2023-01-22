FLAGS ?=

UNAME_S := $(shell uname -s)

CXXFLAGS += -std=c++17 -O3 -g -m64 ${FLAGS}

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

CXX = g++

BINS := 01_skip_access 02_locality 03_matrix_matrix

export VECLIB_MAXIMUM_THREADS=1

.PHONY: default
default: ${BINS}

.PHONY: runall
runall: $(patsubst %,run_%,$(BINS))

.PHONY: run_%
run_%: %
	./$<

03_matrix_matrix: 03_matrix_matrix.cpp StopWatch.hpp Makefile
	${CXX} $< -o $@ ${CXXFLAGS} ${INC} ${LINKFLAGS}

%: %.cpp StopWatch.hpp Makefile
	${CXX} $< -o $@ ${CXXFLAGS}

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
