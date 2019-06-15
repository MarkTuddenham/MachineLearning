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
Matrix<T> Matrix<T>::operator*(const Matrix<T> &t_m) const
{
  return this->multiply(t_m);
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
Matrix<T> Matrix<T>::add(const Matrix<T> &t_m) const
{
  return this->elementwise(t_m, [](T a, T b) { return a + b; });
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &t_m) const
{
  return this->add(t_m);
}

template <typename T>
Matrix<T> Matrix<T>::subtract(const Matrix<T> &t_m) const
{
  return this->elementwise(t_m, [](T a, T b) { return a - b; });
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &t_m) const
{
  return this->subtract(t_m);
}

template <typename T>
Matrix<T> Matrix<T>::hadamard(const Matrix<T> &t_m) const
{
  return this->elementwise(t_m, [](T a, T b) { return a * b; });
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
Matrix<T> Matrix<T>::elementwise(const Matrix<T> &t_m, std::function<T(T, T)> t_f) const
{
  // Deal with scalar multiplication
  bool amIScalar = get_rows() == 1 && get_cols() == 1;
  bool isOtherScalar = t_m.get_rows() == 1 && t_m.get_cols() == 1;

  if (amIScalar || isOtherScalar)
  {

    // if other is scalar then swap

    T n = amIScalar ? flatten()[0] : t_m.flatten()[0];
    unsigned int rows = amIScalar ? t_m.get_rows() : get_rows();
    unsigned int cols = amIScalar ? t_m.get_cols() : get_cols();

    Matrix<T> res(rows, cols);
    for (unsigned int i = 0; i < res.m_rows; i++)
    {
      for (unsigned int j = 0; j < res.m_cols; j++)
      {
        res.m_data[i][j] = t_f(
            n,
            (amIScalar ? t_m.m_data : m_data)[i][j]);
      }
    }
    return res;
  }

  // n-D elementwise

  if (m_rows != t_m.m_rows || m_cols != t_m.m_cols)
  {
    throw std::invalid_argument("Matrices are not the same size! " +
                                std::to_string(m_rows) + "x" + std::to_string(m_cols) + " and " +
                                std::to_string(t_m.m_rows) + "x" + std::to_string(t_m.m_cols));
  }
  else
  {
    Matrix res(m_rows, m_cols);
    for (unsigned int i = 0; i < res.get_rows(); i++)
    {
      for (unsigned int j = 0; j < res.get_cols(); j++)
      {
        res.m_data[i][j] = t_f(m_data[i][j], t_m.m_data[i][j]);
      }
    }
    return res;
  }
}

template <typename T>
Matrix<T> Matrix<T>::apply(std::function<T(T)> t_f) const
{
  Matrix<T> res(m_rows, m_cols);
  for (unsigned int i = 0; i < res.get_rows(); i++)
  {
    for (unsigned int j = 0; j < res.get_cols(); j++)
    {
      res.m_data[i][j] = t_f(m_data[i][j]);
    }
  }
  return res;
}

template <typename T>
Matrix<T> Matrix<T>::from_array(const std::vector<T> &t_arr)
{
  Matrix<T> res(1, t_arr.size());

  for (unsigned int i = 0; i < res.m_cols; i++)
  {
    res.m_data[0][i] = t_arr[i];
  }
  return res;
}

template <typename T>
Matrix<T> Matrix<T>::ones(unsigned int t_rows, unsigned int t_cols)
{
  return Matrix<T>(t_rows, t_cols, 1);
}

template <typename T>
Matrix<T> Matrix<T>::zeros(unsigned int t_rows, unsigned int t_cols)
{
  return Matrix<T>(t_rows, t_cols, 0);
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
void Matrix<T>::print(int t_precision, std::ostream *t_op) const
{
  if (m_name != "")
    *t_op << m_name << ": ";

  *t_op << m_rows << 'x' << m_cols << " Matrix:\n";
  *t_op << std::setprecision(t_precision) << std::fixed;

  for (auto const &r : m_data)
  {
    for (auto const &v : r)
    {
      *t_op << " | ";

      // add extra space to account for minus sign
      if (v > 0)
        *t_op << " ";

      *t_op << v;
    }
    *t_op << " |\n";
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