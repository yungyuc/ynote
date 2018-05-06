# Haven't got this working with anaconda on osx
OS_S = $(shell uname -s)
RUNTIME_PREFIX?=$(shell python3-config --prefix)
DB = gdb
CXX = c++
CXXFLAGS = -I${RUNTIME_PREFIX}/include -O0 -g -fPIC -std=c++11
LDFLAGS = -fPIC -L${RUNTIME_PREFIX}/lib -lstdc++

ifeq ($(OS_S),Darwin)
DB = lldb
CXXFLAGS += -stdlib=libc++
LDFLAGS += -stdlib=libc++
endif

DEPEND=Makefile

.PHONY: default
default: all

.PHONY: all
all: run_sptr2 run_sptr1 run_sptr

%.o: %.cpp $(DEPEND)
	$(CXX) $< -o $@ -c $(CXXFLAGS)

.PHONY: run_sptr
run_sptr: sptr.o
	$(CXX) $< -o $@ $(LDFLAGS)
	./$@

.PHONY: run_sptr1
run_sptr1: sptr1.o
	$(CXX) $< -o $@ $(LDFLAGS)
	./$@

.PHONY: run_sptr2
run_sptr2: sptr2.o
	$(CXX) $< -o $@ $(LDFLAGS)
	./$@

.PHONY: default
clean:
	rm -rf *.o *.so *.dSYM/
