#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

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
            ostr << " " << std::setw(2) << mat(i, j);
        }
    }

    ostr << std::endl << " data: ";
    for (size_t i=0; i<mat.size(); ++i)
    {
        ostr << " " << std::setw(2) << mat.data()[i];
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

int main(int argc, char ** argv)
{
    const size_t n = 3;
    int status;

    std::cout << ">>> Solve Ax=b (row major)" << std::endl;
    Matrix mat(n, n, /* column_major */ false);
    mat(0,0) = 3; mat(0,1) = 5; mat(0,2) = 2;
    mat(1,0) = 2; mat(1,1) = 1; mat(1,2) = 3;
    mat(2,0) = 4; mat(2,1) = 3; mat(2,2) = 2;
    Matrix b(n, 1, false);
    b(0,0) = 57;
    b(1,0) = 22;
    b(2,0) = 41;
    std::vector<int> ipiv(n);

    std::cout << "A:" << mat << std::endl;
    std::cout << "b:" << b << std::endl;

#if !defined(HASMKL) || defined(NOMKL)
    {
        int nn = n;
        int bncol = b.ncol();
        int bnrow = b.nrow();
        int matnrow = mat.nrow();

        dgesv_( // column major.
            &nn // int * n: number of linear equation
          , &bncol // int * nrhs: number of RHS
          , mat.data() // double * a: array (lda, n)
          , &matnrow // int * lda: leading dimension of array a
          , ipiv.data() // int * ipiv: pivot indices
          , b.data() // double * b: array (ldb, nrhs)
          , &bnrow // int * ldb: leading dimension of array b
          , &status
          // for column major matrix, ldb remains the leading dimension.
        );
    }
#else // HASMKL NOMKL
    /*
     * "Note that using row-major ordering may require more memory and time
     * than column-major ordering, because the routine must transpose the
     * row-major order to the column-major order required by the underlying
     * LAPACK routine."  See:
     *
     * - https://www.netlib.org/lapack/lapacke.html#_array_arguments
     * - https://github.com/Reference-LAPACK/lapack/blob/2a39774316821436989cb755a59255cfa1ae9d99/LAPACKE/src/lapacke_dgesv_work.c#L63
     */
    status = LAPACKE_dgesv(
        LAPACK_ROW_MAJOR // int matrix_layout
      , n // lapack_int n
      , b.ncol() // lapack_int nrhs
      , mat.data() // double * a
      , mat.ncol() // lapack_int lda
      , ipiv.data() // lapack_int * ipiv
      , b.data() // double * b
      , b.ncol() // lapack_int ldb
      // for row major matrix, ldb becomes the trailing dimension.
    );
#endif // HASMKL NOMKL

    std::cout << "solution x:" << b << std::endl;
    std::cout << "dgesv status: " << status << std::endl;

    std::cout << ">>> Solve Ax=b (column major)" << std::endl;
    Matrix mat2 = Matrix(n, n, /* column_major */ true);
    mat2(0,0) = 3; mat2(0,1) = 5; mat2(0,2) = 2;
    mat2(1,0) = 2; mat2(1,1) = 1; mat2(1,2) = 3;
    mat2(2,0) = 4; mat2(2,1) = 3; mat2(2,2) = 2;
    Matrix b2(n, 1, true);
    b2(0,0) = 57;
    b2(1,0) = 22;
    b2(2,0) = 41;

    std::cout << "A:" << mat2 << std::endl;
    std::cout << "b:" << b2 << std::endl;

#if !defined(HASMKL) || defined(NOMKL)
    {
        int nn = n;
        int b2ncol = b2.ncol();
        int b2nrow = b2.nrow();
        int mat2nrow = mat2.nrow();

        dgesv_( // column major.
            &nn // int * n: number of linear equation
          , &b2ncol // int * nrhs: number of RHS
          , mat2.data() // double * a: array (lda, n)
          , &mat2nrow // int * lda: leading dimension of array a
          , ipiv.data() // int * ipiv: pivot indices
          , b2.data() // double * b: array (ldb, nrhs)
          , &b2nrow // int * ldb: leading dimension of array b
          , &status
          // for column major matrix, ldb remains the leading dimension.
        );
    }
#else // HASMKL NOMKL
    status = LAPACKE_dgesv(
        LAPACK_COL_MAJOR // int matrix_layout
      , n // lapack_int n
      , b2.ncol() // lapack_int nrhs
      , mat2.data() // double * a
      , mat2.nrow() // lapack_int lda
      , ipiv.data() // lapack_int * ipiv
      , b2.data() // double * b
      , b2.nrow() // lapack_int ldb
      // for column major matrix, ldb remains the leading dimension.
    );
#endif // HASMKL NOMKL

    std::cout << "solution x:" << b2 << std::endl;
    std::cout << "dgesv status: " << status << std::endl;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
