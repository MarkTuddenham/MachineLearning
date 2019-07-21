#include <catch.hpp>
#include <cmath>

#include "matrix.hpp"
#include "neural_network.hpp"

// #include "utils.hpp"

using Teslyn::Matrix;
using Teslyn::NeuralNetwork;

TEST_CASE("Neural Network Constructor", "[nn]")
{
    std::vector<int> layers = {2, 4, 2};
    NeuralNetwork nn(layers);

    REQUIRE(nn.get_layers() == layers);
}

TEST_CASE("Neural Network Activations", "[nn]")
{
    double val = 0.2;
    unsigned int n_rows = 2, n_cols = 7;

    Matrix m_a = Matrix(n_rows, n_cols, val);

    SECTION("Sigmoid")
    {
        std::vector<double> vec_sig_a(n_rows * n_cols, 1 / (1 + std::exp(-val)));

        Matrix m_b = NeuralNetwork::apply_activation(m_a, NeuralNetwork::Sigmoid);
        Matrix m_c = Matrix::from_array(vec_sig_a).reshape(n_rows, n_cols);
        REQUIRE(m_b == m_c);
    }

    SECTION("FastSigmoid")
    {
        std::vector<double> vec_fsig_a(n_rows * n_cols, val / (1 + std::abs(val)));

        Matrix m_b = NeuralNetwork::apply_activation(m_a, NeuralNetwork::FastSigmoid);
        Matrix m_c = Matrix::from_array(vec_fsig_a).reshape(n_rows, n_cols);
        REQUIRE(m_b == m_c);
    }

    SECTION("TanH")
    {
        std::vector<double> vec_tanh_a(n_rows * n_cols, std::tanh(val));

        Matrix m_b = NeuralNetwork::apply_activation(m_a, NeuralNetwork::Tanh);
        Matrix m_c = Matrix::from_array(vec_tanh_a).reshape(n_rows, n_cols);
        REQUIRE(m_b == m_c);
    }

    SECTION("Unknown")
    {
        REQUIRE_THROWS(NeuralNetwork::apply_activation(
            m_a,
            static_cast<Teslyn::NeuralNetwork::Activation>(-1)));
    }
}

TEST_CASE("Neural Network Feedforward", "[nn][!mayfail]")
{

    std::vector<int> layers = {2, 2};
    NeuralNetwork nn(layers);

    Matrix inp1 = Matrix::from_array({1.0, 1.0});
    Matrix out = Matrix::from_array({0, 0});
    REQUIRE(nn.feedforward(inp1) == out);
}

// TEST_CASE("Neural Network Backpropagation", "[nn][!mayfail]")
// {
// }
