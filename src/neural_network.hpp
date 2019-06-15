#pragma once

#include "matrix.hpp"
#include <vector>

template <typename T = double>
class NeuralNetwork
{
private:
  const std::vector<int> m_layers;
  std::vector<Matrix<T>> m_weights, m_biases, m_outs;

public:
  NeuralNetwork(std::vector<int>);

  const std::vector<int> &get_layers() const;

  Matrix<T> feedforward(const Matrix<T> &);
  void backpropagation(const Matrix<T> &, const Matrix<T> &, T eta);
  void print(int precision = 3, std::ostream *op = &std::cout);

  enum Activation
  {
    Sigmoid,
    FastSigmoid,
    Tanh
  };
  static Matrix<T> apply_activation(const Matrix<T> &, Activation);
};