#include <numeric>

#include "teslyn/core/tensor.hpp"


namespace Teslyn
{

Tensor::Tensor()
{
}

Tensor::Tensor(std::vector<size_t> t_shape, double t_fill)
{
    m_data = std::vector<double>(std::accumulate(t_shape.begin(), t_shape.end(), 1, std::multiplies<int>()), t_fill);
    m_strides = std::vector<size_t>(t_shape.size());

    size_t sum = 1;
    m_strides.push_back(sum);

    for (auto iter = t_shape.begin(); iter != t_shape.end() - 1; ++iter){
        sum += *iter;
        m_strides.push_back(sum);
    }
}

Tensor Tensor::operator[](size_t t_i) const
{
    return Tensor();
}

Tensor Tensor::multiply(const Tensor& t_m) const
{
    return Tensor();
}

Tensor Tensor::operator*(const Tensor& t_m) const
{
    return Tensor();
}

Tensor Tensor::add(const Tensor&) const
{
    return Tensor();
}

Tensor Tensor::operator+(const Tensor& t_m) const
{
    return Tensor();
}

std::vector<double> Tensor::flatten() const
{
    return m_data;
}

}