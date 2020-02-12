#include <functional>
#include <memory>
#include <numeric>
#include <optional>

#include "teslyn/core/tensor.hpp"

namespace Teslyn
{

Tensor::Tensor()
{
}

Tensor::Tensor(std::vector<size_t> t_shape, double t_fill) : m_shape(t_shape), m_offset(0)
{
    m_data = std::make_shared<std::vector<double>>(
        std::vector<double>(
            std::accumulate(t_shape.begin(), t_shape.end(), 1, std::multiplies<size_t>()),
            t_fill));

    size_t sum = 1;
    m_strides = std::vector<size_t>();
    // m_strides.reserve(t_shape.size()); // Don't think this is needed as vector is short.
    m_strides.push_back(sum);

    for (auto iter = t_shape.begin(); iter != t_shape.end() - 1; ++iter)
    {
        sum *= *iter;
        m_strides.push_back(sum);
    }
}

Tensor Tensor::operator[](std::initializer_list<std::optional<size_t>> t_ind) const
{

    // TODO how to add slicing, e.g. allow whole dimension x[{0, :}]

    if (t_ind.size() != m_shape.size())
    {
        // TODO throw error
    }

    // Data
    Tensor slice = Tensor();
    slice.m_data = m_data;
    slice.m_strides = std::vector<size_t>();
    slice.m_shape = std::vector<size_t>();
    slice.m_offset = 0;

    // Strides
    std::vector<size_t> is_opt;

    std::transform(
        t_ind.begin(),
        t_ind.end(),
        std::back_inserter(is_opt),
        [](const std::optional<size_t> &size) {
            return static_cast<size_t>(size.has_value());
        });

    // std::transform(
    //     cbegin(t_ind),
    //     cend(t_ind),
    //     std::back_inserter(ind_opt_zero),
    //     [](const std::optional<size_t> &size) {
    //         return size.value_or(0);
    //     });

    std::transform(
        is_opt.cbegin(),
        is_opt.cend(),
        m_strides.cbegin(),
        std::back_inserter(slice.m_strides),
        std::multiplies<size_t>());

    slice.m_strides.erase(
        std::remove(
            slice.m_strides.begin(),
            slice.m_strides.end(),
            0),
        slice.m_strides.end());

    // Shape
    std::transform(
        is_opt.cbegin(),
        is_opt.cend(),
        m_shape.cbegin(),
        std::back_inserter(slice.m_shape),
        std::multiplies<size_t>());

    slice.m_shape.erase(
        std::remove(
            slice.m_shape.begin(),
            slice.m_shape.end(),
            0),
        slice.m_shape.end());

    // Offset
    std::vector<size_t> offset_ind;

    std::transform(
        t_ind.begin(),
        t_ind.end(),
        std::back_inserter(offset_ind),
        [](const std::optional<size_t> &size) {
            return size.value_or(0);
        });

    std::vector<size_t> offsets;
    std::transform(
        offset_ind.cbegin(),
        offset_ind.cend(),
        m_strides.cbegin(),
        std::back_inserter(offsets),
        std::multiplies<size_t>());

    slice.m_offset = m_offset + std::accumulate(offsets.cbegin(), offsets.cend(), 0);

    return slice;
}

// Tensor Tensor::multiply(const Tensor &t_m) const
// {
//     return Tensor();
// }

// Tensor Tensor::operator*(const Tensor &t_m) const
// {
//     return Tensor();
// }

// Tensor Tensor::add(const Tensor &) const
// {
//     return Tensor();
// }

// Tensor Tensor::operator+(const Tensor &t_m) const
// {
//     return Tensor();
// }

std::vector<double> Tensor::flatten() const
{
    // TODO only the data you can get to from the striding
    return *m_data;
}

std::vector<size_t> Tensor::get_shape() const
{
    return m_shape;
}

void print_tensor(const Tensor &t, int t_precision, std::ostream *t_op)
{
    *t_op << "Shape { ";
    for (size_t d : t.get_shape())
        *t_op << d << " ";
    *t_op << "}\n";

    *t_op << "Strides { ";
    for (size_t d : t.m_strides)
        *t_op << d << " ";
    *t_op << "}\n";

    *t_op << "Offset: " << t.m_offset << "\n";

    *t_op << std::setprecision(t_precision) << std::fixed;

    for (double v : t.flatten())
    {

        // add extra space to account for minus sign
        if (v >= 0)
            *t_op << " ";

        *t_op << v << ", ";
    }

    *t_op << "\n\n";
}

} // namespace Teslyn