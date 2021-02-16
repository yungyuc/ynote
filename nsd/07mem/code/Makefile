CC = gcc
CFLAGS = -std=c11 -O3 -g
CXX = g++
CXXFLAGS = -std=c++17 -O3 -g

BINS := cmem cppmem alloc icount
BADBINS := mismatch

.PHONY: default
default: $(BINS) $(BADBINS)

.PHONY: runall
runall: $(patsubst %,run_%,$(BINS))

.PHONY: run_%
run_%: %
	./$<

%: %.c Makefile
	${CC} $< -o $@ $(CFLAGS)

%: %.cpp Makefile
	${CXX} $< -o $@ $(CXXFLAGS)

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ $(BINS) $(BADBINS)
