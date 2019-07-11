#include <iomanip>

#include "../matrix.hpp"
#include "../neural_network.hpp"

using Teslyn::Matrix;
using Teslyn::NeuralNetwork;

namespace Teslyn::display
{

void print_matrix(const Matrix &m, int t_precision, std::ostream *t_op)
{
    if (m.get_name() != "")
        *t_op << m.get_name() << ": ";

    *t_op << m.get_rows() << 'x' << m.get_cols() << " Matrix:\n";
    *t_op << std::setprecision(t_precision) << std::fixed;

    for (auto const &v : m.get_flattened())
    {
        // TODO format properly for n dimensions
        // for (auto const &v : r)
        // {
        *t_op << " | ";

        // add extra space to account for minus sign
        if (v > 0)
            *t_op << " ";

        *t_op << v;
    }
    *t_op << " |\n";
    // }
}

void print_nn(const NeuralNetwork &nn, int t_precision, std::ostream *t_op)
{
    // *t_op << "\n~~~~~~~~~~ # Neural Network # ~~~~~~~~~~";

    // for (size_t i = 0; i < m_weights.size(); ++i)
    // {
    //     *t_op << "\n";
    //     m_weights[i].print(t_precision, t_op);
    //     *t_op << "\n";
    //     m_biases[i].print(t_precision, t_op);
    // }
    // *t_op << "~~~~~~~~~~ # ~~~~~~~~~~~~~~ # ~~~~~~~~~~\n\n";
}

} // namespace Teslyn::display