.PHONY: build
build:
	python3 setup.py build_ext --inplace

.PHONY: run
run: build
	python3 -c "import python_example as pe; print(pe.add(1, 2)); print(pe.subtract(1, 2))"

.PHONY: clean
clean:
	rm -rf build/ *.pyc *.pyo *.so *.dylib
