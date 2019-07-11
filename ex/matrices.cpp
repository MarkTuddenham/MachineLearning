#include <iostream>
#include <cstdlib>

#include "matrix.hpp"
#include "neural_network.hpp"
#include "utils/printer.hpp"

using Teslyn::Matrix;
using Teslyn::NeuralNetwork;

using std::cout;
using std::endl;

int main()
{
    std::cout << "Matrices:\n";

    Matrix m1(2, 2, 1.4, "M1");
    Matrix m2(2, 2);
    m1.randomise();
    m2.randomise();
    Teslyn::display::print_matrix(m1);
    Teslyn::display::print_matrix(m2);

    Teslyn::display::print_matrix(m1.add(m2).set_name("M1+M2"));
    Teslyn::display::print_matrix(m1.multiply(m2).set_name("M1xM2"));
}