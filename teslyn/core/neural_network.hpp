#pragma once

#include "teslyn/core/matrix.hpp"

#include <vector>

namespace Teslyn
{

class NeuralNetwork
{
private:
    const std::vector<int> m_layers;
    std::vector<Matrix> m_weights, m_biases, m_outs;

public:
    NeuralNetwork() = delete;
    explicit NeuralNetwork(const std::vector<int>& t_layers);

    const std::vector<int>& get_layers() const;

    Matrix feedforward(const Matrix& t_inputs);
    void backpropagation(const Matrix& t_inputs, const Matrix& t_targets, double t_eta);

    enum class Activation
    {
        Sigmoid,
        FastSigmoid,
        Tanh
    };
    static Matrix apply_activation(const Matrix& t_m, Activation t_a);
};

} // namespace Teslyn