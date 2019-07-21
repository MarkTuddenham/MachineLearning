#include "matrix.hpp"
#include "neural_network.hpp"

using Teslyn::Matrix;
using Teslyn::NeuralNetwork;

namespace Teslyn::display
{
void print_matrix(const Matrix &m, int t_precision = 3, std::ostream *t_op = &std::cout);
void print_nn(const NeuralNetwork &nn, std::ostream *t_op = &std::cout);
} // namespace Teslyn::display