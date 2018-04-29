# Haven't got this working with anaconda on osx
RUNTIME_PREFIX?=$(shell python3-config --prefix)
CXX = clang++
CXXFLAGS = -I${RUNTIME_PREFIX}/include -O0 -g -fPIC -std=c++11 -stdlib=libc++
CXXFLAGS_PY = $(shell python3-config --includes)
LDFLAGS = -fPIC -L${RUNTIME_PREFIX}/lib -stdlib=libc++ -lstdc++
LDFLAGS_PY = -shared $(shell python3-config --libs)

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
