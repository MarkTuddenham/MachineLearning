#pragma once

#include "matrix.hpp"
#include "utils/printer.hpp"

std::ostream &operator<<(std::ostream &os, Teslyn::Matrix const &m)
{
    Teslyn::display::print_matrix(m, 1, &os);
    return os;
}