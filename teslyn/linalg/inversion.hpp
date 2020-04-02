#pragma once

#include "teslyn/core/matrix.hpp"

namespace Teslyn::LinAlg
{
Matrix inverse(const Matrix& t_m);
Matrix pseudoinverse(const Matrix& t_m);
Matrix _gaussian_elimination(const Matrix& t_m);

} // namespace Teslyn::LinAlg