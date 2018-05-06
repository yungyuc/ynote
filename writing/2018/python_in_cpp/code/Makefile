# Haven't got this working with anaconda on osx
OS_S = $(shell uname -s)
RUNTIME_PREFIX?=$(shell python3-config --prefix)
CXX = c++
CXXFLAGS = -I${RUNTIME_PREFIX}/include -O0 -g -fPIC -std=c++11
CXXFLAGS_PY = $(shell python3-config --includes)
LDFLAGS = -fPIC -L${RUNTIME_PREFIX}/lib -lstdc++
LDFLAGS_PY = -shared $(shell python3-config --libs)

ifeq ($(OS_S),Darwin)
CXXFLAGS += -stdlib=libc++
LDFLAGS += -stdlib=libc++
endif

DEPEND=Makefile

.PHONY: default
default: all

.PHONY: all
all: helloworld pylist pytuple pydict pystr pyho pynone

%.so: %.cpp $(DEPEND)
	$(CXX) $< -o $@ $(CXXFLAGS) $(CXXFLAGS_PY) $(LDFLAGS) $(LDFLAGS_PY)

.PHONY: helloworld
helloworld: _helloworld.so
	python3 -m helloworld

.PHONY: pylist
pylist: _pylist.so
	python3 -m pylist

.PHONY: pytuple
pytuple: _pytuple.so
	python3 -m pytuple

.PHONY: pydict
pydict: _pydict.so
	python3 -m pydict

.PHONY: pystr
pystr: _pystr.so
	python3 -m pystr

.PHONY: pyho
pyho: _pyho.so
	python3 -m pyho

.PHONY: pynone
pynone: _pynone.so
	python3 -m pynone

.PHONY: default
clean:
	rm -rf *.o *.so *.dSYM/
