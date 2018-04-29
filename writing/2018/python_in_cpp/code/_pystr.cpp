#include <pybind11/pybind11.h> // must be first
#include <string>
#include <iostream>
namespace py = pybind11;
using namespace py::literals; // to bring in the `_s` literal
PYBIND11_MODULE(_pystr, mod) {
  mod.def(
    "do",
    []() {
      py::str s("python string {}"_s.format("formatting"));
      py::print(s);
    }
  );
} /* end PYBIND11_PLUGIN(_pystr) */
