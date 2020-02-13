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

Tensor::Tensor(std::initializer_list<size_t> t_shape, dtype t_fill) : m_offset(0)
{
    m_data = std::make_shared<std::vector<dtype>>(
        std::vector<dtype>(
            std::accumulate(
                t_shape.begin(),
                t_shape.end(),
                1,
                std::multiplies<size_t>()),
            t_fill));

    _reshape(t_shape);
}

Tensor Tensor::operator[](const std::vector<std::optional<size_t>> &t_ind) const
{

    // TODO how to add slicing, e.g. allow whole dimension x[{0, :}]

    if (t_ind.size() != m_shape.size())
    {
        // TODO throw error
    }

    // Data
    Tensor slice;
    slice.m_data = m_data;
    slice.m_strides = std::vector<size_t>();
    slice.m_shape = std::vector<size_t>();
    slice.m_offset = 0;

    // Some Helpers
    std::vector<size_t> ind_was_empty;

    std::transform(
        cbegin(t_ind),
        cend(t_ind),
        std::back_inserter(ind_was_empty),
        [](const std::optional<size_t> &size) {
            return static_cast<size_t>(!size.has_value());
        });

    // Shape
    std::transform(
        cbegin(ind_was_empty),
        cend(ind_was_empty),
        m_shape.cbegin(),
        std::back_inserter(slice.m_shape),
        std::multiplies<size_t>());

    slice.m_shape.erase(
        std::remove(
            begin(slice.m_shape),
            end(slice.m_shape),
            0),
        end(slice.m_shape));

    // Offset
    std::vector<size_t> offset_ind;

    std::transform(
        cbegin(t_ind),
        cend(t_ind),
        std::back_inserter(offset_ind),
        [](const std::optional<size_t> &size) {
            return size.value_or(0);
        });

    std::vector<size_t> offsets;
    std::transform(
        cbegin(offset_ind),
        cend(offset_ind),
        cbegin(m_strides),
        std::back_inserter(offsets),
        std::multiplies<size_t>());

    slice.m_offset = m_offset + std::accumulate(cbegin(offsets), cend(offsets), 0);

    // Strides
    std::transform(
        cbegin(ind_was_empty),
        cend(ind_was_empty),
        cbegin(m_strides),
        std::back_inserter(slice.m_strides),
        std::multiplies<size_t>());

    slice.m_strides.erase(
        std::remove(
            begin(slice.m_strides),
            end(slice.m_strides),
            0),
        end(slice.m_strides));

    return slice;
}

Tensor Tensor::mm(const Tensor &t_ten) const
{

    //check dimensions
    if (m_shape.back() != t_ten.m_shape.front())
    {
        throw std::invalid_argument("Internal dimensions of tensors do not match: N x " +
                                    std::to_string(m_shape.back()) + " and " +
                                    std::to_string(t_ten.m_shape.front()) + " x M");
    }
    return Tensor();
}

Tensor Tensor::operator*(const Tensor &t_ten) const
{
    return this->mm(t_ten);
}

// Tensor Tensor::add(const Tensor &) const
// {
//     return Tensor();
// }

// Tensor Tensor::operator+(const Tensor &t_m) const
// {
//     return Tensor();
// }

std::vector<dtype> Tensor::flatten() const
{
    // TODO only the data you can get to from the striding
    return *m_data;
}

std::vector<size_t> Tensor::get_shape() const
{
    return m_shape;
}

dtype Tensor::get(const std::vector<size_t> &t_ind) const
{
    std::vector<size_t> actual_ind;
    std::transform(
        cbegin(t_ind),
        cend(t_ind),
        cbegin(m_strides),
        std::back_inserter(actual_ind),
        std::multiplies<size_t>());

    return m_data->at(m_offset + std::accumulate(cbegin(actual_ind), cend(actual_ind), 0));
}

void Tensor::_reshape(std::initializer_list<size_t> t_shape)
{

    m_shape = std::vector<size_t>(t_shape);
    m_strides = std::vector<size_t>();

    if (t_shape.size() == 1)
    {
        m_strides.push_back(1);
    }
    else
    {
        // If 2 or more dim then swap the first two strides.
        // so defaut repr is rows x cols
        size_t sum = m_shape.at(1);
        m_strides.push_back(sum);
        m_strides.push_back(1);

        // Calculate the rest of the strides if there are any dims left
        sum *= m_shape.at(0);
        for (auto iter = cbegin(m_shape) + 2; iter != cend(m_shape); ++iter)
        {
            m_strides.push_back(sum);
            sum *= *iter;
        }
    }
}

Tensor Tensor::reshape(std::initializer_list<size_t> t_shape) const
{
    Tensor t = *this;
    t._reshape(t_shape);
    return t;
}

Tensor Tensor::from(const std::initializer_list<dtype> t_data)
{
    Tensor t;
    t.m_data = std::make_shared<std::vector<dtype>>(t_data);
    t.m_strides = std::vector<size_t>({1});
    t.m_shape = std::vector<size_t>({t_data.size()});
    t.m_offset = 0;
    return t;
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

    if (t.m_shape.size() == 1)
    {
        *t_op << "{";

        for (size_t x = 0; x < t.m_shape.at(0); ++x)
        {

            dtype val = t.get({x});

            // add extra space to account for minus sign
            if (val >= 0)
                *t_op << " ";

            *t_op << val << " ";
        }
        *t_op << "}\n";
    }
    else if (t.m_shape.size() == 2)
    {
        for (size_t x = 0; x < t.m_shape.at(0); ++x)
        {
            for (size_t y = 0; y < t.m_shape.at(1); ++y)
            {
                dtype val = t.get({x, y});

                *t_op << " | ";

                // add extra space to account for minus sign
                if (val >= 0)
                    *t_op << " ";

                *t_op << val << " ";
            }
            *t_op << " |\n";
        }
    }

    // for (dtype v : t.flatten())
    // {
    //     // add extra space to account for minus sign
    //     if (v >= 0)
    //         *t_op << " ";

    //     *t_op << v << ", ";
    // }

    // *t_op << "\n\n";
    *t_op << "\n";
}

} // namespace Teslyn