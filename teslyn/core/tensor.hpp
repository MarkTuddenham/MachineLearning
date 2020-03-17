#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <iomanip>
#include <iostream>

namespace Teslyn
{

constexpr std::nullopt_t all = std::nullopt;

// Yes, I know about templating but I don't want
// to get into that yet.
typedef double dtype;

typedef std::vector<dtype> _Data;
typedef std::vector<size_t> Shape;
typedef std::vector<size_t> Stride; // TODO is this really wanted?
typedef std::vector<size_t> Index;
typedef std::vector<std::optional<size_t>> PartIndex;

class Tensor
{

public:
    Tensor(Shape t_shape, dtype t_fill = 0);

    Tensor operator[](const PartIndex &t_ind) const;

    Tensor mm(const Tensor &t_ten) const;
    Tensor operator*(const Tensor &t_ten) const;

    // Tensor add(const Tensor &t_ten) const;
    // Tensor operator+(const Tensor &t_ten) const;

    void _reshape(const Shape t_shape);
    Tensor reshape(const Shape t_shape) const;

    std::vector<dtype> flatten() const;
    Shape get_shape() const;

    dtype get(const Index &t_ind) const;

    // special constructors
    static Tensor from(const std::initializer_list<dtype> t_data);

private:
    Tensor();

    size_t m_offset;
    Shape m_shape;
    Stride m_strides;
    std::shared_ptr<_Data> m_data;

    friend void print_tensor(const Tensor &t, int t_precision, std::ostream *t_op);
};

void print_tensor(const Tensor &t, int t_precision = 3, std::ostream *t_op = &std::cout);

} // namespace Teslyn