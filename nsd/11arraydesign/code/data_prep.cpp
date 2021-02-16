#include <pybind11/pybind11.h>
#define FORCE_IMPORT_ARRAY
#include <xtensor-python/pyarray.hpp>

#include <vector>
#include <algorithm>

#include <xtensor/xarray.hpp>
#include <xtensor/xadapt.hpp>
#include <xtensor/xview.hpp>
#include <xtensor-blas/xlinalg.hpp>

using array_type = xt::xarray<double>;

template <class AT>
xt::xarray<double> fit_poly(AT & xarr, AT & yarr, size_t order)
{
    if (xarr.size() != yarr.size()) { throw std::runtime_error("xarr and yarr size mismatch"); }

    xt::xarray<double> matrix(std::vector<size_t>{order+1, order+1});

    for (size_t it=0; it<order+1; ++it)
    {
        for (size_t jt=0; jt<order+1; ++jt)
        {
            double & val = matrix(it, jt);
            val = 0;
            for (size_t kt=0; kt<xarr.size(); ++kt) { val += pow(xarr[kt], it+jt); }
        }
    }

    xt::xarray<double> rhs(std::vector<size_t>{order+1});
    for (size_t jt=0; jt<order+1; ++jt)
    {
        rhs[jt] = 0;
        for (size_t kt=0; kt<yarr.size(); ++kt) { rhs[jt] += pow(xarr[kt], jt) * yarr[kt]; }
    }

    xt::xarray<double> lhs = xt::linalg::solve(matrix, rhs);
    std::reverse(lhs.begin(), lhs.end()); // to make numpy.poly1d happy.

    return lhs;
}

template <class AT>
xt::xarray<double> fit_polys(xt::xarray<double> & xarr, xt::xarray<double> & yarr, size_t order)
{
    size_t xmin = std::floor(*std::min_element(xarr.begin(), xarr.end()));
    size_t xmax = std::ceil(*std::max_element(xarr.begin(), xarr.end()));
    size_t ninterval = xmax - xmin;

    xt::xarray<double> lhs(std::vector<size_t>{ninterval, order+1});
    lhs.fill(0); // sentinel.
    size_t start=0;
    for (size_t it=0; it<xmax; ++it)
    {
        size_t stop;
        for (stop=start; stop<xarr.size(); ++stop) { if (xarr[stop]>=it+1) { break; } }

        AT const sub_x = xt::view(xarr, xt::range(start, stop));
        AT const sub_y = xt::view(yarr, xt::range(start, stop));

        xt::xarray<double> sub_lhs = fit_poly(sub_x, sub_y, order);
        xt::view(lhs, it, xt::all()) = sub_lhs;

        start = stop;
    }

    return lhs;
}

PYBIND11_MODULE(data_prep, m)
{
    xt::import_numpy();
    m.def
    (
        "fit_poly"
      , [](xt::pyarray<double> & xarr_in, xt::pyarray<double> & yarr_in, size_t order)
        {
            std::vector<size_t> xarr_shape(xarr_in.shape().begin(), xarr_in.shape().end());
            xt::xarray<double> xarr = xt::adapt(xarr_in.data(), xarr_shape);

            std::vector<size_t> yarr_shape(yarr_in.shape().begin(), yarr_in.shape().end());
            xt::xarray<double> yarr = xt::adapt(yarr_in.data(), yarr_shape);

            return fit_poly(xarr, yarr, order);
        }
    );
    m.def
    (
        "fit_polys"
      , [](xt::pyarray<double> & xarr_in, xt::pyarray<double> & yarr_in, size_t order)
        {
            std::vector<size_t> xarr_shape(xarr_in.shape().begin(), xarr_in.shape().end());
            xt::xarray<double> xarr = xt::adapt(xarr_in.data(), xarr_shape);
            std::vector<size_t> yarr_shape(yarr_in.shape().begin(), yarr_in.shape().end());
            xt::xarray<double> yarr = xt::adapt(yarr_in.data(), yarr_shape);
            return fit_polys<array_type>(xarr, yarr, order);
        }
    );
}
// [end example]

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
