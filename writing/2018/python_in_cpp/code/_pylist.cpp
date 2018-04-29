#include <pybind11/pybind11.h> // must be first
#include <string>
#include <iostream>
namespace py = pybind11;
PYBIND11_MODULE(_pylist, mod) {
  mod.def(
    "do",
    [](py::list & l) {
      std::cout << "std::cout:" << std::endl;
      for (py::handle obj : l) {
        std::string str = py::cast<std::string>(obj);
        std::cout << str << std::endl;
      }
      std::cout << "py::print:" << std::endl;
      for (size_t it=0; it<l.size(); ++it) {
        py::print(py::str("{}").format(l[it]));
      }
      // operator+() doesn't work:
      // py::list newl = l + l;
    }
  );
} /* end PYBIND11_PLUGIN(_pylist) */
