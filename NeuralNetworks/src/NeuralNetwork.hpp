#pragma once

#include "Matrix.hpp"
#include <vector>

class NeuralNetwork
{
private:
  const std::vector<int> layers;
  std::vector<Matrix> w, b, outs;

public:
  NeuralNetwork(std::vector<int>);
  Matrix feedforward(const Matrix &);
  void backpropagation(const Matrix &, const Matrix &, double);

  enum ActivationFunction
  {
    Sigmoid,
    FastSigmoid,
    Tanh
  };
  static Matrix applyActivationFunction(const Matrix &, ActivationFunction);
};