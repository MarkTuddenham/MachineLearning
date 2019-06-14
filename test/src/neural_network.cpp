#include <catch.hpp>

#include <utils.hpp>
#include <matrix.hpp>
#include <neural_network.hpp>

TEST_CASE("Neural Network Constructor", "[nn]")
{
    std::vector<int> layers = {2, 4, 2};
    NeuralNetwork<double> nn(layers);

    REQUIRE(nn.get_layers() == layers);
}

TEST_CASE("Neural Network Activations", "[nn][!mayfail]")
{
    double a = 0.2;
    unsigned int n_a_rows = 2, n_a_cols = 7;

    Matrix<double> m1 = Matrix<double>(n_a_rows, n_a_cols, a);
    Matrix<double> m2 = NeuralNetwork<double>::applyActivationFunction(m1, NeuralNetwork<double>::Sigmoid).setName("Expected");

    REQUIRE(m1 == m2);
}