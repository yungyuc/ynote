#include <pybind11/pybind11.h> // must be first
#include <string>
#include <iostream>
namespace py = pybind11;
PYBIND11_MODULE(_pylist, mod) {
  mod.def(
    "do",
    [](py::list & l) {
      // convert contents to std::string and send to cout
      std::cout << "std::cout:" << std::endl;
      for (py::handle o : l) {
        std::string s = py::cast<std::string>(o);
        std::cout << s << std::endl;
      }
    }
  );
  mod.def(
    "do2",
    [](py::list & l) {
      // create a new list
      std::cout << "py::print:" << std::endl;
      py::list l2;
      for (py::handle o : l) {
        std::string s = py::cast<std::string>(o);
        s = "elm:" + s;
        py::str s2(s);
        l2.append(s2); // populate contents
      }
      py::print(l2);
    }
  );
} /* end PYBIND11_PLUGIN(_pylist) */
