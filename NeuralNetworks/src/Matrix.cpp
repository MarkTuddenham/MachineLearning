#include "Matrix.hpp"

#include <stdexcept>
#include <numeric>
#include <random>
#include <iomanip>

template <typename T>
Matrix<T>::Matrix(int rows, int cols, T fill, std::string name) : rows(rows), cols(cols), name(name)
{
  this->d = std::vector<std::vector<T>>(rows, std::vector<T>(cols, fill));
};

//~~~~~~~~~~~~~~~~~MATHS~~~~~~~~~~~~~~~~~~~~~~~~~
// [this][m]
template <typename T>
Matrix<T> Matrix<T>::multiply(const Matrix<T> &m) const
{
  //check dimensions
  if (this->cols != m.rows)
  {
    throw std::invalid_argument("Internal dimensions of matrices do not match: " +
                                std::to_string(this->rows) + "x" + std::to_string(this->cols) + " and " +
                                std::to_string(m.rows) + "x" + std::to_string(m.cols));
  }
  else
  {
    Matrix<T> res(this->rows, m.cols);
    Matrix<T> mt = m.transpose();
    for (unsigned int i = 0; i < res.rows; i++)
    {
      for (unsigned int j = 0; j < res.cols; j++)
      {
        //this->row[i] * m.column[j]
        res.d[i][j] = std::inner_product(this->d[i].data(), this->d[i].data() + this->d[i].size(), mt.d[j].data(), 0.0);
      }
    }
    return res;
  }
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &m) const
{
  return this->multiply(m);
}

template <typename T>
Matrix<T> Matrix<T>::transpose(void) const
{
  Matrix<T> t(this->cols, this->rows);
  for (unsigned int i = 0; i < t.rows; i++)
  {
    for (unsigned int j = 0; j < t.cols; j++)
    {
      t.d[i][j] = this->d[j][i];
    }
  }
  return t;
}

// Matrix Matrix::operator~() const
// {
//   return this->transpose();
// }

template <typename T>
Matrix<T> Matrix<T>::add(const Matrix<T> &m) const
{
  return this->elementwise(m, [](T a, T b) { return a + b; });
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &m) const
{
  return this->add(m);
}

template <typename T>
Matrix<T> Matrix<T>::subtract(const Matrix<T> &m) const
{
  return this->elementwise(m, [](T a, T b) { return a - b; });
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &m) const
{
  return this->subtract(m);
}

template <typename T>
Matrix<T> Matrix<T>::hadamard(const Matrix<T> &m) const
{
  return this->elementwise(m, [](T a, T b) { return a * b; });
}
// Matrix Matrix::operator->*(const Matrix &m) const
// {
//   return this->hadamard(m);
// }

template <typename T>
Matrix<T> Matrix<T>::randomise(void)
{
  static T lower_bound = -1;
  static T upper_bound = 1;
  static std::uniform_real_distribution<T> gaussiand(lower_bound, upper_bound);
  static std::default_random_engine re(1289674753);

  for (auto &r : this->d)
  {
    for (T &v : r)
    {
      v = gaussiand(re);
    }
  }

  return *this;
}

//~~~~~~~~~~~~~~~~~HELPERS~~~~~~~~~~~~~~~~~~~~~~~~~
template <typename T>
Matrix<T> Matrix<T>::elementwise(const Matrix<T> &m, std::function<T(T, T)> f) const
{
  // Deal with scalar multiplication
  bool amIScalar = this->getRows() == 1 && this->getCols() == 1;
  bool isOtherScalar = m.getRows() == 1 && m.getCols() == 1;

  if (amIScalar || isOtherScalar)
  {

    // if other is scalar then swap

    T n = amIScalar ? this->flatten()[0] : m.flatten()[0];
    unsigned int rows = amIScalar ? m.getRows() : this->getRows();
    unsigned int cols = amIScalar ? m.getCols() : this->getCols();

    Matrix<T> t(rows, cols);
    for (unsigned int i = 0; i < t.rows; i++)
    {
      for (unsigned int j = 0; j < t.cols; j++)
      {
        t.d[i][j] = f(
            n,
            (amIScalar ? m.d : this->d)[i][j]);
      }
    }
    return t;
  }

  // n-D elementwise

  if (this->rows != m.rows || this->cols != m.cols)
  {
    throw std::invalid_argument("Matrices are not the same size! " +
                                std::to_string(this->rows) + "x" + std::to_string(this->cols) + " and " +
                                std::to_string(m.rows) + "x" + std::to_string(m.cols));
  }
  else
  {
    Matrix t(this->rows, this->cols);
    for (unsigned int i = 0; i < t.rows; i++)
    {
      for (unsigned int j = 0; j < t.cols; j++)
      {
        t.d[i][j] = f(this->d[i][j], m.d[i][j]);
      }
    }
    return t;
  }
}

template <typename T>
Matrix<T> Matrix<T>::apply(std::function<T(T)> f) const
{
  Matrix<T> m(this->rows, this->cols);
  for (unsigned int i = 0; i < m.rows; i++)
  {
    for (unsigned int j = 0; j < m.cols; j++)
    {
      m.d[i][j] = f(this->d[i][j]);
    }
  }
  return m;
}

template <typename T>
Matrix<T> Matrix<T>::fromArray(const std::vector<T> &arr)
{
  Matrix<T> m(1, arr.size());
  for (unsigned int i = 0; i < m.cols; i++)
  {
    m.d[0][i] = arr[i];
  }
  return m;
}

template <typename T>
Matrix<T> Matrix<T>::ones(unsigned int rows, unsigned int cols)
{
  return Matrix<T>(rows, cols, 1);
}

template <typename T>
Matrix<T> Matrix<T>::zeros(unsigned int rows, unsigned int cols)
{
  return Matrix<T>(rows, cols, 0);
}

template <typename T>
std::vector<T> Matrix<T>::flatten(void) const
{
  std::vector<T> flat;
  for (auto const &r : this->d)
  {
    for (T const &v : r)
    {
      flat.push_back(v);
    }
  }
  return flat;
}

template <typename T>
void Matrix<T>::print(int precision, std::ostream *op) const
{
  if (this->name != "")
    *op << this->name << ": ";
  *op << this->rows << 'x' << this->cols << " Matrix:" << '\n';
  *op << std::setprecision(precision) << std::fixed;

  for (auto const &r : this->d)
  {
    for (auto const &v : r)
    {
      *op << " | ";
      if (v > 0)
        *op << " ";
      *op << v;
    }
    *op << " |\n";
  }
};

template <typename T>
unsigned int Matrix<T>::getRows() const
{
  return this->rows;
}

template <typename T>
unsigned int Matrix<T>::getCols() const
{
  return this->cols;
}

template <typename T>
Matrix<T> Matrix<T>::setName(std::string name)
{
  this->name = name;
  return *this;
}

template class Matrix<float>;
template class Matrix<double>;
