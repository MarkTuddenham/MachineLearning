#pragma once

#include <functional>
#include <iostream>
#include <vector>

namespace Teslyn
{

class Matrix
{

public:
    Matrix() = delete;
    Matrix(size_t t_rows, size_t t_cols, double t_fill = 0, std::string t_name = "M");

    Matrix operator[](size_t t_i) const;

    Matrix& set_name(const std::string& t_name);
    std::string get_name() const;

    Matrix& randomise();
    size_t get_rows() const;
    size_t get_cols() const;

    bool operator==(const Matrix& t_m) const;

    Matrix multiply(const Matrix& t_m) const;
    Matrix operator*(const Matrix& t_m) const;

    Matrix reciprocal() const;

    Matrix add(const Matrix&) const;
    Matrix operator+(const Matrix& t_m) const;

    Matrix subtract(const Matrix& t_m) const;
    Matrix operator-(const Matrix& t_m) const;

    Matrix hadamard(const Matrix& t_m) const;
    Matrix t(void) const;

    Matrix apply(std::function<double(double)> t_f) const;
    std::vector<double> flatten() const;

    Matrix& reshape(size_t t_rows, size_t t_cols);

    static Matrix from_array(const std::vector<double>& t_arr);
    static Matrix ones(size_t t_rows, size_t t_cols);
    static Matrix zeros(size_t t_rows, size_t t_cols);
    static Matrix I(size_t t_size);

private:
    size_t m_rows, m_cols;
    std::vector<double> m_data;
    std::string m_name;

    size_t get_size() const;
    Matrix elementwise(const Matrix& t_m, std::function<double(double, double)> t_f) const;
};

} // namespace Teslyn