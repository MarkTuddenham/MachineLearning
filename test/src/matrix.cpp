#include <catch.hpp>

#include "teslyn/core/matrix.hpp"

using Teslyn::Matrix;

TEST_CASE("Matrix Constructor", "[matrix]")
{
    size_t n_rows = 5, n_cols = 3;
    std::string name = "M";

    Matrix m = Matrix(n_rows, n_cols, 1, name);
    REQUIRE(m.get_rows() == n_rows);
    REQUIRE(m.get_cols() == n_cols);
    REQUIRE(m.get_name() == name);
}

TEST_CASE("Matrix Zeros", "[matrix]")
{
    size_t n_rows = 5, n_cols = 3;

    Matrix m_a = Matrix(n_rows, n_cols, 0);
    Matrix m_b = Matrix::zeros(n_rows, n_cols);

    REQUIRE(m_a == m_b);
}

TEST_CASE("Matrix Ones", "[matrix]")
{
    size_t n_rows = 5, n_cols = 3;

    Matrix m_a = Matrix(n_rows, n_cols, 1);
    Matrix m_b = Matrix::ones(n_rows, n_cols);

    REQUIRE(m_a == m_b);
}

TEST_CASE("Matrix Reshape", "[matrix]")
{
    size_t n_a_rows = 6, n_a_cols = 4;
    size_t n_b_rows = 8, n_b_cols = 3;

    Matrix m_a = Matrix(n_a_rows, n_a_cols, 1);
    Matrix m_b = Matrix(n_b_rows, n_b_cols, 1);

    REQUIRE(m_a.reshape(n_b_rows, n_b_cols) == m_b);

    SECTION(" throws with inccorect dimensions")
    {
        REQUIRE_THROWS(m_a.reshape(n_b_rows + 1, n_b_cols));
    }
}

TEST_CASE("Matrix from_array", "[matrix]")
{
    SECTION(" for 1D")
    {
        size_t n = 5;
        double val = 3;
        Matrix m_a = Matrix(1, n, val);
        std::vector<double> vec(n, val);
        Matrix m_b = Matrix::from_array(vec);

        REQUIRE(m_a == m_b);
    }

    SECTION(" using reshape")
    {
        size_t n_rows = 3, n_cols = 5;
        double val = 3;
        Matrix m_a = Matrix(n_rows, n_cols, val);
        std::vector<double> vec(n_rows * n_cols, val);
        Matrix m_b = Matrix::from_array(vec).reshape(n_rows, n_cols);

        REQUIRE(m_a == m_b);
    }
}

TEST_CASE("Matrix Transpose", "[matrix]")
{
    Matrix m_a = Matrix::from_array({1, 2, 3, 4});
    Matrix m_b = Matrix::from_array({1, 2, 3, 4}).reshape(m_a.get_cols(), m_a.get_rows());

    REQUIRE(m_a.transpose() == m_b);
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
    Matrix m_a = Matrix(2, 2);
    Matrix m_b = Matrix(2, 2);
    REQUIRE(m_a == m_b);
}

TEST_CASE("Matrix Randomise", "[matrix]")
{
    Matrix m_a = Matrix(2, 2);
    Matrix m_b = m_a; //TODO check this is copy constructor
    REQUIRE_FALSE(m_a.randomise() == m_b);
}

TEST_CASE("Matrix Addition", "[matrix]")
{
    double a = 4.2, b = 17.73;
    size_t n_rows = 5, n_cols = 3;

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

    SECTION(" by scalar matrix")
    {
        Matrix m_one = Matrix(1, 1, 1);
        Matrix m_c = Matrix(n_rows, n_cols, a + 1, "Expected");
        REQUIRE(m_a + m_one == m_c);
    }

    SECTION(" throws with inccorect dimensions")
    {
        m_b = Matrix(n_rows + 1, n_cols, 1);
        REQUIRE_THROWS(m_a + m_b);
    }
}

TEST_CASE("Matrix Subtraction", "[matrix]")
{
    double a = 4.2, b = 17.73;
    size_t n_rows = 5, n_cols = 3;

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
    size_t n_a_rows = 2, n_a_cols = 7;

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

    SECTION(" throws with incompatible dimensions")
    {
        Matrix m_c = Matrix(n_a_cols + 1, 1, 0, "Throw Expected");
        REQUIRE_THROWS(m_a * m_c);
    }
}

TEST_CASE("Matrix Hadamard", "[matrix]")
{
    double a = 4.2, b = 17.73;
    size_t n_rows = 5, n_cols = 3;

    Matrix m_a = Matrix(n_rows, n_cols, a);
    Matrix m_b = Matrix(n_rows, n_cols, b);

    Matrix m_c = Matrix(n_rows, n_cols, a * b, "Expected");
    REQUIRE(m_a.hadamard(m_b) == m_c);
}