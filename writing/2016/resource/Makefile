CXX = clang++
CXXFLAGS = -I${CONDA_PREFIX}/include -O0 -g -fPIC -std=c++11 -stdlib=libc++
CXXFLAGS_PY = $(shell python3-config --includes)
LDFLAGS = -fPIC -L${CONDA_PREFIX}/lib -stdlib=libc++ -lstdc++
LDFLAGS_PY = -lboost_python -lboost_system $(shell python3-config --libs)
WTYPE ?= bpy

ifeq ($(WTYPE), "bpy")
CXXFLAGS += -DWTYPE_BPY=1
endif
ifndef NOUSE_BOOST_SHARED_PTR
CXXFLAGS += -DUSE_BOOST_SHARED_PTR
endif
ifdef USE_ENABLE_SHARED_FROM_THIS
CXXFLAGS += -DUSE_ENABLE_SHARED_FROM_THIS
endif

DEPEND=Makefile header.hpp

default: run

pymod.o: $(WTYPE).cpp $(DEPEND)
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS_PY)

pymod.so: pymod.o
	$(CXX) -shared $< -o $@ $(LDFLAGS) $(LDFLAGS_PY)

run: pymod.so
	python3 pydrive.py

debug: pymod.so
	lldb -- python3 pydrive.py

ex_shared_ptr_run: ex_shared_ptr.cpp Makefile
	$(CXX) $< -o $@ $(CXXFLAGS)
	./$@

ex_bpy_to_python: ex_bpy_to_python.cpp Makefile
	$(CXX) -shared $< -o $@.so $(CXXFLAGS) $(CXXFLAGS_PY) $(LDFLAGS) $(LDFLAGS_PY)
	python3 $@.py

ex_bpy_from_python: ex_bpy_from_python.cpp Makefile
	$(CXX) -shared $< -o $@.so $(CXXFLAGS) $(CXXFLAGS_PY) $(LDFLAGS) $(LDFLAGS_PY)
	python3 $@.py

ex_bpy_hold_weak: ex_bpy_hold_weak.cpp Makefile
	$(CXX) -shared $< -o $@.so $(CXXFLAGS) $(CXXFLAGS_PY) $(LDFLAGS) $(LDFLAGS_PY)
	python3 $@.py

ex_bpy_shared_from_this: ex_bpy_shared_from_this.cpp Makefile
	$(CXX) -shared $< -o $@.so $(CXXFLAGS) $(CXXFLAGS_PY) $(LDFLAGS) $(LDFLAGS_PY)
	python3 $@.py

ex_bpy_std: ex_bpy_std.cpp Makefile
	$(CXX) -shared $< -o $@.so $(CXXFLAGS) $(CXXFLAGS_PY) $(LDFLAGS) $(LDFLAGS_PY)
	python3 $@.py

ex_pyb_std: ex_pyb_std.cpp Makefile
	$(CXX) -shared $< -o $@.so $(CXXFLAGS) $(CXXFLAGS_PY) $(LDFLAGS) $(LDFLAGS_PY)
ifdef PYB_DEBUG
	lldb -- python3 $@.py
else
	python3 $@.py
endif

ex_pyb_boost: ex_pyb_boost.cpp Makefile
	$(CXX) -shared $< -o $@.so $(CXXFLAGS) $(CXXFLAGS_PY) $(LDFLAGS) $(LDFLAGS_PY)
	python3 $@.py

clean:
	rm -rf *.o *.so ex_*_run *.dSYM/

.PHONY: default run debug clean ex_*_run
