#include <iostream>
#include <cstdlib>

#include "matrix.hpp"
#include "neural_network.hpp"

using std::cout;
using std::endl;

int main()
{
  std::cout << "Neural Networks :)\n";

  // Test matrices

  // Matrix<double> m1(2, 2, 1.4, "M1");
  // Matrix<double> m2(2, 2);
  // m1.randomise();
  // m2.randomise();
  // m1.print();
  // m2.print();
  // m1.add(m2).print();
  // m1.multiply(m2).print();
  // NeuralNetwork<double>::applyActivationFunction(m1, NeuralNetwork<double>::Sigmoid).print();

  //define nn architecture
  std::vector<int> layers = {2, 4, 2};
  //make nn
  NeuralNetwork<double> nn(layers);
  nn.print();

  auto f = [](const Matrix<double> &m) {
    auto flat = m.flatten();
    return (flat.front() == flat.back()) ? Matrix<double>::fromArray({0.0, 1.0}) : Matrix<double>::fromArray({1.0, 0.0});
  };

  //test on some random inputs
  Matrix<double> inp1 = Matrix<double>::fromArray({0.0, 0.0});
  Matrix<double> inp2 = Matrix<double>::fromArray({1.0, 0.0});
  Matrix<double> inp3 = Matrix<double>::fromArray({0.0, 1.0});
  Matrix<double> inp4 = Matrix<double>::fromArray({1.0, 1.0});

  Matrix<double> o = nn.feedforward(inp1);
  o.print(7);
  o = nn.feedforward(inp2);
  o.print(7);
  o = nn.feedforward(inp3);
  o.print(7);
  o = nn.feedforward(inp4);
  o.print(7);

  int start_s = clock();

  //train many sets
  for (unsigned int i = 0; i < 1'000; i++)
  {
    Matrix<double> inp = Matrix<double>::fromArray({static_cast<double>(std::rand() % 2), static_cast<double>(std::rand() % 2)});
    Matrix<double> t = f(inp);
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

  nn.print();

  //test on same inputs
  o = nn.feedforward(inp1);
  o.print(7);
  o = nn.feedforward(inp2);
  o.print(7);
  o = nn.feedforward(inp3);
  o.print(7);
  o = nn.feedforward(inp4);
  o.print(7);
}
