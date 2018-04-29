#include <pybind11/pybind11.h> // must be first
#include <vector>
namespace py = pybind11;
PYBIND11_MODULE(_pytuple, mod) {
  mod.def(
    "do",
    [](py::args & args) {
      py::list l;
      for (py::handle h : args) {
        l.append(h);
      }
      py::tuple t(l);
      py::print(py::str("{} len={}").format(t, t.size()));
      for (size_t it=0; it<t.size(); ++it) {
        py::print(py::str("{}").format(t[it]));
      }
    }
  );
} /* end PYBIND11_PLUGIN(_pytuple) */
