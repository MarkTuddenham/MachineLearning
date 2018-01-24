#pragma once

class Matrix;

#include <vector>
#include <iostream>
#include <functional>


class Matrix {
  private:
      int rows, cols;
      std::vector< std::vector<double> > d;
      std::string name;

      Matrix elementwise(Matrix m, std::function<double(double, double)> f) const;

  public:

    Matrix(int rows, int cols, double fill = 0, std::string name = "M");

    Matrix randomise(void);

    Matrix multiply(Matrix) const;
    Matrix operator*(const Matrix&) const;
    Matrix transpose(void) const;
    Matrix operator~() const;
    Matrix add(Matrix) const;
    Matrix operator+(const Matrix&) const;
    Matrix subtract(Matrix) const;
    Matrix operator-(const Matrix&) const;
    Matrix hadamard(Matrix) const;
    Matrix operator->*(const Matrix&) const;

    Matrix apply(std::function<double(double)> f) const;

    static Matrix fromArray(std::vector<double>);
    std::vector<double> flatten(void) const;
    void print(int precision = 3,std::ostream* op = &std::cout) const;
    int getRows() const;
    int getCols() const;
    void setName(std::string name);
};
