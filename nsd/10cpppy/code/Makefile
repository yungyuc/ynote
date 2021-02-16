export PYTHONPATH=modmesh-master:turgon-master/spacetime

BASES := 01_linear 01_burgers 04_iter
SCRIPTS := $(patsubst %,%.py,$(BASES))
IMAGES := $(patsubst %,../image/%.png,$(BASES))

.PHONY: default
default: $(IMAGES)

.PHONY: modmesh
modmesh:
	./build.sh modmesh

.PHONY: libst
libst:
	./build.sh libst

.PHONY: testimport
testimport: libst
	python3 -c 'import modmesh ; print(modmesh)'
	python3 -c 'import libst ; print(libst)'

../image/%.png: %.py
	./$<
