#pragma once

#include "matrix.hpp"
#include <vector>

template <typename T = double>
class NeuralNetwork
{
private:
  const std::vector<int> layers;
  std::vector<Matrix<T>> w, b, outs;

public:
  NeuralNetwork(std::vector<int>);

  const std::vector<int> &get_layers() const;

  Matrix<T> feedforward(const Matrix<T> &);
  void backpropagation(const Matrix<T> &, const Matrix<T> &, T eta);
  void print(int precision = 3, std::ostream *op = &std::cout);

  enum ActivationFunction
  {
    Sigmoid,
    FastSigmoid,
    Tanh
  };
  static Matrix<T> applyActivationFunction(const Matrix<T> &, ActivationFunction);
};