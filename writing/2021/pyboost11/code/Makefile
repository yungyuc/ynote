FLAGS ?=
VERBOSE ?=

pyextsuffix := $(shell python3-config --extension-suffix)

HEADERS := item.hpp pyboost11.hpp

.PHONY: default
default: modpyb$(pyextsuffix) modbpy$(pyextsuffix)

modpyb$(pyextsuffix): modpyb.cpp $(HEADERS) CMakeLists.txt
	mkdir -p build ; \
	cd build ; \
	cmake .. -DCMAKE_BUILD_TYPE=Release
	env VERBOSE=$(VERBOSE) make -C build modpyb_copy

modbpy$(pyextsuffix): modbpy.cpp $(HEADERS) CMakeLists.txt
	mkdir -p build ; \
	cd build ; \
	cmake .. -DCMAKE_BUILD_TYPE=Release
	env VERBOSE=$(VERBOSE) make -C build modbpy_copy

%: %.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

ALL_FILES := Makefile CMakeLists.txt ${HEADERS} modpyb.cpp modbpy.cpp *.py

.PHONY: tar
tar: ${ALL_FILES}
	tar cfz pyboost11_code.tar.gz ${ALL_FILES}

.PHONY: test
test: modpyb$(pyextsuffix) modbpy$(pyextsuffix)
	env PYTHON_PATH=.:${PYTHON_PATH} python3 -m pytest -v -s

.PHONY: clean
clean:
	rm -rf __pycache__ *.pyc *.pyo build *.so *.dylib
