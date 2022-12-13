#include <pybind11/pybind11.h>
#include <modmesh/buffer/buffer.hpp>
#include <modmesh/buffer/pymod/buffer_pymod.hpp>

#include <vector>
#include <algorithm>
#include <tuple>
#include <iostream>

#include <pybind11/numpy.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace modmesh
{

namespace python
{

void import_numpy()
{
    auto local_import_numpy = []()
    {
        import_array2("cannot import numpy", false); // or numpy c api segfault.
        return true;
    };
    if (!local_import_numpy())
    {
        throw pybind11::error_already_set();
    }
}

}

}

template <typename T>
T calc_norm_amax(modmesh::SimpleArray<T> const & arr0, modmesh::SimpleArray<T> const & arr1)
{
    size_t const nelm = arr0.size();
    T ret = 0;
    for (size_t it = 0; it < nelm; ++it)
    {
        T const val = std::abs(arr0[it] - arr1[it]);
        if (val > ret)
        {
            ret = val;
        }
    }
    return ret;
}

// [begin example]
std::tuple<modmesh::SimpleArray<double>, size_t, double>
solve1(modmesh::SimpleArray<double> u)
{
    const size_t nx = u.shape(0);
    modmesh::SimpleArray<double> un = u;
    bool converged = false;
    size_t step = 0;
    double norm;
    while (!converged)
    {
        ++step;
        for (size_t it=1; it<nx-1; ++it)
        {
            for (size_t jt=1; jt<nx-1; ++jt)
            {
                un(it,jt) = (u(it+1,jt) + u(it-1,jt) + u(it,jt+1) + u(it,jt-1)) / 4;
            }
        }
        norm = calc_norm_amax(u, un);
        if (norm < 1.e-5) { converged = true; }
        u = un;
    }
    return std::make_tuple(u, step, norm);
}

PYBIND11_MODULE(solve_cpp_simplearray, m)
{
    modmesh::python::import_numpy();
    modmesh::python::wrap_ConcreteBuffer(m);
    modmesh::python::wrap_SimpleArray(m);
    m.def
    (
        "solve_cpp"
      , [](pybind11::array_t<double> & uin)
        {
            return solve1(modmesh::python::makeSimpleArray(uin));
        }
    );
}
// [end example]

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
