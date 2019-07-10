#pragma once
#include <matrix.hpp>

using Teslyn::Matrix;

std::ostream &operator<<(std::ostream &os, Matrix const &m)
{
    m.print(1, &os);
    return os;
}