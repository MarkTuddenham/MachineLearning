#include <iostream>
#include <cstdlib>

#include "matrix.hpp"
#include "linalg/inversion.hpp"
#include "utils/printer.hpp"

using Teslyn::Matrix;
using Teslyn::NeuralNetwork;

int main()
{
    std::cout << "Linear Algebra\n";

    Matrix m = Matrix::from_array({2, 1, 3, 4}).reshape(2, 2);
    Matrix m_inv = Teslyn::LinAlg::inverse(m);

    Teslyn::display::print_matrix(m);
    Teslyn::display::print_matrix(m_inv);
    Teslyn::display::print_matrix(m * m_inv);
};