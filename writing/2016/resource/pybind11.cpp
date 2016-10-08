#include <pybind11/pybind11.h>

#include "header.hpp"

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, SHARED_PTR<T>);

PYBIND11_PLUGIN(pymod) {

    py::module mod("pymod");

    py::class_<Object, SHARED_PTR<Object>>(mod, "Object")
        .def_static("print_status", &Object::print_status)
        .def_static("make", &Object::make)
        .def_static("get_strong", &Object::get_strong)
        .def_static("get_strong_count", &Object::get_strong_count)
        .def_static("reset_strong", &Object::reset_strong)
        .def_static("get_weak_count", &Object::get_weak_count)
        .def_static("reset_weak", &Object::reset_weak)
    ;

    py::class_<Holder>(mod, "Holder")
        .def(py::init<const SHARED_PTR<Object> &>())
        .def("get_count", &Holder::get_count)
    ;

    mod.def("print_from_python", print_from_python);
    mod.def("get_cpp_count", get_cpp_count);

    mod.attr("wrapper_type") = py::str("pybind11");

    mod.attr("boost_shared_ptr") = py::bool_(
#ifdef USE_BOOST_SHARED_PTR
        true
#else // USE_BOOST_SHARED_PTR
        false
#endif // USE_BOOST_SHARED_PTR
    );

    mod.attr("shared_from_this") = py::bool_(
#ifdef USE_ENABLE_SHARED_FROM_THIS
        true
#else // USE_ENABLE_SHARED_FROM_THIS
        false
#endif // USE_ENABLE_SHARED_FROM_THIS
    );

    return mod.ptr();
}

// vim: set ai et nu sw=4 ts=4:
