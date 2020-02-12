#include <cstdlib>
#include <iostream>

#include <teslyn.hpp>

using Teslyn::print_tensor;
using Teslyn::Tensor;

int main()
{
    std::cout << "Tensors\n";

    Tensor t = Tensor({2, 2}, 1);

    print_tensor(t);

    print_tensor(t[{0, Teslyn::all}]);
    print_tensor(t[{1, Teslyn::all}]);
    print_tensor(t[{Teslyn::all, 0}]);
    print_tensor(t[{Teslyn::all, 1}]);

    t = Tensor({2, 2, 3}, 1);
    print_tensor(t[{Teslyn::all, 0, Teslyn::all}]); // FUUUUUUUU
    // TODO using Teslyn::all gets rid of a dim instead of keeping it
    // otherwise the offsets and actual values for strides look ok
};
