#pragma once

#include <vector>

namespace Teslyn
{

class Tensor
{

public:
    //Tensor() = delete;
    Tensor();
    Tensor(std::vector<size_t> t_shape, double t_fill = 0);

    Tensor operator[](size_t t_i) const;

    Tensor multiply(const Tensor& t_m) const;
    Tensor operator*(const Tensor& t_m) const;

    Tensor add(const Tensor&) const;
    Tensor operator+(const Tensor& t_m) const;

    std::vector<double> flatten() const;


private:
    std::vector<size_t> m_shape;
    std::vector<size_t> m_strides;
    std::vector<double> m_data;
};

} // namespace Teslyn