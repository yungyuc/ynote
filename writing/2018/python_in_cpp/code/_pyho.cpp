#include <pybind11/pybind11.h> // must be first
#include <iostream>
namespace py = pybind11;
using namespace py::literals; // to bring in the `_s` literal
PYBIND11_MODULE(_pyho, mod) {
  mod.def(
    "do",
    [](py::object const & o) {
      std::cout << "refcount in the beginning: "
                << o.ptr()->ob_refcnt << std::endl;
      py::handle h(o);
      std::cout << "no increase of refcount with a new pybind11::handle: "
                << h.ptr()->ob_refcnt << std::endl;
      {
        py::object o2(o);
        std::cout << "increased refcount with a new pybind11::object: "
                  << o2.ptr()->ob_refcnt << std::endl;
      }
      std::cout << "decreased refcount after the new pybind11::object destructed: "
                << o.ptr()->ob_refcnt << std::endl;
      h.inc_ref();
      std::cout << "manually increases refcount after h.inc_ref(): "
                << h.ptr()->ob_refcnt << std::endl;
      h.dec_ref();
      std::cout << "manually descrases refcount after h.dec_ref(): "
                << h.ptr()->ob_refcnt << std::endl;
    }
  );
} /* end PYBIND11_PLUGIN(_pyho) */
