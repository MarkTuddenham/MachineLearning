#include <matrix.hpp>

template <typename T>
std::ostream &operator<<(std::ostream &os, Matrix<T> const &value)
{
    value.print(1, &os);
    return os;
}