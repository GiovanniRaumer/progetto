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



int main(int argc, char* argv[]) {
try {

if (argc!=6) {
  throw std::runtime_error{"5 inputs required"};
}

for (int i=1; i!=4;++i) {
  if (atoi(argv[i])<=0) {
    throw std::runtime_error{"inputs must be positive and different from zero"};
  } 
}

if (std::stod(argv[4])>1 || std::stod(argv[5])>1 || std::stod(argv[4])<=0. || std::stod(argv[5])<=0.) {
  throw std::runtime_error{"gamma and beta must be positive and less than 1"};
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
if (states.back().I==0) {
  std::cout<<"ciao\n";
}
    
}
catch (std::runtime_error const& e) {
  std::cerr << e.what() << '\n';

} 
}


