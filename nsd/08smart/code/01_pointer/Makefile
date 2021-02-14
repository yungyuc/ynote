FLAGS ?=

CXX := g++
CXXFLAGS += -std=c++17 -g -O3 -m64 -Wall -Wextra -Werror $(FLAGS)

BINS := 01_raw_pointer 02_reference 03_ownership 04_unique 05_shared

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
