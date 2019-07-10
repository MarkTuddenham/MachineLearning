#include <catch.hpp>

#include <matrix.hpp>
#include <neural_network.hpp>

// #include "utils.hpp"

using Teslyn::Matrix;
using Teslyn::NeuralNetwork;

TEST_CASE("Neural Network Constructor", "[nn]")
{
    std::vector<int> layers = {2, 4, 2};
    NeuralNetwork nn(layers);

    REQUIRE(nn.get_layers() == layers);
}

TEST_CASE("Neural Network Activations", "[nn][!mayfail]")
{
    double a = 0.2;
    unsigned int n_a_rows = 2, n_a_cols = 7;

    Matrix m1 = Matrix(n_a_rows, n_a_cols, a);
    Matrix m2 = NeuralNetwork::apply_activation(m1, NeuralNetwork::Sigmoid).set_name("Expected");

    REQUIRE(m1 == m2);
}
