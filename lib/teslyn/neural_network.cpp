#include <functional>
#include <cmath>

#include "teslyn/neural_network.hpp"
#include "teslyn/matrix.hpp"

namespace Teslyn
{

NeuralNetwork::NeuralNetwork(const std::vector<int> &t_layers) : m_layers(t_layers)
{
  // Initialise weights & biases
  // inp layer + hidden layers + out layer

  //TODO validate number of layers cout >=3 or 2?

  for (size_t i = 1; i < m_layers.size(); ++i)
  {

    m_weights.push_back(Matrix(m_layers[i - 1], m_layers[i]).randomise().set_name("w_" + std::to_string(i)));
    m_biases.push_back(Matrix(1, m_layers[i]).randomise().set_name("b_" + std::to_string(i)));
  }
}

const std::vector<int> &NeuralNetwork::get_layers() const
{
  return m_layers;
}

Matrix NeuralNetwork::feedforward(const Matrix &t_inputs)
{
  m_outs.clear();
  // turn inp array into matrix to calculate feedforward
  // call it out to use as first instance in the loop

  // TODO Copy properly
  Matrix out = t_inputs;
  out.set_name("inp");
  m_outs.push_back(out);

  for (size_t i = 0; i < m_layers.size() - 1; ++i)
  {
    Matrix net = (out * m_weights[i]) + m_biases[i];
    out = NeuralNetwork::apply_activation(net, NeuralNetwork::Sigmoid);

    out.set_name("out_" + std::to_string(i + 1));
    m_outs.push_back(out);
  }
  out.set_name("Output");
  return out;
}

void NeuralNetwork::backpropagation(const Matrix &t_inputs, const Matrix &t_targets, double t_eta)
{

  Matrix lr = Matrix(1, 1, t_eta); // Learning rate as a matrix

  //compute total error
  feedforward(t_inputs);
  // Matrix t = t_targets.transpose();

  // calculate weight errors

  //TODO only sigmoid atm
  std::function<Matrix(Matrix)> delta_activ = [](Matrix m) {
    return m.hadamard(Matrix::ones(m.get_rows(), m.get_cols()) - m);
  };

  std::vector<Matrix> w_e, b_e;
  //weight to output layers (H_N->O) are computed with a different node delta
  unsigned int N = m_layers.size() - 1;
  Matrix node_delta = (m_outs[N] - t_targets).hadamard(delta_activ(m_outs[N]));
  w_e.insert(w_e.begin(), (m_outs[N - 1].transpose() * node_delta).hadamard(lr));
  b_e.insert(b_e.begin(), node_delta.hadamard(lr));

  // loop to compute node deltas and weight error responsibilities from input to last hidden layer (I->H_[N-1])
  for (size_t i = m_layers.size() - 2; i > 0; --i)
  {
    node_delta = (node_delta * m_weights[i].transpose()).hadamard(delta_activ(m_outs[i]));
    w_e.insert(w_e.begin(), (m_outs[i - 1].transpose() * node_delta).hadamard(lr));
    b_e.insert(b_e.begin(), node_delta.hadamard(lr));
  }

  // update weights
  // Sum and reset names
  for (size_t i = 0; i < m_weights.size(); ++i)
  {
    // std::cout << "m_weights[i] = " << m_weights[i].get_rows() << "x" << m_weights[i].get_cols() << std::endl;
    // std::cout << "w_e[i] = " << w_e[i].get_rows() << "x" << w_e[i].get_cols() << std::endl;
    m_weights[i] = (m_weights[i] - w_e[i]).set_name("w_" + std::to_string(i + 1));
    m_biases[i] = (m_biases[i] - b_e[i]).set_name("b_" + std::to_string(i + 1));
  }
}

Matrix NeuralNetwork::apply_activation(const Matrix &t_m, Activation t_a)
{
  std::function<double(double)> f;

  switch (t_a)
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

  return t_m.apply(f);
}

} // namespace Teslyn