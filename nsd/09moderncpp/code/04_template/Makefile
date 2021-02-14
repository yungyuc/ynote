FLAGS ?=

OPT ?= -O3

CXX = g++
CXXFLAGS += -std=c++17 -g $(OPT) $(FLAGS)

BINS := 01_factory

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
