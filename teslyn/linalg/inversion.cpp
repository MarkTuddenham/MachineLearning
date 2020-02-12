#include <string>

#include "teslyn/core/matrix.hpp"
#include "teslyn/linalg/inversion.hpp"

namespace Teslyn::LinAlg
{

// Left pseudoinverse
Matrix pseudoinverse(const Matrix &t_m)
{
    // Use the complex conjugate for complex numbers
    Matrix m_transpose = t_m.transpose();

    return _gaussian_elimination(m_transpose * t_m) * m_transpose;
}

Matrix inverse(const Matrix &t_m)
{
    return _gaussian_elimination(t_m);
}

Matrix _gaussian_elimination(const Matrix &t_m)
{

    //check dimensions
    if (t_m.get_rows() != t_m.get_cols())
    {
        throw std::invalid_argument("Matrix is not square: (" + std::to_string(t_m.get_rows()) + 'x' + std::to_string(t_m.get_cols()) + ')');
    }

    unsigned int size = t_m.get_rows();

    Matrix aug = Matrix::I(size);

    // Stage 1
    Matrix multiplier = t_m[0][0].reciprocal();
    auto m0 = multiplier * t_m[0];
    auto aug0 = multiplier * aug[0];

    // Stage 2
    auto m1 = t_m[1] - (t_m[1][0] * m0);
    auto aug1 = aug[1] - (t_m[1][0] * aug0);

    // Stage 3
    multiplier = m1[1].reciprocal();
    m1 = multiplier * m1;
    aug1 = multiplier * aug1;

    // Stage 4
    aug0 = aug0 - (m0[1] * aug1);
    m0 = m0 - (m0[1] * m1);

    auto aug0_f = aug0.get_flattened();
    auto aug1_f = aug1.get_flattened();

    auto m = std::vector<double>();
    m.reserve(aug0_f.size() + aug1_f.size());
    m.insert(end(m), begin(aug0_f), end(aug0_f));
    m.insert(end(m), begin(aug1_f), end(aug1_f));

    aug = Matrix::from_array(m);

    return aug.reshape(t_m.get_rows(), t_m.get_cols()).set_name(t_m.get_name() + "-1");
}

} // namespace Teslyn::LinAlg