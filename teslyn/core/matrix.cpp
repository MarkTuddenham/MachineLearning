#include <cmath>
#include <iomanip>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>

#include "teslyn/core/matrix.hpp"

namespace Teslyn
{

Matrix::Matrix(size_t t_rows, size_t t_cols, double t_fill, std::string t_name) : m_rows(t_rows), m_cols(t_cols), m_name(t_name)
{
  m_data = std::vector<double>(static_cast<size_t>(m_rows) * static_cast<size_t>(m_cols), t_fill);
}

bool Matrix::operator==(const Matrix &t_m) const
{
  return (m_data == t_m.m_data) &&
         (m_rows == t_m.get_rows()) &&
         (m_cols == t_m.get_cols());
}

//~~~~~~~~~~~~~~~~~ACCESS~~~~~~~~~~~~~~~~~~~~~~~~~

// Gets the row as a Matrix
Matrix Matrix::operator[](size_t ind) const
{

  size_t size = m_rows > 1 ? m_cols : m_rows;

  std::vector<double> ret(size);

  size_t first = ind * size;
  std::copy(begin(m_data) + first, begin(m_data) + first + size, begin(ret));

  return Matrix::from_array(ret);
}

//~~~~~~~~~~~~~~~~~MATHS~~~~~~~~~~~~~~~~~~~~~~~~~

Matrix Matrix::multiply(const Matrix &t_m) const
{

  // See if we are multiplying by a scaler
  bool is_left_scalar = m_cols == 1 && m_rows == 1;
  bool is_right_scalar = t_m.m_cols == 1 && t_m.m_rows == 1;
  if (is_left_scalar || is_right_scalar)
  {
    return hadamard(t_m);
  }
  else
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
      Matrix mt = t_m.t();
      for (size_t i = 0; i < res.get_size(); ++i)
      {
        size_t first_start = m_cols * (i / t_m.m_cols);
        size_t second_start = (i * mt.m_rows) % mt.get_size();

        res.m_data[i] = std::inner_product(
            begin(m_data) + first_start,
            begin(m_data) + first_start + m_cols,
            begin(mt.m_data) + second_start,
            0.0);
      }
      return res;
    }
  }
}

Matrix Matrix::operator*(const Matrix &t_m) const
{
  return this->multiply(t_m);
}

// Transpose
Matrix Matrix::t(void) const
{
  Matrix t(m_cols, m_rows);
  for (size_t i = 0; i < t.get_size(); ++i)
  {
    size_t old_ind = (i % m_rows) * m_cols + i / m_rows;
    t.m_data[i] = m_data[old_ind];
  }
  return t;
}

Matrix Matrix::reciprocal() const
{
  return apply(
      [](double x) {
        return (1 / x);
      });
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

Matrix &Matrix::randomise(void)
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

size_t Matrix::get_size() const
{
  return m_rows * m_cols;
}

Matrix &Matrix::reshape(size_t t_rows, size_t t_cols)
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
  // Deal with scalar
  bool amIScalar = get_rows() == 1 && get_cols() == 1;
  bool isOtherScalar = t_m.get_rows() == 1 && t_m.get_cols() == 1;

  if (amIScalar || isOtherScalar)
  {
    // if other is scalar then swap
    double n = amIScalar ? flatten()[0] : t_m.flatten()[0];
    size_t rows = amIScalar ? t_m.get_rows() : get_rows();
    size_t cols = amIScalar ? t_m.get_cols() : get_cols();

    Matrix res(rows, cols);
    for (size_t i = 0; i < res.get_size(); ++i)
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
    for (size_t i = 0; i < res.get_size(); ++i)
    {
      res.m_data[i] = t_f(m_data[i], t_m.m_data[i]);
    }
    return res;
  }
}

Matrix Matrix::apply(std::function<double(double)> t_f) const
{
  Matrix res(m_rows, m_cols);
  for (size_t i = 0; i < res.get_size(); ++i)
  {
    res.m_data[i] = t_f(m_data[i]);
  }
  return res;
}

Matrix Matrix::from_array(const std::vector<double> &t_arr)
{
  Matrix res(1, t_arr.size());
  res.m_data = t_arr;
  return res;
}

Matrix Matrix::ones(size_t t_rows, size_t t_cols)
{
  return Matrix(t_rows, t_cols, 1);
}

Matrix Matrix::zeros(size_t t_rows, size_t t_cols)
{
  return Matrix(t_rows, t_cols, 0);
}

std::vector<double> Matrix::flatten(void) const
{
  return m_data;
}

size_t Matrix::get_rows() const
{
  return m_rows;
}

size_t Matrix::get_cols() const
{
  return m_cols;
}

std::string Matrix::get_name() const
{
  return m_name;
}

Matrix &Matrix::set_name(const std::string &t_name)
{
  m_name = t_name;
  return *this;
}

Matrix Matrix::I(size_t t_size)
{
  Matrix m = Matrix::zeros(t_size, t_size);

  for (size_t i = 0; i < t_size * t_size; i += t_size + 1)
  {
    m.m_data[i] = 1;
  }
  return m.set_name("I");
}

} // namespace Teslyn