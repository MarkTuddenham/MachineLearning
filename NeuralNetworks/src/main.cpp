#include "Matrix.hpp"
#include "NeuralNetwork.hpp"
#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

int main()
{
  std::cout << "Neural Networks :)" << '\n';

  // Test matrices

  // Matrix m1(2,2,1.4,"M1");
  // Matrix m2(2,2);
  // m1.randomise();
  // m2.randomise();
  // m1.print();
  // m2.print();
  // m1.add(m2).print();
  // m1.multiply(m2).print();
  // NeuralNetwork::applyActivationFunction(m1,NeuralNetwork::Sigmoid).print();

  //define nn architecture
  std::vector<int> layers = {2, 3, 2};
  //make nn
  NeuralNetwork nn(layers);

  auto f = [](const Matrix &m) {
    auto flat = m.flatten();
    return (flat.front() == flat.back()) ? Matrix::fromArray({0.0, 1.0}) : Matrix::fromArray({1.0, 0.0});
  };

  //test on some random inputs
  Matrix inp1 = Matrix::fromArray({0.0, 0.0}), inp2 = Matrix::fromArray({1.0, 0.0}), inp3 = Matrix::fromArray({0.0, 1.0}), inp4 = Matrix::fromArray({1.0, 1.0});

  Matrix o = nn.feedforward(inp1);
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
    Matrix inp = Matrix::fromArray({(double)(std::rand() % 2), (double)(std::rand() % 2)});
    Matrix t = f(inp);
    // std::cout << inp[0] << ":" << inp[1] << "  =  " << t[0] << '\n';

    nn.backpropagation(inp, t, 1.5);
  }

  int stop_s = clock();
  std::cout << "~~~~~~~~ NN Trained ";
  std::cout << "in " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << "s ~~~~~~~~\n";

  //test on same inputs
  o = nn.feedforward(inp1);
  o.print(7);
  o = nn.feedforward(inp2);
  o.print(7);
  o = nn.feedforward(inp3);
  o.print(7);
  o = nn.feedforward(inp4);
  o.print(7);

  return 0;
}
