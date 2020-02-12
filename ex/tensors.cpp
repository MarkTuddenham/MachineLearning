#include <iostream>
#include <cstdlib>

#include "teslyn/core/tensor.hpp"
#include <iomanip>

using Teslyn::Tensor;
void print_tensor(const Tensor& m, int t_precision = 3, std::ostream* t_op = &std::cout);


int main()
{
    std::cout << "Tensors\n";

    Tensor t = Tensor({2,2}, 1);

    print_tensor(t);

};


void print_tensor(const Tensor& m, int t_precision, std::ostream* t_op)
{
    //*t_op <<  m.get_shape();
    *t_op << std::setprecision(t_precision) << std::fixed;


    for (double v : m.flatten())
    {

            // add extra space to account for minus sign
            if (v >= 0)
                *t_op << " ";

            *t_op << v << ", ";
        
    }
}