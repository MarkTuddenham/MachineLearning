#include "teslyn/core/matrix.hpp"
#include "teslyn/core/neural_network.hpp"
#include "teslyn/utils/printer.hpp"

#include <catch.hpp>
#include <sstream>

using Teslyn::Matrix;
using Teslyn::NeuralNetwork;

TEST_CASE("Matrix printer", "[display]")
{
    unsigned int n_rows = 5, n_cols = 3;
    double val = 3;
    std::string name = "M";

    int precision = 1;

    Matrix m = Matrix(n_rows, n_cols, val, name);

    std::ostringstream oss;
    Teslyn::display::print_matrix(m, precision, &oss);

    std::ostringstream expected;
    expected << name << ": 5x3 "
             << "Matrix:\n"
             << " |  3.0  |  3.0  |  3.0  |\n"
             << " |  3.0  |  3.0  |  3.0  |\n"
             << " |  3.0  |  3.0  |  3.0  |\n"
             << " |  3.0  |  3.0  |  3.0  |\n"
             << " |  3.0  |  3.0  |  3.0  |\n";

    REQUIRE(oss.str() == expected.str());
}

TEST_CASE("Neural Network printer", "[display]")
{
    std::vector<int> layers = {2, 3, 2};
    NeuralNetwork nn = NeuralNetwork(layers);

    std::ostringstream oss;
    Teslyn::display::print_nn(nn, &oss);

    std::ostringstream expected;
    expected << "~~~~~~~~~~ # Neural Network # ~~~~~~~~~~\n"
             << "Layers:\n"
             << "   - Nodes: 2\n"
             << "   - Nodes: 3\n"
             << "   - Nodes: 2\n"
             << "~~~~~~~~~~ # ~~~~~~~~~~~~~~ # ~~~~~~~~~~\n";

    REQUIRE(oss.str() == expected.str());
}