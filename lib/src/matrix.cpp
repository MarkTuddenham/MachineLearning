#include "matrix.hpp"

#include <stdexcept>
#include <numeric>
#include <random>
#include <iomanip>

namespace Teslyn
{

Matrix::Matrix(unsigned int t_rows, unsigned int t_cols, double t_fill, std::string t_name) : m_rows(t_rows), m_cols(t_cols), m_name(t_name)
{
  this->m_data = std::vector<double>(m_rows * m_cols, t_fill);
}

bool Matrix::operator==(const Matrix &t_m) const
{
  return (m_data == t_m.m_data) &&
         (m_rows == t_m.get_rows()) &&
         (m_cols == t_m.get_cols());
}

//~~~~~~~~~~~~~~~~~ACCESS~~~~~~~~~~~~~~~~~~~~~~~~~
// unsigned long operator[](unsigned int i) const
// {
//   return m_data[i];
// }

//~~~~~~~~~~~~~~~~~MATHS~~~~~~~~~~~~~~~~~~~~~~~~~
// [this][m]

Matrix Matrix::multiply(const Matrix &t_m) const
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
    Matrix res(m_rows, t_m.m_cols);
    Matrix mt = t_m.transpose();
    unsigned int first_start, first_end, second_start;
    for (unsigned int i = 0; i < res.get_size(); ++i)
    {
      first_start = i / res.get_cols();
      first_end = first_start + get_cols();
      second_start = i % res.get_cols();

      res.m_data[i] = std::inner_product(begin(m_data) + first_start, begin(m_data) + first_end, begin(t_m.m_data) + second_start, 0.0);
    }
    return res;
  }
}

Matrix Matrix::operator*(const Matrix &t_m) const
{
  return this->multiply(t_m);
}

Matrix Matrix::transpose(void) const
{
  Matrix t(m_cols, m_rows);
  unsigned int new_ind;
  for (unsigned int i = 0; i < t.get_size(); ++i)
  {
    new_ind = (i % m_rows) * m_cols + i / m_rows;
    t.m_data[new_ind] = m_data[i];
  }
  return t;
}

Matrix Matrix::add(const Matrix &t_m) const
{
  return this->elementwise(t_m, [](double a, double b) { return a + b; });
}

Matrix Matrix::operator+(const Matrix &t_m) const
{
  return this->add(t_m);
}

Matrix Matrix::subtract(const Matrix &t_m) const
{
  return this->elementwise(t_m, [](double a, double b) { return a - b; });
}

Matrix Matrix::operator-(const Matrix &t_m) const
{
  return this->subtract(t_m);
}

Matrix Matrix::hadamard(const Matrix &t_m) const
{
  return this->elementwise(t_m, [](double a, double b) { return a * b; });
}

Matrix Matrix::randomise(void)
{
  static double lower_bound = -1;
  static double upper_bound = 1;
  static std::uniform_real_distribution<double> gaussian_dist(lower_bound, upper_bound);
  static std::random_device rd;
  static std::mt19937 gen(rd());

  for (auto &r : m_data)
  {
    r = gaussian_dist(gen);
  }

  return *this;
}

//~~~~~~~~~~~~~~~~~HELPERS~~~~~~~~~~~~~~~~~~~~~~~~~

unsigned int Matrix::get_size() const
{
  return m_rows * m_cols;
}

Matrix Matrix::reshape(unsigned int t_rows, unsigned int t_cols)
{
  if (t_rows * t_cols == get_size())
  {
    m_rows = t_rows;
    m_cols = t_cols;
  }
  else
  {
    throw std::invalid_argument("Can't reshape matrix with " +
                                std::to_string(get_size()) + " elements to a " +
                                std::to_string(t_rows) + "x" + std::to_string(t_cols) +
                                " matrix of " + std::to_string(t_rows * t_cols) + " elements.");
  }

  return *this;
}

Matrix Matrix::elementwise(const Matrix &t_m, std::function<double(double, double)> t_f) const
{
  // Deal with scalar multiplication
  bool amIScalar = get_rows() == 1 && get_cols() == 1;
  bool isOtherScalar = t_m.get_rows() == 1 && t_m.get_cols() == 1;

  if (amIScalar || isOtherScalar)
  {
    // if other is scalar then swap
    double n = amIScalar ? get_flattened()[0] : t_m.get_flattened()[0];
    unsigned int rows = amIScalar ? t_m.get_rows() : get_rows();
    unsigned int cols = amIScalar ? t_m.get_cols() : get_cols();

    Matrix res(rows, cols);
    for (unsigned int i = 0; i < res.get_size(); ++i)
    {
      res.m_data[i] = t_f(
          n,
          (amIScalar ? t_m.m_data : m_data)[i]);
    }
    return res;
  }

  // n-D elementwise

  if (get_rows() != t_m.get_rows() || get_cols() != t_m.get_cols())
  {
    throw std::invalid_argument("Matrices are not the same size! " +
                                std::to_string(get_rows()) + "x" + std::to_string(get_cols()) + " and " +
                                std::to_string(t_m.get_rows()) + "x" + std::to_string(t_m.get_cols()));
  }
  else
  {
    Matrix res(m_rows, m_cols);
    for (unsigned int i = 0; i < res.get_size(); ++i)
    {
      res.m_data[i] = t_f(m_data[i], t_m.m_data[i]);
    }
    return res;
  }
}

Matrix Matrix::apply(std::function<double(double)> t_f) const
{
  Matrix res(m_rows, m_cols);
  for (unsigned int i = 0; i < res.get_size(); ++i)
  {
    res.m_data[i] = t_f(m_data[i]);
  }
  return res;
}

Matrix Matrix::from_array(const std::vector<double> &t_arr)
{
  Matrix res(1, t_arr.size());

  for (unsigned int i = 0; i < res.get_size(); ++i)
  {
    res.m_data[i] = t_arr[i];
  }
  return res;
}

Matrix Matrix::ones(unsigned int t_rows, unsigned int t_cols)
{
  return Matrix(t_rows, t_cols, 1);
}

Matrix Matrix::zeros(unsigned int t_rows, unsigned int t_cols)
{
  return Matrix(t_rows, t_cols, 0);
}

std::vector<double> Matrix::get_flattened(void) const
{
  return m_data;
}

unsigned int Matrix::get_rows() const
{
  return m_rows;
}

unsigned int Matrix::get_cols() const
{
  return m_cols;
}

std::string Matrix::get_name() const
{
  return m_name;
}

Matrix Matrix::set_name(const std::string &t_name)
{
  m_name = t_name;
  return *this;
}

} // namespace Teslyn