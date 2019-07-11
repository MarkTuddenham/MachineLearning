#include <iostream>
#include <cstdlib>

#include "matrix.hpp"
#include "neural_network.hpp"
#include "utils/printer.hpp"

using Teslyn::Matrix;
using Teslyn::NeuralNetwork;

using Teslyn::display::print_matrix;
using Teslyn::display::print_nn;

using std::cout;
using std::endl;

int main()
{
  std::cout << "Neural Networks :)\n";

  //define nn architecture
  std::vector<int> layers = {2, 4, 2};
  //make nn
  NeuralNetwork nn(layers);
  print_nn(nn);

  auto f = [](const Matrix &m) {
    auto flat = m.get_flattened();
    return (flat.front() == flat.back()) ? Matrix::from_array({0.0, 1.0}) : Matrix::from_array({1.0, 0.0});
  };

  //test on some random inputs
  Matrix inp1 = Matrix::from_array({0.0, 0.0});
  Matrix inp2 = Matrix::from_array({1.0, 0.0});
  Matrix inp3 = Matrix::from_array({0.0, 1.0});
  Matrix inp4 = Matrix::from_array({1.0, 1.0});

  Matrix o = nn.feedforward(inp1);
  print_matrix(o, 7);
  o = nn.feedforward(inp2);
  print_matrix(o, 7);
  o = nn.feedforward(inp3);
  print_matrix(o, 7);
  o = nn.feedforward(inp4);
  print_matrix(o, 7);

  int start_s = clock();

  //train many sets
  for (unsigned int i = 0; i < 1'000; i++)
  {
    Matrix inp = Matrix::from_array({static_cast<double>(std::rand() % 2), static_cast<double>(std::rand() % 2)});
    Matrix t = f(inp);
    // std::cout << inp[0] << ":" << inp[1] << "  =  " << t[0] << '\n';
    nn.backpropagation(inp, t, 4.5);
  }

  int stop_s = clock();
  cout << std::endl
       << "~~~~~~~~ NN Trained "
       << "in "
       << (stop_s - start_s) / double(CLOCKS_PER_SEC)
       << "s ~~~~~~~~"
       << std::endl;

  print_nn(nn);

  //test on same inputs
  o = nn.feedforward(inp1);
  print_matrix(o, 7);
  o = nn.feedforward(inp2);
  print_matrix(o, 7);
  o = nn.feedforward(inp3);
  print_matrix(o, 7);
  o = nn.feedforward(inp4);
  print_matrix(o, 7);
}
