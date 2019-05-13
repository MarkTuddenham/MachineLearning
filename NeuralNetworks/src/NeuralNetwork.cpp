#include "NeuralNetwork.hpp"
#include "Matrix.hpp"

#include <functional>
#include <cmath>

NeuralNetwork::NeuralNetwork(std::vector<int> layers) : layers(layers)
{
  // Initialise weights & biases
  // inp layer + hidden layers + out layer

  //TODO validate number of layers cout >=3 or 2?

  for (unsigned int i = 1; i < layers.size(); i++)
  {

    this->w.push_back(Matrix(layers[i - 1], layers[i]).randomise());
    this->b.push_back(Matrix(1, layers[i]).randomise());
  }
};

Matrix NeuralNetwork::feedforward(const Matrix &inputs)
{
  this->outs.clear();
  // turn inp array into matrix to calculate feedforward
  // call it out to use as first instance in the loop

  // TODO Copy properly
  Matrix out = inputs;
  out.setName("inp");
  this->outs.push_back(out);

  for (unsigned int i = 0; i < layers.size() - 1; i++)
  {
    Matrix net = (out * w[i]) + b[i];
    out = NeuralNetwork::applyActivationFunction(net, NeuralNetwork::Sigmoid);

    out.setName("out_" + std::to_string(i + 1));
    this->outs.push_back(out);
  }
  out.setName("Output");
  return out;
}

void NeuralNetwork::backpropagation(const Matrix &inputs, const Matrix &targets, double eta = 0.1)
{

  Matrix lr = Matrix(1, 1, eta); // Learning rate as a matrix

  //compute total error
  feedforward(inputs);
  // Matrix t = targets.transpose();

  // calculate weight errors

  //TODO only sigmoid atm
  std::function<Matrix(Matrix)> delta_activ = [](Matrix m) {
    return m.hadamard(Matrix::ones(m.getRows(), m.getCols()) - m);
  };

  std::vector<Matrix> w_e;
  //weight to output layers (H_N->O) are computed with a different node delta
  unsigned int N = layers.size() - 1;
  Matrix node_delta = (outs[N] - targets).hadamard(delta_activ(outs[N]));
  w_e.insert(w_e.begin(), (outs[N - 1].transpose() * node_delta).hadamard(lr));

  // loop to compute node deltas and weight error responsibilities from input to last hidden layer (I->H_[N-1])
  for (unsigned int i = layers.size() - 2; i > 0; i--)
  {
    node_delta = (node_delta * w[i].transpose()).hadamard(delta_activ(outs[i]));
    w_e.insert(w_e.begin(), (outs[i - 1].transpose() * node_delta).hadamard(lr));
  }

  //update weights
  for (unsigned int i = 0; i < w.size(); i++)
  {
    // std::cout << "w[i] = " << w[i].getRows() << "x" << w[i].getCols() << std::endl;
    // std::cout << "w_e[i] = " << w_e[i].getRows() << "x" << w_e[i].getCols() << std::endl;
    w[i] = w[i] - w_e[i];
  }
}

Matrix NeuralNetwork::applyActivationFunction(const Matrix &m, ActivationFunction af)
{
  std::function<double(double)> f;

  switch (af)
  {
  case Sigmoid:
    f = [](double d) {
      return 1 / (1 + std::exp(-d));
    };
    break;
  case FastSigmoid:
    f = [](double d) {
      return d / (1 + std::abs(d));
    };
    break;
  case Tanh:
    f = [](double d) {
      return std::tanh(d);
    };
    break;
  default:
    throw std::invalid_argument("Unknown Activation Function.");
    break;
  }
  return m.apply(f);
}
