#include "matrix.hpp"

#include <stdexcept>
#include <numeric>
#include <random>
#include <iomanip>

namespace Teslyn
{

Matrix::Matrix(unsigned int t_rows, unsigned int t_cols, double t_fill, std::string t_name) : m_rows(t_rows), m_cols(t_cols), m_name(t_name)
{
  this->m_data = std::vector<std::vector<double>>(m_rows, std::vector<double>(m_cols, t_fill));
}

bool Matrix::operator==(const Matrix &t_m) const
{
  return m_data == t_m.m_data; //TODO and the shape is the same
}

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
    for (unsigned int i = 0; i < res.m_rows; ++i)
    {
      for (unsigned int j = 0; j < res.m_cols; ++j)
      {
        //this->row[i] * m.column[j]
        res.m_data[i][j] = std::inner_product(m_data[i].data(), m_data[i].data() + m_data[i].size(), mt.m_data[j].data(), 0.0);
      }
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
  for (unsigned int i = 0; i < t.m_rows; ++i)
  {
    for (unsigned int j = 0; j < t.m_cols; ++j)
    {
      t.m_data[i][j] = m_data[j][i];
    }
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
    for (double &v : r)
    {
      v = gaussian_dist(gen);
    }
  }

  return *this;
}

//~~~~~~~~~~~~~~~~~HELPERS~~~~~~~~~~~~~~~~~~~~~~~~~

Matrix Matrix::elementwise(const Matrix &t_m, std::function<double(double, double)> t_f) const
{
  // Deal with scalar multiplication
  bool amIScalar = get_rows() == 1 && get_cols() == 1;
  bool isOtherScalar = t_m.get_rows() == 1 && t_m.get_cols() == 1;

  if (amIScalar || isOtherScalar)
  {

    // if other is scalar then swap

    double n = amIScalar ? flatten()[0] : t_m.flatten()[0];
    unsigned int rows = amIScalar ? t_m.get_rows() : get_rows();
    unsigned int cols = amIScalar ? t_m.get_cols() : get_cols();

    Matrix res(rows, cols);
    for (unsigned int i = 0; i < res.m_rows; ++i)
    {
      for (unsigned int j = 0; j < res.m_cols; ++j)
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
    for (unsigned int i = 0; i < res.get_rows(); ++i)
    {
      for (unsigned int j = 0; j < res.get_cols(); ++j)
      {
        res.m_data[i][j] = t_f(m_data[i][j], t_m.m_data[i][j]);
      }
    }
    return res;
  }
}

Matrix Matrix::apply(std::function<double(double)> t_f) const
{
  Matrix res(m_rows, m_cols);
  for (unsigned int i = 0; i < res.get_rows(); ++i)
  {
    for (unsigned int j = 0; j < res.get_cols(); ++j)
    {
      res.m_data[i][j] = t_f(m_data[i][j]);
    }
  }
  return res;
}

Matrix Matrix::from_array(const std::vector<double> &t_arr)
{
  Matrix res(1, t_arr.size());

  for (unsigned int i = 0; i < res.m_cols; ++i)
  {
    res.m_data[0][i] = t_arr[i];
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

std::vector<double> Matrix::flatten(void) const
{
  std::vector<double> flat;
  for (auto const &r : m_data)
  {
    for (double const &v : r)
    {
      flat.push_back(v);
    }
  }
  return flat;
}

void Matrix::print(int t_precision, std::ostream *t_op) const
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

unsigned int Matrix::get_rows() const
{
  return m_rows;
}

unsigned int Matrix::get_cols() const
{
  return m_cols;
}

Matrix Matrix::set_name(std::string t_name)
{
  m_name = t_name;
  return *this;
}

} // namespace Teslyn