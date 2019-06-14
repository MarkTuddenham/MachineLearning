#include "neural_network.hpp"

#include <functional>
#include <cmath>

#include "matrix.hpp"

template <typename T>
NeuralNetwork<T>::NeuralNetwork(std::vector<int> layers) : layers(layers)
{
  // Initialise weights & biases
  // inp layer + hidden layers + out layer

  //TODO validate number of layers cout >=3 or 2?

  for (size_t i = 1; i < layers.size(); i++)
  {

    this->w.push_back(Matrix<T>(layers[i - 1], layers[i]).randomise().setName("w_" + std::to_string(i)));
    this->b.push_back(Matrix<T>(1, layers[i]).randomise().setName("b_" + std::to_string(i)));
  }
}

template <typename T>
const std::vector<int> &NeuralNetwork<T>::get_layers() const
{
  return this->layers;
}

template <typename T>
Matrix<T> NeuralNetwork<T>::feedforward(const Matrix<T> &inputs)
{
  this->outs.clear();
  // turn inp array into matrix to calculate feedforward
  // call it out to use as first instance in the loop

  // TODO Copy properly
  Matrix<T> out = inputs;
  out.setName("inp");
  this->outs.push_back(out);

  for (size_t i = 0; i < layers.size() - 1; i++)
  {
    Matrix<T> net = (out * w[i]) + b[i];
    out = NeuralNetwork::applyActivationFunction(net, NeuralNetwork::Sigmoid);

    out.setName("out_" + std::to_string(i + 1));
    this->outs.push_back(out);
  }
  out.setName("Output");
  return out;
}

template <typename T>
void NeuralNetwork<T>::backpropagation(const Matrix<T> &inputs, const Matrix<T> &targets, T eta)
{

  Matrix<T> lr = Matrix<T>(1, 1, eta); // Learning rate as a matrix

  //compute total error
  feedforward(inputs);
  // Matrix t = targets.transpose();

  // calculate weight errors

  //TODO only sigmoid atm
  std::function<Matrix<T>(Matrix<T>)> delta_activ = [](Matrix<T> m) {
    return m.hadamard(Matrix<T>::ones(m.getRows(), m.getCols()) - m);
  };

  std::vector<Matrix<T>> w_e, b_e;
  //weight to output layers (H_N->O) are computed with a different node delta
  unsigned int N = layers.size() - 1;
  Matrix<T> node_delta = (outs[N] - targets).hadamard(delta_activ(outs[N]));
  w_e.insert(w_e.begin(), (outs[N - 1].transpose() * node_delta).hadamard(lr));
  b_e.insert(b_e.begin(), node_delta.hadamard(lr));

  // loop to compute node deltas and weight error responsibilities from input to last hidden layer (I->H_[N-1])
  for (size_t i = layers.size() - 2; i > 0; i--)
  {
    node_delta = (node_delta * w[i].transpose()).hadamard(delta_activ(outs[i]));
    w_e.insert(w_e.begin(), (outs[i - 1].transpose() * node_delta).hadamard(lr));
    b_e.insert(b_e.begin(), node_delta.hadamard(lr));
  }

  // update weights
  // Sum and reset names
  for (size_t i = 0; i < w.size(); i++)
  {
    // std::cout << "w[i] = " << w[i].getRows() << "x" << w[i].getCols() << std::endl;
    // std::cout << "w_e[i] = " << w_e[i].getRows() << "x" << w_e[i].getCols() << std::endl;
    w[i] = (w[i] - w_e[i]).setName("w_" + std::to_string(i + 1));
    b[i] = (b[i] - b_e[i]).setName("b_" + std::to_string(i + 1));
  }
}

template <typename T>
void NeuralNetwork<T>::print(int precision, std::ostream *op)
{
  *op << std::endl
      << "~~~~~~~~~~ # Neural Network # ~~~~~~~~~~"
      << std::endl;

  for (size_t i = 0; i < this->w.size(); i++)
  {
    this->w[i].print(precision, op);
    *op << std::endl;
    this->b[i].print(precision, op);
    *op << std::endl;
  }
  *op << "~~~~~~~~~~ # ~~~~~~~~~~~~~~ # ~~~~~~~~~~"
      << std::endl
      << std::endl;
}

template <typename T>
Matrix<T> NeuralNetwork<T>::applyActivationFunction(const Matrix<T> &m, ActivationFunction af)
{
  std::function<double(double)> f;

  switch (af)
  {
  case Sigmoid:
    f = [](T d) {
      return 1 / (1 + std::exp(-d));
    };
    break;
  case FastSigmoid:
    f = [](T d) {
      return d / (1 + std::abs(d));
    };
    break;
  case Tanh:
    f = [](T d) {
      return std::tanh(d);
    };
    break;
  default:
    throw std::invalid_argument("Unknown Activation Function.");
    break;
  }
  return m.apply(f);
}

template class NeuralNetwork<float>;
template class NeuralNetwork<double>;