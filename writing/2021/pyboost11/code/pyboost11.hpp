#pragma once

#include <pybind11/pybind11.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <boost/python.hpp>
#pragma GCC diagnostic pop

namespace pyboost11
{

// Pybind11 cast by using boost.python.
template <typename T> struct caster
{

    caster(pybind11::handle src)
      : obj(boost::python::handle<>(boost::python::borrowed(src.ptr())))
      , ext(obj)
    {}

    bool check() const { return ext.check(); }

    // From-Python conversion.
    operator T() { return ext(); }
    T operator()() { return ext(); }

    // To-Python conversion.
    static pybind11::handle to_python(T & src)
    {
        namespace bpy = boost::python;
        return bpy::incref(bpy::object(src).ptr());
    }

    boost::python::object obj;
    boost::python::extract<T> ext;

};

} // end namespace pyboost11

namespace pybind11
{

namespace detail
{

template <typename type> struct pyboost11_type_caster
{

// Expanded from PYBIND11_TYPE_CASTER.
protected:
    type value;
public:
    template <typename T_, enable_if_t<std::is_same<type, remove_cv_t<T_>>::value, int> = 0>
    static handle cast(T_ *src, return_value_policy policy, handle parent) {
        if (!src) return none().release();
        if (policy == return_value_policy::take_ownership) {
            auto h = cast(std::move(*src), policy, parent); delete src; return h;
        } else {
            return cast(*src, policy, parent);
        }
    }
    operator type*() { return &value; }
    operator type&() { return value; }
    operator type&&() && { return std::move(value); }
    template <typename T_> using cast_op_type = pybind11::detail::movable_cast_op_type<T_>;

    // Boilerplate.
    bool load(handle src, bool)
    {
        if (!src)
        {
            return false;
        }
        pyboost11::caster<type> ext(src);
        if (!ext.check())
        {
            return false;
        }
        value = ext();
        return true;
    }
    static handle cast(type * src, return_value_policy /* policy */, handle /* parent */)
    {
        return pyboost11::caster<type>::to_python(src);
    }
    static handle cast(type src, return_value_policy /* policy */, handle /* parent */)
    {
        return pyboost11::caster<type>::to_python(src);
    }

};

#define PYBOOST11_TYPE_CASTER(type, py_name) \
    template <> struct type_caster<type> : public pyboost11_type_caster<type> \
    { static constexpr auto name = py_name; }

} // end namespace detail

} // end namespace pybind11

namespace pyboost11
{

// Boost.python convert by using pybind11.
template <typename T> struct converter
{

public:

    converter() { init(); }

    void init()
    {
        static bool initialized = false;
        if (!initialized)
        {
            namespace bpy = boost::python;
            // From-Python conversion.
            bpy::converter::registry::push_back
            (
                &convertible
              , &construct
              , bpy::type_id<T>()
            );
            // To-Python conversion.
            bpy::to_python_converter<T, converter>();

            initialized = true;
        }
    }

    // From-Python convertibility.
    static void * convertible(PyObject * objptr)
    {
        namespace pyb = pybind11;
        try
        {
            pyb::handle(objptr).cast<T>();
            return objptr;
        }
        catch (pyb::cast_error const &)
        {
            return nullptr;
        }
    }

    // From-Python conversion.
    static void construct
    (
        PyObject * objptr
      , boost::python::converter::rvalue_from_python_stage1_data * data
    )
    {
        namespace pyb = pybind11;
        void * storage = reinterpret_cast
        <
            boost::python::converter::rvalue_from_python_storage<T> *
        >(data)->storage.bytes;
        new (storage) T(pyb::handle(objptr).cast<T>());
        data->convertible = storage;
    }

    // To-Python conversion.
    static PyObject * convert(T const & t)
    {
        return pybind11::cast(t).inc_ref().ptr();
    }

};

} // end namespace pyboost11

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
