#include <iostream>
#include <cstdlib>

#include "teslyn/core/matrix.hpp"
#include "teslyn/utils/printer.hpp"

using Teslyn::Matrix;

using std::cout;
using std::endl;

int main()
{
    std::cout << "Matrices:\n";

    Matrix m1(3, 2, 1.4, "M1");
    m1.randomise();
    Teslyn::display::print_matrix(m1);
    Teslyn::display::print_matrix(m1.transpose().set_name("M1'"));

    Matrix m2 = Matrix::from_array({1, 2, 3, 4, 5, 6}).reshape(3, 2).set_name("M2");
    Teslyn::display::print_matrix(m2);

    Matrix m3 = Matrix::from_array({1, 2, 3}).set_name("M3");
    Matrix m4 = Matrix::from_array({1, 2, 3}).transpose().set_name("M4");
    Teslyn::display::print_matrix(m3);
    Teslyn::display::print_matrix(m4);

    Teslyn::display::print_matrix((m3 * m4).set_name("M3xM4"));

    Teslyn::display::print_matrix((m1 + m2).set_name("M1+M2"));
    Teslyn::display::print_matrix((m1 * m2.transpose()).set_name("M1xM2'"));
}