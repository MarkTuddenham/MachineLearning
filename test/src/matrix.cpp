#include <catch.hpp>
#include <sstream>

#include <matrix.hpp>

namespace Catch
{

template <typename T>
std::string toString(Matrix<T> const &m)
{
    std::stringstream ss;
    m.print(3, ss);
    return ss.str();
}
} // namespace Catch

TEST_CASE("Matrix Equality", "[matrix]")
{
    Matrix<double> m1 = Matrix<double>(2, 2);
    Matrix<double> m2 = Matrix<double>(2, 2);
    REQUIRE(m1 == m2);
}

TEST_CASE("Matrix Addition", "[matrix]")
{
    double a = 4.2, b = 17.73;
    uint n_rows = 5, n_cols = 3;

    Matrix<double> m_a = Matrix<double>(n_rows, n_cols, a);
    Matrix<double> m_b = Matrix<double>(n_rows, n_cols, b);

    // SECTION(" by scalar")
    // {
    //     Matrix<double> m_s = Matrix<double>(1, 1, b);
    //     REQUIRE(m_a + m_b == m_c);
    // }

    SECTION(" by matrix")
    {
        Matrix<double> m_c = Matrix<double>(n_rows, n_cols, a + b);
        REQUIRE(m_a + m_b == m_c);
    }
}

TEST_CASE("Matrix Subtraction", "[matrix]")
{
    double a = 4.2, b = 17.73;
    uint n_rows = 5, n_cols = 3;

    Matrix<double> m_a = Matrix<double>(n_rows, n_cols, a);
    Matrix<double> m_b = Matrix<double>(n_rows, n_cols, b);

    // SECTION(" by scalar")
    // {
    //     Matrix<double> m_s = Matrix<double>(1, 1, b);
    //     REQUIRE(m_a - m_b == m_c);
    // }

    SECTION(" by matrix")
    {
        Matrix<double> m_c = Matrix<double>(n_rows, n_cols, a - b);
        REQUIRE(m_a - m_b == m_c);
    }
}

TEST_CASE("Matrix Multiplication", "[matrix]")
{
    double a = 4.2, b = 17.73;
    uint n_a_rows = 5, n_a_cols = 3;

    Matrix<double> m_a = Matrix<double>(n_a_rows, n_a_cols, a);
    Matrix<double> m_b = Matrix<double>(n_a_cols, n_a_rows, b);

    // SECTION(" by scalar")
    // {
    //     Matrix<double> m_s = Matrix<double>(1, 1, b);
    //     REQUIRE(m_a * m_b == m_c);
    // }

    SECTION(" by matrix")
    {
        Matrix<double> m_c = Matrix<double>::fromArray({});
        REQUIRE(m_a * m_b == m_c);
    }
}