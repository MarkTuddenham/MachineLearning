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
  NeuralNetwork() = delete;
  explicit NeuralNetwork(const std::vector<int> &t_layers);

  const std::vector<int> &get_layers() const;

  Matrix<T> feedforward(const Matrix<T> &t_inputs);
  void backpropagation(const Matrix<T> &t_inputs, const Matrix<T> &t_targets, T t_eta);
  void print(int t_precision = 3, std::ostream *t_op = &std::cout);

  enum Activation
  {
    Sigmoid,
    FastSigmoid,
    Tanh
  };
  static Matrix<T> apply_activation(const Matrix<T> &t_m, Activation t_a);
};