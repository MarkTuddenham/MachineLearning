#include <algorithm>
#include <functional>
#include <memory>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <string>

#include <iterator>

#include "teslyn/core/tensor.hpp"

namespace Teslyn
{

Tensor::Tensor() : m_offset(0)
{
}

Tensor::Tensor(Shape t_shape, dtype t_fill) : m_offset(0)
{
    m_data = std::make_shared<_Data>(
        std::vector<dtype>(
            std::accumulate(
                cbegin(t_shape),
                cend(t_shape),
                1,
                std::multiplies<size_t>()),
            t_fill));

    _reshape(t_shape);
}

Tensor Tensor::operator[](const PartIndex &t_ind) const
{

    // TODO and that all the indicies are in limits
    // maybe just let it overflow
    if (t_ind.size() != m_shape.size())
    {
        // TODO throw error
    }

    // Data
    Tensor slice;
    slice.m_data = m_data;
    slice.m_shape = std::vector<size_t>();
    slice.m_strides = std::vector<size_t>();

    // Helper Vectors
    Index ind_was_empty;

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
    Index offset_ind;

    std::transform(
        cbegin(t_ind),
        cend(t_ind),
        std::back_inserter(offset_ind),
        [](const std::optional<size_t> &size) {
            return size.value_or(0);
        });

    Index offsets;
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

    size_t shared_size = m_shape.back();

    Tensor res;
    res.m_shape = std::vector<size_t>();
    res.m_strides = std::vector<size_t>();

    res.m_shape.insert(end(res.m_shape), cbegin(m_shape), std::prev(cend(m_shape)));
    res.m_shape.insert(end(res.m_shape), std::next(cbegin(t_ten.m_shape)), cend(t_ten.m_shape));
    res._reshape(res.m_shape);

    res.m_data = std::make_shared<_Data>(std::vector<dtype>(
        std::accumulate(
            cbegin(res.m_shape),
            cend(res.m_shape),
            1,
            std::multiplies<size_t>())));

    // To do tensor multiplication we collapse the unecessary
    // leading and trailing dimensions
    // and then do matrix multiplication
    // e.g. 2x2x3 @ 3x4x4 -> 4x3 @ 3x16
    // then reshape to the outer dims

    Tensor collapsed_self = this->reshape({std::accumulate(
                                               cbegin(m_shape),
                                               std::prev(cend(m_shape)),
                                               static_cast<size_t>(0)),
                                           shared_size});

    Tensor collapsed_other = t_ten.reshape({shared_size,
                                            std::accumulate(
                                                std::next(cbegin(t_ten.m_shape)),
                                                cend(t_ten.m_shape),
                                                static_cast<size_t>(0))});

    for (size_t j = 0; j < collapsed_other.m_shape.back(); ++j)
    {
        for (size_t i = 0; i < collapsed_self.m_shape.front(); ++i)
        {

            auto self_row = collapsed_self[{i, all}].flatten();
            auto other_col = collapsed_other[{all, j}].flatten();

            size_t data_ind = j + i * collapsed_other.m_shape.back();

            res.m_data->at(data_ind) = std::inner_product(
                cbegin(self_row),
                cend(self_row),
                cbegin(other_col),
                static_cast<dtype>(0));
        }
    }

    return res;
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
    size_t size = std::accumulate(cbegin(m_shape), cend(m_shape), 0);
    std::vector<dtype> res;

    std::vector<size_t> partial_sum;
    partial_sum.emplace_back(1);
    std::partial_sum(cbegin(m_shape), cend(m_shape), std::back_inserter(partial_sum));

    for (size_t i = 0; i < size; ++i)
    {

        Index index;

        for (size_t s = 0; s < m_shape.size(); ++s)
        {
            index.emplace_back((i / partial_sum.at(s)) % partial_sum.at(s + 1));
        }

        res.emplace_back(get(index));
    }

    return res;
}

Shape Tensor::get_shape() const
{
    return m_shape;
}

dtype Tensor::get(const Index &t_ind) const
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

void Tensor::_reshape(const Shape t_shape)
{
    // TODO I think this will totally break if the tensor is
    // already a view of another one, i.e. the data for this tensor
    // is not contiguous in m_data.

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
        for (auto iter = std::next(cbegin(m_shape), 2); iter != cend(m_shape); ++iter)
        {
            m_strides.push_back(sum);
            sum *= *iter;
        }
    }
}

Tensor Tensor::reshape(const Shape t_shape) const
{
    // TODO allow partial shapes
    // TODO check product of new shape is the same as the product of the old.
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