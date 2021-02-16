BINS = \
	helloworld helloworld2 \
	hellomain hellomain_sys hellomain2 hellomain3 distance \
	types cstdint signness arrays float nlimits fpexc \
	class accessor reference constructor polymorphic crtp \
	vector array list map unordered_map

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLINC ?= ${MKLROOT}/include
MKLLIB ?= ${MKLROOT}/lib
MKLEXT ?= a
CXXFLAGS :=
BLASLINK := -lcblas
export DYLD_LIBRARY_PATH=.
# For CBLAS.
export CPATH=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers
else ifeq ($(UNAME_S),Linux)
MKLINC ?= /usr/include/mkl
MKLLIB ?= /usr/lib/x86_64-linux-gnu
MKLEXT ?= so
BLASLINK := \
	${MKLLIB}/libmkl_intel_lp64.${MKLEXT} \
	${MKLLIB}/libmkl_sequential.${MKLEXT} \
	${MKLLIB}/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl
CXXFLAGS := -Wl,--no-as-needed
export LD_LIBRARY_PATH=.
endif

.PHONY: default
default: $(BINS)

.PHONY: runall
runall: $(patsubst %,run_%,$(BINS))

.PHONY: run_%
run_%: %
	./$<

helloworld: helloworld.cpp Makefile
	g++ $< -o $@

helloworld.o: helloworld.cpp Makefile
	g++ -c $< -o $@

helloworld2: helloworld.o Makefile
	g++ $< -o $@

hello.o: hello.cpp hello.hpp Makefile
	g++ -c $< -o $@

hello_error: hello.cpp hello.hpp Makefile
	g++ $< -o $@

hellomain.o: hellomain.cpp Makefile
	g++ -c $< -o $@

hellomain.o_error: hellomain.cpp Makefile
	g++ $< -o hellomain.o

hellomain: hello.o hellomain.o Makefile
	g++ hello.o hellomain.o -o $@

hellomain_sys.o: hellomain_sys.cpp Makefile
	g++ -I. -c $< -o $@

hellomain_sys: hello.o hellomain_sys.o Makefile
	g++ hello.o hellomain_sys.o -o $@

hellomain_sys_error.o: hellomain_sys.cpp Makefile
	g++ -c $< -o $@

hellomain_sys_error: hello.o hellomain_sys_error.o Makefile
	g++ hello.o hellomain_sys_error.o -o $@

libhello.a: hello.o Makefile
	ar rcs $@ $<

hellomain2: hellomain.o libhello.a Makefile
	g++ hellomain.o -L. -lhello -o $@

hello_pic.o: hello.cpp hello.hpp Makefile
	g++ -c -fPIC $< -o $@

libshared_hello.so: hello_pic.o Makefile
	g++ -shared $< -o $@

hellomain3: hellomain.o libshared_hello.so Makefile
	g++ $< -L. -lshared_hello -o $@

distance: distance.cpp Makefile
	g++ $< -o $@ $(BLASLINK)

types: types.cpp Makefile
	g++ $< -o $@

cstdint: cstdint.cpp Makefile
	g++ $< -o $@

signness: signness.cpp Makefile
	g++ $< -o $@

signness_error: signness.cpp Makefile
	g++ $< -o $@ -Wsign-compare -Werror

arrays: arrays.cpp Makefile
	g++ $< -o $@ -Wall -Wextra -Werror

float: float.cpp Makefile
	g++ $< -o $@

nlimits: nlimits.cpp Makefile
	g++ $< -o $@

fpexc: fpexc.cpp Makefile
	g++ $< -o $@

class: class.cpp Makefile
	g++ $< -o $@ --std=c++11

accessor: accessor.cpp Makefile
	g++ $< -o $@ --std=c++11

reference: reference.cpp Makefile
	g++ $< -o $@ --std=c++11

constructor: constructor.cpp Makefile
	g++ $< -o $@ --std=c++11

polymorphic: polymorphic.cpp Makefile
	g++ $< -o $@ --std=c++11

crtp: crtp.cpp Makefile
	g++ $< -o $@ --std=c++11

vector: vector.cpp Makefile
	g++ $< -o $@ --std=c++11

array: array.cpp Makefile
	g++ $< -o $@ --std=c++11

list: list.cpp Makefile
	g++ $< -o $@ --std=c++11

map: map.cpp Makefile
	g++ $< -o $@ --std=c++11

unordered_map: unordered_map.cpp Makefile
	g++ $< -o $@ --std=c++11

.PHONY: clean
clean:
	rm -f *.o *.so *.a *.dSYM/ $(BINS)
