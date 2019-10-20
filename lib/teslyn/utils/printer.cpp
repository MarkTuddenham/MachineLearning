#include <iomanip>

#include "teslyn/matrix.hpp"
#include "teslyn/neural_network.hpp"

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

    std::vector<double> values = m.get_flattened();

    for (size_t i = 0; i < m.get_rows(); ++i)
    {
        for (size_t j = 0; j < m.get_cols(); ++j)
        {
            int ind = i * m.get_cols() + j;

            *t_op << " | ";

            // add extra space to account for minus sign
            if (values[ind] >= 0)
                *t_op << " ";

            *t_op << values[ind] << " ";
        }
        *t_op << " |\n";
    }
}

void print_nn(const NeuralNetwork &nn, std::ostream *t_op)
{
    *t_op << "~~~~~~~~~~ # Neural Network # ~~~~~~~~~~\n";
    *t_op << "Layers:\n";

    for (auto l : nn.get_layers())
    {
        *t_op << "   - Nodes: " << l;
        // *t_op << "Activ: " << l;
        *t_op << "\n";
    }

    *t_op << "~~~~~~~~~~ # ~~~~~~~~~~~~~~ # ~~~~~~~~~~\n";
}

} // namespace Teslyn::display