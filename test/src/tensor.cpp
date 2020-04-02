#include "teslyn/core/tensor.hpp"

#include <catch.hpp>
#include <functional>
#include <numeric>

using Teslyn::Tensor;

TEST_CASE("Tensor Construction", "[Tensor]")
{
    size_t n_rows = 5, n_cols = 3;
    std::vector<size_t> shape = {n_rows, n_cols};

    SECTION("Constructor")
    {
        Tensor t = Tensor(shape, 1);
        REQUIRE(t.get_shape().at(0) == n_rows);
        REQUIRE(t.get_shape().at(1) == n_cols);
    }

    SECTION("Tensor Zeros")
    {
        Tensor t_a = Tensor(shape, 0);
        Tensor t_b = Tensor::zeros(shape);

        REQUIRE(t_a == t_b);
    }

    SECTION("Tensor Ones")
    {
        Tensor t_a = Tensor(shape, 1);
        Tensor t_b = Tensor::ones(shape);

        REQUIRE(t_a == t_b);
    }
}
TEST_CASE("Tensor Reshape", "[Tensor]")
{
    std::vector<size_t> a_shape = {6, 4};
    std::vector<size_t> b_shape = {8, 3};

    Tensor t_a = Tensor::ones(a_shape);
    Tensor t_b = Tensor::ones(b_shape);

    REQUIRE(t_a.reshape(b_shape) == t_b);

    SECTION(" throws with inccorect dimensions")
    {
        b_shape.at(1)++;
        REQUIRE_THROWS(t_a.reshape(b_shape));
    }
}

TEST_CASE("Tensor from_array", "[Tensor]")
{
    SECTION(" for 1D")
    {
        size_t n = 5;
        double val = 3;
        Tensor t_a = Tensor({n}, val);
        std::vector<double> vec(n, val);
        Tensor t_b = Tensor::from(vec);

        REQUIRE(t_a == t_b);
    }

    SECTION(" using reshape")
    {
        std::vector<size_t> shape = {3, 5};
        double val = 3;
        Tensor t_a = Tensor(shape, val);

        std::vector<double> vec(
            std::accumulate(
                cbegin(shape),
                cend(shape),
                static_cast<size_t>(1),
                std::multiplies<size_t>()),
            val);

        Tensor t_b = Tensor::from(vec).reshape(shape);

        REQUIRE(t_a == t_b);
    }
}

// TEST_CASE("Tensor Transpose", "[Tensor]")
// {
//     Tensor t_a = Tensor::from_array({1, 2, 3, 4});
//     Tensor t_b = Tensor::from_array({1, 2, 3, 4}).reshape(t_a.get_cols(), t_a.get_rows());

//     REQUIRE(t_a.t() == t_b);
// }

// TEST_CASE("Tensor Apply", "[Tensor][!mayfail]")
// {
//     //    TODO
// }

// TEST_CASE("Tensor Flatten", "[Tensor][!mayfail]")
// {
//     //    TODO
// }

// TEST_CASE("Tensor Randomise", "[Tensor]")
// {
//     Tensor t_a = Tensor(2, 2);
//     Tensor t_b = t_a; //TODO check this is copy constructor
//     REQUIRE_FALSE(t_a.randomise() == t_b);
// }

// TEST_CASE("Tensor Addition", "[Tensor]")
// {
//     double a = 4.2, b = 17.73;
//     size_t n_rows = 5, n_cols = 3;

//     Tensor t_a = Tensor(n_rows, n_cols, a);
//     Tensor t_b = Tensor(n_rows, n_cols, b);

//     // SECTION(" by scalar")
//     // {
//     //     Tensor m_s = Tensor(1, 1, b);
//     //     REQUIRE(t_a + t_b == m_c);
//     // }

//     SECTION(" by Tensor")
//     {
//         Tensor m_c = Tensor(n_rows, n_cols, a + b, "Expected");
//         REQUIRE(t_a + t_b == m_c);
//     }

//     SECTION(" by scalar Tensor")
//     {
//         Tensor m_one = Tensor(1, 1, 1);
//         Tensor m_c = Tensor(n_rows, n_cols, a + 1, "Expected");
//         REQUIRE(t_a + m_one == m_c);
//     }

//     SECTION(" throws with inccorect dimensions")
//     {
//         t_b = Tensor(n_rows + 1, n_cols, 1);
//         REQUIRE_THROWS(t_a + t_b);
//     }
// }

// TEST_CASE("Tensor Subtraction", "[Tensor]")
// {
//     double a = 4.2, b = 17.73;
//     size_t n_rows = 5, n_cols = 3;

//     Tensor t_a = Tensor(n_rows, n_cols, a);
//     Tensor t_b = Tensor(n_rows, n_cols, b);

//     // SECTION(" by scalar")
//     // {
//     //     Tensor m_s = Tensor(1, 1, b);
//     //     REQUIRE(t_a - t_b == m_c);
//     // }

//     SECTION(" by Tensor")
//     {
//         Tensor m_c = Tensor(n_rows, n_cols, a - b, "Expected");
//         REQUIRE(t_a - t_b == m_c);
//     }
// }

// TEST_CASE("Tensor Multiplication", "[Tensor]")
// {
//     double a = 4.2, b = 17.73;
//     size_t n_a_rows = 2, n_a_cols = 7;

//     Tensor t_a = Tensor(n_a_rows, n_a_cols, a);
//     Tensor t_b = Tensor(n_a_cols, n_a_rows, b);

//     // SECTION(" by scalar")
//     // {
//     //     Tensor m_s = Tensor(1, 1, b);
//     //     REQUIRE(t_a * t_b == m_c);
//     // }

//     SECTION(" by Tensor")
//     {
//         Tensor m_c = Tensor(n_a_rows, n_a_rows, a * b * n_a_cols, "Expected");
//         REQUIRE(t_a * t_b == m_c);
//     }

//     SECTION(" throws with incompatible dimensions")
//     {
//         Tensor m_c = Tensor(n_a_cols + 1, 1, 0, "Throw Expected");
//         REQUIRE_THROWS(t_a * m_c);
//     }
// }

// TEST_CASE("Tensor Hadamard", "[Tensor]")
// {
//     double a = 4.2, b = 17.73;
//     size_t n_rows = 5, n_cols = 3;

//     Tensor t_a = Tensor(n_rows, n_cols, a);
//     Tensor t_b = Tensor(n_rows, n_cols, b);

//     Tensor m_c = Tensor(n_rows, n_cols, a * b, "Expected");
//     REQUIRE(t_a.hadamard(t_b) == m_c);
// }