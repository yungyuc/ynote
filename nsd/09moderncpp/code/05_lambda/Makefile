FLAGS ?=

OPT ?= -O3

CXX = g++
CXXFLAGS += -std=c++17 -g $(OPT) $(FLAGS)

BINS := 01_lambda 02_stored 02_stored_demangle 03_closure

.PHONY: default
default: $(BINS)

.PHONY: runall
runall: $(patsubst %,run_%,$(BINS))

.PHONY: run_%
run_%: %
	./$<

%: %.cpp Makefile
	$(CXX) $< -o $@ $(CXXFLAGS)

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ $(BINS)
