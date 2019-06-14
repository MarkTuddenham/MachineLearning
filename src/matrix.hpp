#pragma once

// class Matrix;

#include <vector>
#include <iostream>
#include <functional>

template <typename T = double>
class Matrix
{
private:
  unsigned int rows, cols;
  std::vector<std::vector<T>> d; //TODO change to just one vector and use better indexing
  std::string name;

  Matrix<T> elementwise(const Matrix<T> &m, std::function<T(T, T)> f) const;

public:
  Matrix(int rows, int cols, T fill = 0, std::string name = "M");

  Matrix<T> setName(std::string name);
  Matrix<T> randomise(void);
  unsigned int getRows() const;
  unsigned int getCols() const;

  bool operator==(const Matrix<T> &m) const;

  Matrix<T> multiply(const Matrix<T> &) const;
  Matrix<T> operator*(const Matrix<T> &)const;

  Matrix<T> add(const Matrix<T> &) const;
  Matrix<T> operator+(const Matrix<T> &) const;

  Matrix<T> subtract(const Matrix<T> &) const;
  Matrix<T> operator-(const Matrix<T> &) const;

  Matrix<T> hadamard(const Matrix<T> &) const;
  Matrix<T> transpose(void) const;

  Matrix<T> apply(std::function<T(T)> f) const;
  std::vector<T> flatten(void) const;
  void print(int precision = 3, std::ostream *op = &std::cout) const;

  static Matrix<T> fromArray(const std::vector<T> &);
  static Matrix<T> ones(unsigned int, unsigned int);
  static Matrix<T> zeros(unsigned int, unsigned int);
};
