#include "Matrix.hpp"


#include <stdexcept>
#include <numeric>
#include <random>
#include <iomanip>


// Matrix::Matrix(int rows, int cols):  Matrix("M", rows, cols, 0.0){};

Matrix::Matrix(int rows, int cols, double fill, std::string name) : rows(rows), cols(cols),  name(name) {
  this->d = std::vector< std::vector<double> > (rows, std::vector<double> (cols, fill) );
};



//~~~~~~~~~~~~~~~~~MATHS~~~~~~~~~~~~~~~~~~~~~~~~~
// [this][m]
Matrix Matrix::multiply(Matrix m) const {
  //check dimensions
  if(this->cols != m.rows){
    throw std::invalid_argument("Internal dimensions of matricies do not match: " +
     std::to_string(this->rows) + "x" + std::to_string(this->cols) + " and " +
     std::to_string(m.rows) + "x" + std::to_string(m.cols));

  } else {
    Matrix res(this->rows,m.cols);
    Matrix mt = m.transpose();
    for (size_t i = 0; i < res.rows; i++) {
      for (size_t j = 0; j < res.cols; j++) {
        //this->row[i] * m.column[j]
        res.d[i][j] = std::inner_product(this->d[i].data(), this->d[i].data()+this->d[i].size(), mt.d[j].data(), (double) 0);
      }
    }
    return res;
  }
}
Matrix Matrix::operator* (const Matrix& m) const {
      return this->multiply(m);
}


Matrix Matrix::transpose(void) const {
  Matrix t(this->cols, this->rows);
  for (size_t i = 0; i < t.rows; i++) {
    for (size_t j = 0; j < t.cols; j++) {
      t.d[i][j] = this->d[j][i];
    }
  }
  return t;
}
Matrix Matrix::operator~ () const {
      return this->transpose();
}


Matrix Matrix::add(Matrix m) const{
  return this->elementwise(m, [](double a, double b){return a+b;} );
}
Matrix Matrix::operator+ (const Matrix& m) const {
      return this->add(m);
}

Matrix Matrix::subtract(Matrix m) const{
  return this->elementwise(m, [](double a, double b){return a-b;} );
}
Matrix Matrix::operator- (const Matrix& m) const {
      return this->subtract(m);
}

Matrix Matrix::hadamard(Matrix m) const {
  return this->elementwise(m, [](double a, double b){return a*b;} );
}
Matrix Matrix::operator->* (const Matrix& m) const {
      return this->hadamard(m);
}


Matrix Matrix::randomise(void){
  static double lower_bound = -1;
  static double upper_bound = 1;
  static std::uniform_real_distribution<double> gaussiand(lower_bound,upper_bound);
  static std::default_random_engine re(1289674753);

  for(auto& r: this->d) {
    for(double& v: r) {
      v = gaussiand(re);
    }
  }

  return *this;
}


//~~~~~~~~~~~~~~~~~HELPERS~~~~~~~~~~~~~~~~~~~~~~~~~
Matrix Matrix::elementwise(Matrix m, std::function<double(double,double)> f) const{
  if(this->rows == 1 && this->cols == 1){
    double n = this->d[0][0];
    Matrix t(m.rows, m.cols);
    for (size_t i = 0; i < t.rows; i++) {
      for (size_t j = 0; j < t.cols; j++) {
        t.d[i][j] = f(n, m.d[i][j]);
      }
    }
    return t;
  }

  if(this->rows != m.rows || this->cols != m.cols){
    throw std::invalid_argument("Matricies are not the same size! " +
     std::to_string(this->rows) + "x" + std::to_string(this->cols) + " and " +
     std::to_string(m.rows) + "x" + std::to_string(m.cols));

  } else {
    Matrix t(this->rows, this->cols);
    for (size_t i = 0; i < t.rows; i++) {
      for (size_t j = 0; j < t.cols; j++) {
        t.d[i][j] = f(this->d[i][j], m.d[i][j]);
      }
    }
    return t;
  }
}

Matrix Matrix::apply(std::function<double(double)> f) const {
  Matrix m(this->rows, this->cols);
  for (size_t i = 0; i < m.rows; i++) {
    for (size_t j = 0; j < m.cols; j++) {
      m.d[i][j] = f(this->d[i][j]);
    }
  }
  return m;
}

Matrix Matrix::fromArray(std::vector<double> arr){
  Matrix m(1, arr.size());
  for (size_t i = 0; i < m.cols; i++) {
      m.d[0][i] = arr[i];
  }
  return m;
}

std::vector<double> Matrix::flatten(void) const{
  std::vector<double> flat;
  for(auto const& r: this->d) {
    for(double const& v: r) {
      flat.push_back(v);
    }
  }
  return flat;
}

void Matrix::print(int precision, std::ostream* op) const{
  if(this->name != "") *op << this->name << ": ";
  *op << this->rows << 'x' << this->cols << " Matrix:" << '\n';
  *op << std::setprecision(precision) << std::fixed;

  for(auto const& r: this->d) {
    for(auto const& v: r) {
      *op << " | ";
      if(v>0) *op << " ";
      *op << v;

    }
    *op << " |\n";
  }
};

int Matrix::getRows() const {
  return this->rows;
}
int Matrix::getCols() const {
  return this->cols;
}

void Matrix::setName(std::string name){
  this->name = name;
}
