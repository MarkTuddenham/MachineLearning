#include <catch.hpp>
#include <sstream>

#include <matrix.hpp>

#include "utils.hpp"

using Teslyn::Matrix;

TEST_CASE("Matrix Constructor", "[matrix]")
{
    unsigned int n_rows = 5, n_cols = 3;

    Matrix m = Matrix(n_rows, n_cols);
    REQUIRE(m.get_rows() == n_rows);
    REQUIRE(m.get_cols() == n_cols);
}

TEST_CASE("Matrix Zeros", "[matrix]")
{
    unsigned int n_rows = 5, n_cols = 3;

    Matrix m1 = Matrix(n_rows, n_cols, 0);
    Matrix m2 = Matrix::zeros(n_rows, n_cols);

    REQUIRE(m1 == m2);
}

TEST_CASE("Matrix Ones", "[matrix]")
{
    unsigned int n_rows = 5, n_cols = 3;

    Matrix m1 = Matrix(n_rows, n_cols, 1);
    Matrix m2 = Matrix::ones(n_rows, n_cols);

    REQUIRE(m1 == m2);
}

TEST_CASE("Matrix Transpose", "[matrix][!mayfail]")
{
    unsigned int n_rows = 5, n_cols = 3;
    Matrix m1 = Matrix(n_rows, n_cols).randomise();
    Matrix m2 = Matrix(n_cols, n_rows);

    REQUIRE(m1.transpose() == m2);
}

// TEST_CASE("Matrix Apply", "[matrix][!mayfail]")
// {
//     //    TODO
// }

// TEST_CASE("Matrix Flatten", "[matrix][!mayfail]")
// {
//     //    TODO
// }

TEST_CASE("Matrix Equality", "[matrix]")
{
    Matrix m1 = Matrix(2, 2);
    Matrix m2 = Matrix(2, 2);
    REQUIRE(m1 == m2);
}

TEST_CASE("Matrix Randomise", "[matrix]")
{
    Matrix m1 = Matrix(2, 2);
    Matrix m2 = m1;
    REQUIRE_FALSE(m1.randomise() == m2);
}

TEST_CASE("Matrix Addition", "[matrix]")
{
    double a = 4.2, b = 17.73;
    unsigned int n_rows = 5, n_cols = 3;

    Matrix m_a = Matrix(n_rows, n_cols, a);
    Matrix m_b = Matrix(n_rows, n_cols, b);

    // SECTION(" by scalar")
    // {
    //     Matrix m_s = Matrix(1, 1, b);
    //     REQUIRE(m_a + m_b == m_c);
    // }

    SECTION(" by matrix")
    {
        Matrix m_c = Matrix(n_rows, n_cols, a + b, "Expected");
        REQUIRE(m_a + m_b == m_c);
    }
}

TEST_CASE("Matrix Subtraction", "[matrix]")
{
    double a = 4.2, b = 17.73;
    unsigned int n_rows = 5, n_cols = 3;

    Matrix m_a = Matrix(n_rows, n_cols, a);
    Matrix m_b = Matrix(n_rows, n_cols, b);

    // SECTION(" by scalar")
    // {
    //     Matrix m_s = Matrix(1, 1, b);
    //     REQUIRE(m_a - m_b == m_c);
    // }

    SECTION(" by matrix")
    {
        Matrix m_c = Matrix(n_rows, n_cols, a - b, "Expected");
        REQUIRE(m_a - m_b == m_c);
    }
}

TEST_CASE("Matrix Multiplication", "[matrix]")
{
    double a = 4.2, b = 17.73;
    unsigned int n_a_rows = 2, n_a_cols = 7;

    Matrix m_a = Matrix(n_a_rows, n_a_cols, a);
    Matrix m_b = Matrix(n_a_cols, n_a_rows, b);

    // SECTION(" by scalar")
    // {
    //     Matrix m_s = Matrix(1, 1, b);
    //     REQUIRE(m_a * m_b == m_c);
    // }

    SECTION(" by matrix")
    {
        Matrix m_c = Matrix(n_a_rows, n_a_rows, a * b * n_a_cols, "Expected");
        REQUIRE(m_a * m_b == m_c);
    }
}

TEST_CASE("Matrix Hadamard", "[matrix]")
{
    double a = 4.2, b = 17.73;
    unsigned int n_rows = 5, n_cols = 3;

    Matrix m_a = Matrix(n_rows, n_cols, a);
    Matrix m_b = Matrix(n_rows, n_cols, b);

    Matrix m_c = Matrix(n_rows, n_cols, a * b, "Expected");
    REQUIRE(m_a.hadamard(m_b) == m_c);
}