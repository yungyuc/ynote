FLAGS ?=

UNAME_S := $(shell uname -s)

CXXFLAGS += -std=c++17 -O3 -g -m64 ${FLAGS}
INC ?=

ifeq ($(UNAME_S),Darwin)
# Do not define "HASMKL" macro.
INC += -I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers
LINKFLAGS := -framework Accelerate
else ifeq ($(UNAME_S),Linux)
INC += -I/usr/include/mkl

CXXFLAGS += -DHASMKL -Wl,--no-as-needed

MKLLIB ?= /usr/lib/x86_64-linux-gnu
MKLEXT ?= so

LINKFLAGS := \
	${MKLLIB}/libmkl_intel_lp64.${MKLEXT} \
	${MKLLIB}/libmkl_sequential.${MKLEXT} \
	${MKLLIB}/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl
endif

CXX = g++

# Array.
PODBINS := pod01_vector pod02_matrix_auto pod03_matrix_rowmajor pod04_matrix_colmajor
BINS += $(PODBINS)
# Matrix class.
MABINS := ma01_matrix_class ma02_matrix_vector ma03_matrix_matrix
BINS += $(MABINS)
# Linear algebra.
LABINS := la01_gesv la02_geev la03_syev la04_gesvd la05_gels
BINS += $(LABINS)

.PHONY: default
default: ${BINS}

.PHONY: runpod
runpod: $(patsubst %,run_%,$(PODBINS))

.PHONY: runma
runma: $(patsubst %,run_%,$(MABINS))

.PHONY: runla
runla: $(patsubst %,run_%,$(LABINS))

.PHONY: runall
runall: $(patsubst %,run_%,$(BINS))

.PHONY: run_%
run_%: %
	./$<

pod%: pod%.cpp Makefile
	${CXX} $< -o $@ ${CXXFLAGS}

ma%: ma%.cpp Makefile
	${CXX} $< -o $@ ${CXXFLAGS}

la%: la%.cpp Makefile
	${CXX} $< -o $@ ${CXXFLAGS} ${INC} ${LINKFLAGS}

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
