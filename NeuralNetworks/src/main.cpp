#include "Matrix.hpp"
#include "NeuralNetwork.hpp"
#include <iostream>
#include <cstdlib>


int main() {
  std::cout << "Neural Networks :)"  << '\n';

  // Test matricies

  // Matrix m1(2,2,1.4,"M1");
  // Matrix m2(2,2);
  // m1.randomise();
  // m2.randomise();
  // m1.print();
  // m2.print();
  // m1.add(m2).print();
  // m1.multiply(m2).print();
  // NeuralNetwork::applyActivationFunction(m1,NeuralNetwork::Sigmoid).print();


  //define nn architecure
  std::vector<int> layers = {2,3,1};
  //make nn
  NeuralNetwork nn(layers);


  std::function<double(double, double)> f = [](double d1,double d2){
    if (d1==d2){
      return 0.0;
    }else{
      return 1.0;
    }};

  //test on some random inputs
  double inp1[] = {0.0,0.0}, inp2[] = {1.0,0.0}, inp3[] = {0.0,1.0}, inp4[] = {1.0,1.0};
  Matrix o = nn.feedforward(inp1);
  o.print(7);
  o = nn.feedforward(inp2);
  o.print(7);
  o = nn.feedforward(inp3);
  o.print(7);
  o = nn.feedforward(inp4);
  o.print(7);

  int start_s=clock();
  //train many set
  for (size_t i = 0; i < 100000; i++) {
    double inp[] = {(double)(std::rand()%2),(double)(std::rand()%2)};
    double t[] = {f(inp[0],inp[1])};
    // std::cout << inp[0] << ":" << inp[1] << "  =  " << t[0] << '\n';
    nn.backpropagation(inp, t, 5);
  }
  int stop_s=clock();
  std::cout<< "~~~~~~~~~~~~~~~NN Trained~~~~~~~~~~~~~\n";
  std::cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)<< "s\n";



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

