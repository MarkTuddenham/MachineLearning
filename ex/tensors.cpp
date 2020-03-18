#include <iterator>
#include <sstream>

#include <teslyn.hpp>

using Teslyn::Tensor;

int app()
{
    PROFILE_FUNCTION();

    TESLYN_INFO("Tensors");

    Tensor a = Tensor::from({1, 2, 3, 4, 5, 6, 7, 8}).reshape({4, 2});
    Tensor b = Tensor::from({5, 6, 7, 8, 9, 10}).reshape({2, 3});

    TESLYN_INFO(a.to_string());
    auto a_flat = a[{Teslyn::all, 1}].flatten();
    std::ostringstream a_flat_oss;
    std::copy(cbegin(a_flat), cend(a_flat), std::ostream_iterator<double>(a_flat_oss, ", "));
    TESLYN_TRACE(a_flat_oss.str());

    // print_tensor(b);

    // print_tensor(a[{0, Teslyn::all}]);
    // print_tensor(a[{1, Teslyn::all}]);
    // print_tensor(a[{Teslyn::all, 0}]);
    // print_tensor(a[{Teslyn::all, 1}]);

    // std::cout << "~~~~~~~~~~~~~~~~~~\n\n";

    // Tensor c = Tensor::from({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}).reshape({2, 2, 3});
    // print_tensor(c);

    // print_tensor(c[{Teslyn::all, 1, 2}]);
    // print_tensor(c[{Teslyn::all, 1, Teslyn::all}]);

    // std::cout << "~~~~~~~~~~~~~~~~~~\n\n";

    Tensor t = a * b;
    TESLYN_INFO(t.to_string());

    auto t_shape = t.get_shape();
    std::ostringstream t_shape_oss;
    std::copy(cbegin(t_shape), cend(t_shape), std::ostream_iterator<double>(t_shape_oss, ", "));
    TESLYN_TRACE(t_shape_oss.str());

    // TODO try on larger dim tensors

    // t = c * a; // should error

    return 0;
};

int main()
{
    Teslyn::Log::init(Teslyn::Log::TRACE);
    Instrumentor::Get().BeginSession("Teslyn");

    int r = app();

    Instrumentor::Get().EndSession();

    return r;
}
