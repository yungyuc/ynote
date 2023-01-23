#include <pybind11/pybind11.h>
#include <modmesh/buffer/buffer.hpp>
#include <modmesh/buffer/pymod/buffer_pymod.hpp>

#include <pybind11/numpy.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#ifdef HASMKL
#include <mkl_lapack.h>
#include <mkl_lapacke.h>
#else // HASMKL
#ifdef __MACH__
#include <clapack.h>
#include <Accelerate/Accelerate.h>
#endif // __MACH__
#endif // HASMKL

#include <vector>
#include <algorithm>

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

} /* end namespace python */

} /* end namespace modmesh */

modmesh::SimpleArray<double> solve
(
    modmesh::SimpleArray<double> const & sarr
  , modmesh::SimpleArray<double> const & rhs
)
{
    // Populate the column major input by transposing
    modmesh::SimpleArray<double> mat(
        std::vector<size_t>{sarr.shape(1), sarr.shape(0)});
    for (size_t i = 0; i < mat.shape(0); ++i)
    {
        for (size_t j = 0; j < mat.shape(1); ++j)
        {
            mat(i, j) = sarr(j, i);
        }
    }

    modmesh::SimpleArray<double> b = rhs;
    int n = rhs.size();
    modmesh::SimpleArray<int> ipiv(n);

    int status;
    int nn = mat.shape(0);
    int bncol = 1;
    int bnrow = b.shape(0);
    int matnrow = mat.shape(1);

    // FIXME: This call is not yet validated. I am not sure about the
    // correctness of the solution.
    dgesv_( // column major.
        &nn // int * n: number of linear equation
      , &bncol // int * nrhs: number of RHS
      , mat.data() // double * a: array (lda, n)
      , &matnrow // int * lda: leading dimension of array a
      , ipiv.data() // int * ipiv: pivot indices
      , b.data() // double * b: array (ldb, nrhs)
      , &bnrow // int * ldb: leading dimension of array b
      , &status // for column major matrix, ldb remains the leading dimension.
    );

    return b;
}

// [begin example: single fit]
/**
 * This function calculates the least-square regression of a point cloud to a
 * polynomial function of a given order.
 */
modmesh::SimpleArray<double> fit_poly
(
    modmesh::SimpleArray<double> const & xarr
  , modmesh::SimpleArray<double> const & yarr
  , size_t start
  , size_t stop
  , size_t order
)
{
    if (xarr.size() != yarr.size())
    {
        throw std::runtime_error("xarr and yarr size mismatch");
    }

    // The rank of the linear map is (order+1).
    modmesh::SimpleArray<double> matrix(std::vector<size_t>{order+1, order+1});

    // Use the x coordinates to build the linear map for least-square
    // regression.
    for (size_t it=0; it<order+1; ++it)
    {
        for (size_t jt=0; jt<order+1; ++jt)
        {
            double & val = matrix(it, jt);
            val = 0;
            for (size_t kt=start; kt<stop; ++kt)
            {
                val += pow(xarr[kt], it+jt);
            }
        }
    }

    // Use the x and y coordinates to build the vector in the right-hand side
    // of the linear system.
    modmesh::SimpleArray<double> rhs(std::vector<size_t>{order+1});
    for (size_t jt=0; jt<order+1; ++jt)
    {
        rhs[jt] = 0;
        for (size_t kt=start; kt<stop; ++kt)
        {
            rhs[jt] += pow(xarr[kt], jt) * yarr[kt];
        }
    }

    // Solve the linear system for the least-square minimization.
    modmesh::SimpleArray<double> lhs = solve(matrix, rhs);
    std::reverse(lhs.begin(), lhs.end()); // to make numpy.poly1d happy.

    return lhs;
}
// [end example: single fit]

// [begin example: multiple fit]
/**
 * This function calculates the least-square regression of multiple sets of
 * point clouds to the corresponding polynomial functions of a given order.
 */
modmesh::SimpleArray<double> fit_polys
(
    modmesh::SimpleArray<double> const & xarr
  , modmesh::SimpleArray<double> const & yarr
  , size_t order
)
{
    size_t xmin = std::floor(*std::min_element(xarr.begin(), xarr.end()));
    size_t xmax = std::ceil(*std::max_element(xarr.begin(), xarr.end()));
    size_t ninterval = xmax - xmin;

    modmesh::SimpleArray<double> lhs(std::vector<size_t>{ninterval, order+1});
    std::fill(lhs.begin(), lhs.end(), 0); // sentinel.
    size_t start=0;
    for (size_t it=0; it<xmax; ++it)
    {
        // NOTE: We take advantage of the intrinsic features of the input data
        // to determine the grouping.  This is ad hoc and hard to maintain.  We
        // play this trick to demonstrate a hackish way of performing numerical
        // calculation.
        size_t stop;
        for (stop=start; stop<xarr.size(); ++stop)
        {
            if (xarr[stop]>=it+1) { break; }
        }

        // Use the single polynomial helper function.
        auto sub_lhs = fit_poly(xarr, yarr, start, stop, order);
        for (size_t jt=0; jt<order+1; ++jt)
        {
            lhs(it, jt) = sub_lhs[jt];
        }

        start = stop;
    }

    return lhs;
}
// [end example: multiple fit]

// [begin example: wrapping]
/**
 * The pybind11 wrapper for the helper functions for polynomial fitting.
 */
PYBIND11_MODULE(data_prep, m)
{
    // Boilerplate for using the SimpleArray with Python
    {
        modmesh::python::import_numpy();
        modmesh::python::wrap_ConcreteBuffer(m);
        modmesh::python::wrap_SimpleArray(m);
    }

    m.def
    (
        "fit_poly"
      , []
        (
            pybind11::array_t<double> & xarr_in
          , pybind11::array_t<double> & yarr_in
          , size_t order
        )
        {
            auto xarr = modmesh::python::makeSimpleArray(xarr_in);
            auto yarr = modmesh::python::makeSimpleArray(yarr_in);
            auto ret = fit_poly(xarr, yarr, 0, xarr.size(), order);
            return modmesh::python::to_ndarray(ret);
        }
    );
    m.def
    (
        "fit_polys"
      , []
        (
            pybind11::array_t<double> & xarr_in
          , pybind11::array_t<double> & yarr_in
          , size_t order
        )
        {
            auto xarr = modmesh::python::makeSimpleArray(xarr_in);
            auto yarr = modmesh::python::makeSimpleArray(yarr_in);
            auto ret = fit_polys(xarr, yarr, order);
            return modmesh::python::to_ndarray(ret);
        }
    );
}
// [end example: wrapping]

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
