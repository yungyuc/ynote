#include <pybind11/pybind11.h> // must be first
#include <string>
#include <stdexcept>
#include <iostream>
namespace py = pybind11;
PYBIND11_MODULE(_pydict, mod) {
  mod.def(
    "do",
    [](py::args & args) {
      if (args.size() % 2 != 0) {
        throw std::runtime_error("argument number must be even");
      }
      // create a dict from the input tuple
      py::dict d;
      for (size_t it=0; it<args.size(); it+=2) {
        d[args[it]] = args[it+1];
      }
      return d;
    }
  );
  mod.def(
    "do2",
    [](py::dict d, py::args & args) {
      for (py::handle h : args) {
        if (d.contains(h)) {
          std::cout << py::cast<std::string>(h)
                    << " is in the input dictionary" << std::endl;
        } else {
          std::cout << py::cast<std::string>(h)
                    << " is not found in the input dictionary" << std::endl;
        }
      }
      std::cout << "remove everything in the input dictionary!" << std::endl;
      d.clear();
      return d;
    }
  );
} /* end PYBIND11_PLUGIN(_pydict) */
