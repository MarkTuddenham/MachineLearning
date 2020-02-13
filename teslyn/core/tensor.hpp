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

class Tensor
{

public:
    Tensor(std::initializer_list<size_t> t_shape, dtype t_fill = 0);

    Tensor operator[](const std::vector<std::optional<size_t>> &t_ind) const;

    Tensor mm(const Tensor &t_ten) const;
    Tensor operator*(const Tensor &t_ten) const;

    // Tensor add(const Tensor &t_ten) const;
    // Tensor operator+(const Tensor &t_ten) const;

    void _reshape(std::initializer_list<size_t> t_shape);
    Tensor reshape(std::initializer_list<size_t> t_shape) const;

    std::vector<dtype> flatten() const;
    std::vector<size_t> get_shape() const;

    dtype get(const std::vector<size_t> &t_ind) const;

    // special constructors
    static Tensor from(const std::initializer_list<dtype> t_data);

private:
    Tensor();

    size_t m_offset;
    std::vector<size_t> m_shape;
    std::vector<size_t> m_strides;
    std::shared_ptr<std::vector<dtype>> m_data;

    friend void print_tensor(const Tensor &t, int t_precision, std::ostream *t_op);
};

void print_tensor(const Tensor &t, int t_precision = 3, std::ostream *t_op = &std::cout);

} // namespace Teslyn