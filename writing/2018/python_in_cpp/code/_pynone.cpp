#include <pybind11/pybind11.h> // must be first
#include <iostream>
namespace py = pybind11;
using namespace py::literals; // to bring in the `_s` literal
PYBIND11_MODULE(_pynone, mod) {
  mod.def(
    "do",
    [](py::object const & o) {
      if (o.is(py::none())) {
        std::cout << "it is None" << std::endl;
      } else {
        std::cout << "it is not None" << std::endl;
      }
    }
  );
} /* end PYBIND11_PLUGIN(_pynone) */
