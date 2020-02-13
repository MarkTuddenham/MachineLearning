#include <cstdlib>
#include <iostream>

#include <teslyn.hpp>

using Teslyn::print_tensor;
using Teslyn::Tensor;

int main()
{
    std::cout << "Tensors\n";

    Tensor a = Tensor::from({1, 2, 3, 4}).reshape({2, 2});
    Tensor b = Tensor::from({5, 6, 7, 8}).reshape({2, 2});

    print_tensor(a);

    print_tensor(a[{0, Teslyn::all}]);
    print_tensor(a[{1, Teslyn::all}]);
    print_tensor(a[{Teslyn::all, 0}]);
    print_tensor(a[{Teslyn::all, 1}]);

    std::cout << "~~~~~~~~~~~~~~~~~~\n\n";

    Tensor c = Tensor::from({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}).reshape({2, 2, 3});
    print_tensor(c);

    print_tensor(c[{Teslyn::all, 1, 2}]);
    print_tensor(c[{Teslyn::all, 1, Teslyn::all}]);

    std::cout << "~~~~~~~~~~~~~~~~~~\n\n";

    Tensor t = a * b;
    print_tensor(t);

    // t = c * a; // should error
};
