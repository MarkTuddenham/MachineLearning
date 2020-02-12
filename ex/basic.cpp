#include <iostream>
#include <cstdlib>

#include "teslyn/core/matrix.hpp"
#include "teslyn/core/neural_network.hpp"
#include "teslyn/utils/printer.hpp"
#include "teslyn/utils/timer.hpp"

using Teslyn::Matrix;
using Teslyn::NeuralNetwork;

using Teslyn::display::print_matrix;
using Teslyn::display::print_nn;

using std::cout;
using std::endl;

int main()
{
  std::cout << "Neural Networks :)\n\n";

  //define nn architecture
  std::vector<int> layers = {2, 3, 1};

  //make nn
  NeuralNetwork nn(layers);
  print_nn(nn);
  std::cout << "\n";

  // xor function
  auto f = [](const Matrix &m) {
    auto flat = m.get_flattened();
    return (flat.front() == flat.back()) ? Matrix::from_array({0.0}) : Matrix::from_array({1.0});
  };

  // test on some inputs
  Matrix inp1 = Matrix::from_array({0.0, 0.0});
  Matrix inp2 = Matrix::from_array({0.0, 1.0});
  Matrix inp3 = Matrix::from_array({1.0, 0.0});
  Matrix inp4 = Matrix::from_array({1.0, 1.0});

  Matrix o = nn.feedforward(inp1);
  print_matrix(o, 7);
  o = nn.feedforward(inp2);
  print_matrix(o, 7);
  o = nn.feedforward(inp3);
  print_matrix(o, 7);
  o = nn.feedforward(inp4);
  print_matrix(o, 7);

  double lr = 0.5;

  {
    auto timer = Teslyn::time::Timer("\nTraining");

    //train many sets
    for (unsigned int i = 0; i < 10'000; i++)
    {
      Matrix x = Matrix::from_array({static_cast<double>(std::rand() % 2), static_cast<double>(std::rand() % 2)});
      Matrix y = f(x);
      // std::cout << inp[0] << ":" << inp[1] << "  =  " << t[0] << '\n';
      nn.backpropagation(x, y, lr);
    }
  }

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
