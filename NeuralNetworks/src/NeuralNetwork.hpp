#pragma once

class NerualNetwork;

#include "Matrix.hpp"
#include <vector>

class NeuralNetwork {
  private:
      const std::vector<int> layers;
      std::vector<Matrix> w,b,outs;


  public:
    NeuralNetwork(std::vector<int>);
    Matrix feedforward(double*);
    void backpropagation(double*, double*, double eta = 0.1);

    enum ActivationFunction {Sigmoid, FastSigmoid, Tanh};
    static Matrix applyActivationFunction(Matrix m, ActivationFunction af);
};