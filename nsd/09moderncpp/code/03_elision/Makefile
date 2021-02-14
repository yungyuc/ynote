FLAGS ?=

OPT ?= -O3

CXX = g++
CXXFLAGS += -std=c++17 -g $(OPT) $(FLAGS)

BINS := 01_copy 02_move 03_accumulate

.PHONY: default
default: $(BINS)

.PHONY: runall
runall: $(patsubst %,run_%,$(BINS))

.PHONY: run_%
run_%: %
	./$<

%: %.cpp Makefile
	$(CXX) $< -o $@ $(CXXFLAGS)

.PHONY: 01_copy_sym
01_copy_sym: 01_copy
	r2 -Aqc "e scr.color=0 ; afl" $<

.PHONY: 01_copy_asm
01_copy_asm: 01_copy
	r2 -Aqc "e scr.color=0 ; sf sym.worker1__ ; pdf" $<
	r2 -Aqc "e scr.color=0 ; sf sym.worker2__ ; pdf" $<
	r2 -Aqc "e scr.color=0 ; sf sym.entry0 ; pdf" $<
	r2 -Aqc "e scr.color=0 ; sf sym.Data::Data__ ; pdf" $<

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ $(BINS)
