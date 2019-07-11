#pragma once

#include <vector>
#include <iostream>
#include <functional>

namespace Teslyn
{

class Matrix
{
private:
  unsigned int m_rows, m_cols;
  std::vector<std::vector<double>> m_data; //TODO change to just one vector and use better indexing
  std::string m_name;

  Matrix elementwise(const Matrix &t_m, std::function<double(double, double)> t_f) const;

public:
  Matrix() = delete;
  Matrix(unsigned int t_rows, unsigned int t_cols, double t_fill = 0, std::string t_name = "M");

  Matrix set_name(const std::string &t_name);
  std::string get_name() const;

  Matrix randomise(void);
  unsigned int get_rows() const;
  unsigned int get_cols() const;

  bool operator==(const Matrix &t_m) const;

  Matrix multiply(const Matrix &t_m) const;
  Matrix operator*(const Matrix &t_m) const;

  Matrix add(const Matrix &) const;
  Matrix operator+(const Matrix &t_m) const;

  Matrix subtract(const Matrix &t_m) const;
  Matrix operator-(const Matrix &t_m) const;

  Matrix hadamard(const Matrix &t_m) const;
  Matrix transpose(void) const;

  Matrix apply(std::function<double(double)> t_f) const;
  std::vector<double> flatten(void) const;

  static Matrix from_array(const std::vector<double> &t_arr);
  static Matrix ones(unsigned int t_rows, unsigned int t_cols);
  static Matrix zeros(unsigned int t_rows, unsigned int t_cols);
};

} // namespace Teslyn