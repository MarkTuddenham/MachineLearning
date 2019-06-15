#include "matrix.hpp"

#include <stdexcept>
#include <numeric>
#include <random>
#include <iomanip>

template <typename T>
Matrix<T>::Matrix(unsigned int t_rows, unsigned int t_cols, T t_fill, std::string t_name) : m_rows(t_rows), m_cols(t_cols), m_name(t_name)
{
  this->m_data = std::vector<std::vector<T>>(m_rows, std::vector<T>(m_cols, t_fill));
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &t_m) const
{
  return m_data == t_m.m_data;
}

//~~~~~~~~~~~~~~~~~MATHS~~~~~~~~~~~~~~~~~~~~~~~~~
// [this][m]
template <typename T>
Matrix<T> Matrix<T>::multiply(const Matrix<T> &t_m) const
{
  //check dimensions
  if (m_cols != t_m.m_rows)
  {
    throw std::invalid_argument("Internal dimensions of matrices do not match: " +
                                std::to_string(m_rows) + "x" + std::to_string(m_cols) + " and " +
                                std::to_string(t_m.m_rows) + "x" + std::to_string(t_m.m_cols));
  }
  else
  {
    Matrix<T> res(m_rows, t_m.m_cols);
    Matrix<T> mt = t_m.transpose();
    for (unsigned int i = 0; i < res.m_rows; i++)
    {
      for (unsigned int j = 0; j < res.m_cols; j++)
      {
        //this->row[i] * m.column[j]
        res.m_data[i][j] = std::inner_product(m_data[i].data(), m_data[i].data() + m_data[i].size(), mt.m_data[j].data(), 0.0);
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
  Matrix<T> t(m_cols, m_rows);
  for (unsigned int i = 0; i < t.m_rows; i++)
  {
    for (unsigned int j = 0; j < t.m_cols; j++)
    {
      t.m_data[i][j] = m_data[j][i];
    }
  }
  return t;
}

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

template <typename T>
Matrix<T> Matrix<T>::randomise(void)
{
  static T lower_bound = -1;
  static T upper_bound = 1;
  static std::uniform_real_distribution<T> gaussiand(lower_bound, upper_bound);
  static std::default_random_engine re(1289674753);

  for (auto &r : m_data)
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
  bool amIScalar = this->get_rows() == 1 && this->get_cols() == 1;
  bool isOtherScalar = m.get_rows() == 1 && m.get_cols() == 1;

  if (amIScalar || isOtherScalar)
  {

    // if other is scalar then swap

    T n = amIScalar ? this->flatten()[0] : m.flatten()[0];
    unsigned int rows = amIScalar ? m.get_rows() : this->get_rows();
    unsigned int cols = amIScalar ? m.get_cols() : this->get_cols();

    Matrix<T> t(rows, cols);
    for (unsigned int i = 0; i < t.m_rows; i++)
    {
      for (unsigned int j = 0; j < t.m_cols; j++)
      {
        t.m_data[i][j] = f(
            n,
            (amIScalar ? m.m_data : m_data)[i][j]);
      }
    }
    return t;
  }

  // n-D elementwise

  if (m_rows != m.m_rows || m_cols != m.m_cols)
  {
    throw std::invalid_argument("Matrices are not the same size! " +
                                std::to_string(m_rows) + "x" + std::to_string(m_cols) + " and " +
                                std::to_string(m.m_rows) + "x" + std::to_string(m.m_cols));
  }
  else
  {
    Matrix t(m_rows, m_cols);
    for (unsigned int i = 0; i < t.m_rows; i++)
    {
      for (unsigned int j = 0; j < t.m_cols; j++)
      {
        t.m_data[i][j] = f(m_data[i][j], m.m_data[i][j]);
      }
    }
    return t;
  }
}

template <typename T>
Matrix<T> Matrix<T>::apply(std::function<T(T)> f) const
{
  Matrix<T> m(m_rows, m_cols);
  for (unsigned int i = 0; i < m.m_rows; i++)
  {
    for (unsigned int j = 0; j < m.m_cols; j++)
    {
      m.m_data[i][j] = f(m_data[i][j]);
    }
  }
  return m;
}

template <typename T>
Matrix<T> Matrix<T>::from_array(const std::vector<T> &arr)
{
  Matrix<T> m(1, arr.size());
  for (unsigned int i = 0; i < m.m_cols; i++)
  {
    m.m_data[0][i] = arr[i];
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
  for (auto const &r : m_data)
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
  if (m_name != "")
    *op << m_name << ": ";

  *op << m_rows << 'x' << m_cols << " Matrix:" << '\n';
  *op << std::setprecision(precision) << std::fixed;

  for (auto const &r : m_data)
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
}

template <typename T>
unsigned int Matrix<T>::get_rows() const
{
  return m_rows;
}

template <typename T>
unsigned int Matrix<T>::get_cols() const
{
  return m_cols;
}

template <typename T>
Matrix<T> Matrix<T>::set_name(std::string t_name)
{
  m_name = t_name;
  return *this;
}

template class Matrix<float>;
template class Matrix<double>;