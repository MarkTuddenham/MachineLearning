#pragma once

// class Matrix;

#include <vector>
#include <iostream>
#include <functional>

template <typename T = double>
class Matrix
{
private:
  unsigned int m_rows, m_cols;
  std::vector<std::vector<T>> m_data; //TODO change to just one vector and use better indexing
  std::string m_name;

  Matrix<T> elementwise(const Matrix<T> &t_m, std::function<T(T, T)> t_f) const;

public:
  Matrix(unsigned int t_rows, unsigned int t_cols, T t_fill = 0, std::string t_name = "M");

  Matrix<T> set_name(std::string);
  std::string get_name() const;

  Matrix<T> randomise(void);
  unsigned int get_rows() const;
  unsigned int get_cols() const;

  bool operator==(const Matrix<T> &) const;

  Matrix<T> multiply(const Matrix<T> &) const;
  Matrix<T> operator*(const Matrix<T> &)const;

  Matrix<T> add(const Matrix<T> &) const;
  Matrix<T> operator+(const Matrix<T> &) const;

  Matrix<T> subtract(const Matrix<T> &) const;
  Matrix<T> operator-(const Matrix<T> &) const;

  Matrix<T> hadamard(const Matrix<T> &) const;
  Matrix<T> transpose(void) const;

  Matrix<T> apply(std::function<T(T)>) const;
  std::vector<T> flatten(void) const;
  void print(int t_precision = 3, std::ostream *t_op = &std::cout) const;

  static Matrix<T> from_array(const std::vector<T> &);
  static Matrix<T> ones(unsigned int, unsigned int);
  static Matrix<T> zeros(unsigned int, unsigned int);
};
