#include <iomanip>
#include <iostream>
#include "sir.hpp"
#include <stdexcept>


void print(std::vector<State> const& states)
{

  std::cout << "+-----------+-----------+-----------+\n"
               "|     S     |     I     |     R     |\n"
               "+-----------+-----------+-----------+\n"
            << std::fixed;
  for (auto& state : states) {
    std::cout << "| " << std::setw(9) << state.S << " | " << std::setw(9) << state.I << " | "<< std::setw(9) << state.R<<" |\n";
  }
  std::cout << "+-----------+-----------+-----------+\n";
}


/*
void print_simple(std::vector<State> const& states)
{
  double const Au = 1.496e11;

  std::cout << "x y\n";
  for (auto& state : states) {
    std::cout << state.x / Au << ' ' << state.y / Au << '\n';
  }
}
*/

int main(int argc, char* argv[]) {
try {
if (argc!=5) {
  throw std::runtime_error{"not enough inputs"};
}
if (argc<5) {
  throw std::runtime_error{""};
}
if (argc<5) {
  throw std::runtime_error{""};
}
if (argc<5) {
  throw std::runtime_error{""};
}
if (argc<5) {
  throw std::runtime_error{""};
}

int S0=std::atoi(argv[1]);
int I0=std::atoi(argv[2]);
int T=std::atoi(argv[3]);
double beta=std::stod(argv[4]);
double gamma=std::stod(argv[5]);

State s0{};
s0.S=S0;
s0.I =I0;
s0.R =0;
s0.b =beta;
s0.g =gamma;
Epidemic covid{s0};
auto const states=covid.evolve(T);
print(states);
    
}
catch (std::runtime_error const& e) {
  std::cerr << e.what() << '\n';

} 
}


