#include <cstdlib>
#include <iostream>

#include <teslyn.hpp>

using Teslyn::print_tensor;
using Teslyn::Tensor;

int main()
{
    std::cout << "Tensors\n";

    Tensor t = Tensor::from({1, 2, 3, 4}).reshape({2, 2});

    print_tensor(t);

    print_tensor(t[{0, Teslyn::all}]);
    print_tensor(t[{1, Teslyn::all}]);
    print_tensor(t[{Teslyn::all, 0}]);
    print_tensor(t[{Teslyn::all, 1}]);

    std::cout << "~~~~~~~~~~~~~~~~~~\n\n";

    t = Tensor::from({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}).reshape({2, 2, 3});
    print_tensor(t);

    print_tensor(t[{Teslyn::all, 1, 2}]);
    print_tensor(t[{Teslyn::all, 1, Teslyn::all}]);
};
