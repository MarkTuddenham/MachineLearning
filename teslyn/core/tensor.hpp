#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <iomanip>
#include <iostream>

namespace Teslyn
{

constexpr std::nullopt_t all = std::nullopt;

class Tensor
{

public:
    Tensor(std::vector<size_t> t_shape, double t_fill = 0);

    Tensor operator[](std::initializer_list<std::optional<size_t>> t_ind) const;

    // Tensor multiply(const Tensor &t_m) const;
    // Tensor operator*(const Tensor &t_m) const;

    // Tensor add(const Tensor &) const;
    // Tensor operator+(const Tensor &t_m) const;

    std::vector<double> flatten() const;
    std::vector<size_t> get_shape() const;

private:
    Tensor();

    size_t m_offset;
    std::vector<size_t> m_shape;
    std::vector<size_t> m_strides;
    std::shared_ptr<std::vector<double>> m_data;

    friend void print_tensor(const Tensor &, int, std::ostream *);
};

void print_tensor(const Tensor &t, int t_precision = 3, std::ostream *t_op = &std::cout);

} // namespace Teslyn