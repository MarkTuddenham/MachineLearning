#include "Matrix.hpp"

#include <stdexcept>
#include <numeric>
#include <random>
#include <iomanip>

// Matrix::Matrix(int rows, int cols):  Matrix("M", rows, cols, 0.0){};

Matrix::Matrix(int rows, int cols, double fill, std::string name) : rows(rows), cols(cols), name(name)
{
  this->d = std::vector<std::vector<double>>(rows, std::vector<double>(cols, fill));
};

//~~~~~~~~~~~~~~~~~MATHS~~~~~~~~~~~~~~~~~~~~~~~~~
// [this][m]
Matrix Matrix::multiply(const Matrix &m) const
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
    Matrix res(this->rows, m.cols);
    Matrix mt = m.transpose();
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
Matrix Matrix::operator*(const Matrix &m) const
{
  return this->multiply(m);
}

Matrix Matrix::transpose(void) const
{
  Matrix t(this->cols, this->rows);
  for (unsigned int i = 0; i < t.rows; i++)
  {
    for (unsigned int j = 0; j < t.cols; j++)
    {
      t.d[i][j] = this->d[j][i];
    }
  }
  return t;
}

Matrix Matrix::operator~() const
{
  return this->transpose();
}

Matrix Matrix::add(const Matrix &m) const
{
  return this->elementwise(m, [](double a, double b) { return a + b; });
}

Matrix Matrix::operator+(const Matrix &m) const
{
  return this->add(m);
}

Matrix Matrix::subtract(const Matrix &m) const
{
  return this->elementwise(m, [](double a, double b) { return a - b; });
}

Matrix Matrix::operator-(const Matrix &m) const
{
  return this->subtract(m);
}

Matrix Matrix::hadamard(const Matrix &m) const
{
  return this->elementwise(m, [](double a, double b) { return a * b; });
}
// Matrix Matrix::operator->*(const Matrix &m) const
// {
//   return this->hadamard(m);
// }

Matrix Matrix::randomise(void)
{
  static double lower_bound = -1;
  static double upper_bound = 1;
  static std::uniform_real_distribution<double> gaussiand(lower_bound, upper_bound);
  static std::default_random_engine re(1289674753);

  for (auto &r : this->d)
  {
    for (double &v : r)
    {
      v = gaussiand(re);
    }
  }

  return *this;
}

//~~~~~~~~~~~~~~~~~HELPERS~~~~~~~~~~~~~~~~~~~~~~~~~
Matrix Matrix::elementwise(const Matrix &m, std::function<double(double, double)> f) const
{
  // Deal with scalar multiplication
  bool amIScalar = this->getRows() == 1 && this->getCols() == 1;
  bool isOtherScalar = m.getRows() == 1 && m.getCols() == 1;

  if (amIScalar || isOtherScalar)
  {

    // if other is scalar then swap

    double n = amIScalar ? this->flatten()[0] : m.flatten()[0];
    unsigned int rows = amIScalar ? m.getRows() : this->getRows();
    unsigned int cols = amIScalar ? m.getCols() : this->getCols();

    Matrix t(rows, cols);
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

Matrix Matrix::apply(std::function<double(double)> f) const
{
  Matrix m(this->rows, this->cols);
  for (unsigned int i = 0; i < m.rows; i++)
  {
    for (unsigned int j = 0; j < m.cols; j++)
    {
      m.d[i][j] = f(this->d[i][j]);
    }
  }
  return m;
}

Matrix Matrix::fromArray(const std::vector<double> &arr)
{
  Matrix m(1, arr.size());
  for (unsigned int i = 0; i < m.cols; i++)
  {
    m.d[0][i] = arr[i];
  }
  return m;
}

Matrix Matrix::ones(unsigned int rows, unsigned int cols)
{
  return Matrix(rows, cols, 1);
}

Matrix Matrix::zeros(unsigned int rows, unsigned int cols)
{
  return Matrix(rows, cols, 0);
}

std::vector<double> Matrix::flatten(void) const
{
  std::vector<double> flat;
  for (auto const &r : this->d)
  {
    for (double const &v : r)
    {
      flat.push_back(v);
    }
  }
  return flat;
}

void Matrix::print(int precision, std::ostream *op) const
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

unsigned int Matrix::getRows() const
{
  return this->rows;
}
unsigned int Matrix::getCols() const
{
  return this->cols;
}

void Matrix::setName(std::string name)
{
  this->name = name;
}
