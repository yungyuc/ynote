#include <pybind11/pybind11.h>
#define FORCE_IMPORT_ARRAY
#include <xtensor-python/pyarray.hpp>

#include <vector>
#include <algorithm>
#include <tuple>
#include <iostream>

#include <xtensor/xarray.hpp>
#include <xtensor/xadapt.hpp>
#include <xtensor/xview.hpp>

std::tuple<xt::xarray<double>, size_t, double>
solve1(xt::xarray<double> u)
{
    const size_t nx = u.shape(0);
    xt::xarray<double> un = u;
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
        norm = xt::amax(xt::abs(un-u))();
        if (norm < 1.e-5) { converged = true; }
        u = un;
    }
    return std::make_tuple(u, step, norm);
}

PYBIND11_MODULE(solve_cpp, m)
{
    xt::import_numpy();
    m.def
    (
        "solve_cpp", [](xt::pyarray<double> & uin) { return solve1(xt::xarray<double>(uin)); }
    );
}
// [end example]

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
