#include <pybind11/pybind11.h> // must be first
#include <vector>
#include <string>
namespace py = pybind11;
PYBIND11_MODULE(_helloworld, mod) {
  mod.def(
    "do",
    []() {
      std::vector<char> v{'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd'};
      py::list l;
      for (auto & i : v) {
        py::str s(std::string(1, i));
        l.append(s);
      }
      return l;
    },
    "a little more interesting hello world"
  );
} /* end PYBIND11_PLUGIN(_helloworld) */
