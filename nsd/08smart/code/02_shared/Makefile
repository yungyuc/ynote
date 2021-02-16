FLAGS ?=

CXX := g++
CXXFLAGS += -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror $(FLAGS)

BINS := 01_fully 03_fromthis 04_cyclic 05_weak
BADBINS := 02_duplicate

.PHONY: default
default: $(BINS) $(BADBINS)

.PHONY: runall
runall: $(patsubst %,run_%,$(BINS))

.PHONY: run_%
run_%: %
	./$<

%: %.cpp Makefile
	$(CXX) $< -o $@ $(CXXFLAGS)

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ $(BINS) $(BADBINS)
