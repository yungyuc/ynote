#include <boost/python.hpp>

#include "header.hpp"

BOOST_PYTHON_MODULE(pymod)
{
    using namespace boost::python;

    class_<Object, SHARED_PTR<Object>, boost::noncopyable>
    ("Object", no_init)
        .def("print_status", &Object::print_status)
        .staticmethod("print_status")
        .def("make", &Object::make)
        .staticmethod("make")
        .def("get_strong", &Object::get_strong)
        .staticmethod("get_strong")
        .def("get_strong_count", &Object::get_strong_count)
        .staticmethod("get_strong_count")
        .def("reset_strong", &Object::reset_strong)
        .staticmethod("reset_strong")
        .def("get_weak_count", &Object::get_weak_count)
        .staticmethod("get_weak_count")
        .def("reset_weak", &Object::reset_weak)
        .staticmethod("reset_weak")
    ;

    class_<Holder>
    ("Holder", no_init)
        .def(init<const SHARED_PTR<Object> &>())
        .def("get_count", &Holder::get_count)
    ;

    def("print_from_python", print_from_python);
    def("get_cpp_count", get_cpp_count);

    scope().attr("wrapper_type") = std::string("bpy");

    scope().attr("boost_shared_ptr") =
#ifdef USE_BOOST_SHARED_PTR
        true
#else // USE_BOOST_SHARED_PTR
        false
#endif // USE_BOOST_SHARED_PTR
    ;

    scope().attr("shared_from_this") =
#ifdef USE_ENABLE_SHARED_FROM_THIS
        true
#else // USE_ENABLE_SHARED_FROM_THIS
        false
#endif // USE_ENABLE_SHARED_FROM_THIS
    ;
}

// vim: set ai et nu sw=4 ts=4:
