#include <iostream>

#include "NeuralNetwork.hpp"
#include "utils.hpp"

int main () {
  std::vector<int> v = {2, 2, 2};
  NeuralNetwork a(v);
  NeuralNetwork b(v);
  NeuralNetwork c(a, b);
  a.printGene();
  a.print();
  std::cout << "---" << std::endl;
  b.printGene();
  b.print();
  std::cout << "---" << std::endl;
  c.printGene();
  c.print();


  return 0;
}
