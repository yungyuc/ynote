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

.PHONY: run_sptr3
run_sptr3: sptr3.o
	$(CXX) $< -o $@ $(LDFLAGS)
	./$@

.PHONY: run_sptr4
run_sptr4: sptr4.o
	$(CXX) $< -o $@ $(LDFLAGS)
	./$@

.PHONY: run_sptr5
run_sptr5: sptr5.o
	$(CXX) $< -o $@ $(LDFLAGS)
	./$@

.PHONY: run_move1
run_move1: move1.o
	$(CXX) $< -o $@ $(LDFLAGS)
	./$@

.PHONY: run_move2
run_move2: move2.o
	$(CXX) $< -o $@ $(LDFLAGS)
	./$@

.PHONY: run_lambda1
run_lambda1: lambda1.o
	$(CXX) $< -o $@ $(LDFLAGS)
	./$@

.PHONY: run_vart1
run_vart1: vart1.o
	$(CXX) $< -o $@ $(LDFLAGS)
	./$@

.PHONY: default
clean:
	rm -rf *.o *.so *.dSYM/
