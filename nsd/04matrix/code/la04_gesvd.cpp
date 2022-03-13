#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <algorithm>

#ifdef HASMKL
#include <mkl_lapack.h>
#include <mkl_lapacke.h>
#else // HASMKL
#ifdef __MACH__
#include <clapack.h>
#include <Accelerate/Accelerate.h>
#endif // __MACH__
#endif // HASMKL

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol, bool column_major)
      : m_nrow(nrow), m_ncol(ncol), m_column_major(column_major)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix
    (
        size_t nrow, size_t ncol, bool column_major
      , std::vector<double> const & vec
    )
      : m_nrow(nrow), m_ncol(ncol), m_column_major(column_major)
    {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix & operator=(std::vector<double> const & vec)
    {
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
      , m_column_major(other.m_column_major)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix & operator=(Matrix const & other)
    {
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
        return *this;
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
      , m_column_major(other.m_column_major)
    {
        reset_buffer(0, 0);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        m_column_major = other.m_column_major;
        return *this;
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const
    {
        return std::vector<double>(m_buffer, m_buffer+size());
    }

    double * data() const { return m_buffer; }

private:

    size_t index(size_t row, size_t col) const
    {
        if (m_column_major) { return row          + col * m_nrow; }
        else                { return row * m_ncol + col         ; }
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = new double[nelement]; }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    bool m_column_major = false;
    double * m_buffer = nullptr;

};

std::ostream & operator << (std::ostream & ostr, Matrix const & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        ostr << std::endl << " ";
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            ostr << " " << std::setw(10) << mat(i, j);
        }
    }

    ostr << std::endl << " data: ";
    for (size_t i=0; i<mat.size(); ++i)
    {
        ostr << " " << mat.data()[i];
    }

    return ostr;
}

std::ostream & operator << (std::ostream & ostr, std::vector<double> const & vec)
{
    for (size_t i=0; i<vec.size(); ++i)
    {
        std::cout << " " << vec[i];
    }

    return ostr;
}

/*
 * See references:
 * * https://software.intel.com/en-us/mkl-developer-reference-c-gesvd
 * * https://software.intel.com/sites/products/documentation/doclib/mkl_sa/11/mkl_lapack_examples/lapacke_dgesvd_row.c.htm
 */
int main(int argc, char ** argv)
{
    const size_t m = 3, n = 4;
    int status;

    std::cout << ">>> SVD" << std::endl;
    Matrix mat(m, n, /* column_major */ true);
    mat(0,0) = 3; mat(0,1) = 5; mat(0,2) = 2; mat(0, 3) = 6;
    mat(1,0) = 2; mat(1,1) = 1; mat(1,2) = 3; mat(1, 3) = 2;
    mat(2,0) = 4; mat(2,1) = 3; mat(2,2) = 2; mat(2, 3) = 4;
    std::vector<double> s(m), superb(m);
    Matrix u(m, m, /* column_major */ true);
    Matrix vt(n, n, /* column_major */ true);

    std::cout << "A:" << mat << std::endl;

#if !defined(HASMKL) || defined(NOMKL)
    {
        char jobu = 'A';
        char jobv = 'A';
        int mm = m;
        int nn = n;
        int matnrow = mat.nrow();
        int lwork = 5 * std::max(m, n);
        std::vector<double> work(lwork);

        dgesvd_( // column major.
            &jobu
          , &jobv
          , &mm // int *: m
          , &nn // int *: n
          , mat.data() // double *: a
          , &matnrow // int *: lda
          , s.data() // double *: s
          , u.data() // double *: u
          , &mm
          , vt.data() // double *: vt
          , &nn // int *: ldvt
          , work.data() // double *: work
          , &lwork // int *: lwork
          , &status
          // for column major matrix, ldb remains the leading dimension.
        );
    }
#else // HASMKL NOMKL
    status = LAPACKE_dgesvd(
        LAPACK_COL_MAJOR // int matrix_layout;
      , 'A' // char jobu;
      , 'A' // char jobvt;
      , m // lapack_int m
      , n // lapack_int n
      , mat.data() // double * a
      , mat.nrow() // lapack_int lda
      , s.data() // double * s
      , u.data() // double * u
      , u.nrow() // lapack_int ldu
      , vt.data() // double * vt
      , vt.nrow() // lapack_int ldvt
      , superb.data() // double * superb
    );
#endif // HASMKL NOMKL

    std::cout << "dgesvd status: " << status << std::endl;
    std::cout << "singular values: " << s << std::endl;
    std::cout << "u: " << u << std::endl;
    std::cout << "vt: " << vt << std::endl;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
