#pragma once

class Matrix;

#include <vector>
#include <iostream>
#include <functional>

class Matrix
{
private:
  unsigned int rows, cols;
  std::vector<std::vector<double>> d;
  std::string name;

  Matrix elementwise(const Matrix &m, std::function<double(double, double)> f) const;

public:
  Matrix(int rows, int cols, double fill = 0, std::string name = "M");

  Matrix randomise(void);

  Matrix multiply(const Matrix &) const;
  Matrix operator*(const Matrix &)const;
  Matrix transpose(void) const;
  Matrix operator~() const;
  Matrix add(const Matrix &) const;
  Matrix operator+(const Matrix &) const;
  Matrix subtract(const Matrix &) const;
  Matrix operator-(const Matrix &) const;

  Matrix hadamard(const Matrix &) const;
  // Matrix operator->*(const Matrix&) const;

  Matrix apply(std::function<double(double)> f) const;

  std::vector<double> flatten(void) const;

  void print(int precision = 3, std::ostream *op = &std::cout) const;
  unsigned int getRows() const;
  unsigned int getCols() const;
  void setName(std::string name);

  static Matrix fromArray(const std::vector<double> &);
};
